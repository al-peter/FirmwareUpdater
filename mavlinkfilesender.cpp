#include "mavlinkfilesender.h"

MavlinkFileSender::MavlinkFileSender(QObject *parent)
    : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &MavlinkFileSender::onSocketReadyRead);
    connect(&ackTimeoutTimer, &QTimer::timeout, this, &MavlinkFileSender::onAckTimeout);
}

void MavlinkFileSender::setTarget(const QString &ip, quint16 port)
{
    targetIp = ip;
    targetPort = port;
}

void MavlinkFileSender::sendFile(const QString &localFilePath)
{
    if (file.isOpen()) {
        file.close();
    }
    file.setFileName(localFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit fileSent(false, "Cannot open file: " + localFilePath);
        return;
    }

    bytesSent = 0;
    session = 0;
    retryCount = 0;

    // firstly, send the CreateFile package with the file name
    sendCreateFile();
}

void MavlinkFileSender::sendCreateFile()
{
    mavlink_message_t message{};
    mavlink_file_transfer_protocol_t ftp_pkt{};
    memset(&ftp_pkt, 0, sizeof(ftp_pkt));

    ftp_pkt.target_network = 0;
    ftp_pkt.target_system = 1;
    ftp_pkt.target_component = 1;

    ftp_pkt.payload[0] = 6; // OpCode CreateFile
    ftp_pkt.payload[1] = session;

    QByteArray name = QFileInfo(file.fileName()).fileName().toUtf8();
    int nameLen = qMin(name.size(), 239 - 2); // leave 2 bytes for OpCode and Session
    memcpy(&ftp_pkt.payload[2], name.constData(), nameLen);

    mavlink_msg_file_transfer_protocol_encode(1, 1, &message, &ftp_pkt);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &message);
    lastSentPacket = QByteArray(reinterpret_cast<char*>(buffer), len);
    lastOpcodeSent = 6;

    udpSocket->writeDatagram(lastSentPacket, QHostAddress(targetIp), targetPort);

    qDebug() << "CreateFile sent for" << name;

    ackTimeoutTimer.start(300);
}

void MavlinkFileSender::sendNextChunk()
{
    if (!file.isOpen()) {
        emit fileSent(false, "File not open");
        return;
    }

    if (file.atEnd()) {
        // send TerminateSession
        sendTerminateSession();
        return;
    }

    QByteArray chunk = file.read(chunkSize);

    mavlink_message_t message{};
    mavlink_file_transfer_protocol_t ftp_pkt{};
    memset(&ftp_pkt, 0, sizeof(ftp_pkt));

    ftp_pkt.target_network = 0;
    ftp_pkt.target_system = 1;
    ftp_pkt.target_component = 1;

    ftp_pkt.payload[0] = 7; // OpCode WriteFile
    ftp_pkt.payload[1] = session;

    // write 4-byte offset (little-endian)
    ftp_pkt.payload[2] = (bytesSent & 0xFF);         // bytesSent(0) & 11111111(0xFF) = 00000000
    ftp_pkt.payload[3] = ((bytesSent >> 8) & 0xFF);  // bytesSent >> 8 => 00000000 00000000 & 11111111 => 00000000 00000000
    ftp_pkt.payload[4] = ((bytesSent >> 16) & 0xFF); // bytesSent >> 16 => 00000000 00000000 00000000 & 11111111 => 00000000 00000000 00000000
    ftp_pkt.payload[5] = ((bytesSent >> 24) & 0xFF); // bytesSent >> 24 => ... => 00000000 00000000 00000000 00000000

    int copyLen = qMin(chunk.size(), 239 - 6);
    memcpy(&ftp_pkt.payload[6], chunk.constData(), copyLen);

    mavlink_msg_file_transfer_protocol_encode(1, 1, &message, &ftp_pkt);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &message);

    lastSentPacket = QByteArray(reinterpret_cast<char*>(buffer), len);
    lastOpcodeSent = 7;
    udpSocket->writeDatagram(lastSentPacket, QHostAddress(targetIp), targetPort);

    ackTimeoutTimer.start(300);

    qDebug() << "WriteFile chunk sent, offset:" << bytesSent << "size:" << copyLen;

    bytesSent += copyLen;
}

void MavlinkFileSender::sendTerminateSession()
{
    mavlink_message_t message{};
    mavlink_file_transfer_protocol_t ftp_pkt{};
    memset(&ftp_pkt, 0, sizeof(ftp_pkt));

    ftp_pkt.target_network = 0;
    ftp_pkt.target_system = 1;
    ftp_pkt.target_component = 1;

    ftp_pkt.payload[0] = 1; // OpCode TerminateSession
    ftp_pkt.payload[1] = session;

    mavlink_msg_file_transfer_protocol_encode(1, 1, &message, &ftp_pkt);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &message);

    lastSentPacket = QByteArray(reinterpret_cast<char*>(buffer), len);
    lastOpcodeSent = 1;
    udpSocket->writeDatagram(lastSentPacket, QHostAddress(targetIp), targetPort);

    ackTimeoutTimer.start(300);

    qDebug() << "TerminateSession sent";
}

void MavlinkFileSender::onSocketReadyRead()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        // handle ACK/NAK here
        mavlink_message_t msg;
        mavlink_status_t status;
        for (int i = 0; i < datagram.size(); ++i) {
            if (mavlink_parse_char(MAVLINK_COMM_0, static_cast<uint8_t>(datagram[i]), &msg, &status)) {
                if (msg.msgid == MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL) {
                    mavlink_file_transfer_protocol_t ftp;
                    mavlink_msg_file_transfer_protocol_decode(&msg, &ftp);

                    uint8_t opcode = ftp.payload[0];
                    if (opcode == 128) { // ACK
                        handleAck();
                    } else if (opcode == 129) { // NAK
                        handleNak();
                    }
                }
            }
        }
    }
}


void MavlinkFileSender::onAckTimeout()
{
    if (++retryCount > maxRetries) {
        emit fileSent(false, "No ACK received. Giving up.");
        file.close();
        ackTimeoutTimer.stop();
        return;
    }

    qDebug() << "ACK timeout. Retrying last packet. Attempt" << retryCount;
    resendLastPacket();
}

void MavlinkFileSender::resendLastPacket()
{
    udpSocket->writeDatagram(lastSentPacket, QHostAddress(targetIp), targetPort);
    ackTimeoutTimer.start(300);
}

void MavlinkFileSender::handleAck()
{
    ackTimeoutTimer.stop();
    retryCount = 0;

    if (lastOpcodeSent == 6) {
        qDebug() << "ACK for CreateFile received.\n";
        sendNextChunk();
    } else if (lastOpcodeSent == 7) {
        qDebug() << "ACK for WriteFile chunk.\n";
        sendNextChunk();
    } else if (lastOpcodeSent == 1) {
        qDebug() << "ACK for TerminateSession. Transfer complete.\n";
        file.close();
        emit fileSent(true, "File sent successfully.");
    }
}

void MavlinkFileSender::handleNak()
{
    qDebug() << "NAK received. Retrying.";
    onAckTimeout();
}
