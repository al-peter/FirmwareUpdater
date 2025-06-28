#ifndef MAVLINKFILESENDER_H
#define MAVLINKFILESENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <QtGlobal>
#include <QThread>
#include "mavlink.h"

class MavlinkFileSender : public QObject
{
    Q_OBJECT
public:
    explicit MavlinkFileSender(QObject *parent = nullptr);

    void setTarget(const QString& ip, quint16 port);
    void sendFile(const QString& localFilePath);

signals:
    void fileSent(bool success, const QString& message);

private slots:
    void onSocketReadyRead();
    void onAckTimeout();

private:
    void sendCreateFile();
    void sendNextChunk();
    void sendTerminateSession();

    void handleAck();
    void handleNak();
    void resendLastPacket();

    QUdpSocket* udpSocket = nullptr;
    QString targetIp;
    quint16 targetPort; //14550

    QFile file;
    quint8 session = 0;
    qint64 bytesSent = 0;
    const qint64 chunkSize = 128;

    QTimer ackTimeoutTimer;
    QByteArray lastSentPacket;
    uint8_t lastOpcodeSent = 0;
    const int maxRetries = 5;
    int retryCount = 0;
};

#endif // MAVLINKFILESENDER_H
