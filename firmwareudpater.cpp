#include "firmwareupdater.h"

// Constructor: initializes all configuration parameters
FirmwareUpdater::FirmwareUpdater(QObject *parent,
                                 const QString& user,
                                 const QString& password,
                                 const QString& ip,
                                 const QString& plink,
                                 const QString& pscp,
                                 const QString& localBasePath/*,
                                 const QString& hostKey*/)
    : QObject(parent),
    remoteUser(user),
    remotePassword(password),
    currentRemoteIp(ip),
    plinkPath(plink),
    pscpPath(pscp),
    baseLocalPath(localBasePath)/*,
    serverHostKey(hostKey)*/
{
}

// Executes plink command with given timeout (without output param)
bool FirmwareUpdater::executePlinkCommand(const QString& command, int timeoutMs)
{
    QStringList arguments = {
        "-pw", remotePassword,
        "-batch",
        "-hostkey", serverHostKey,
        QString("%1@%2").arg(remoteUser, currentRemoteIp),
        command
    };

    QProcess process;
    process.start(plinkPath, arguments);
    if (!process.waitForFinished(timeoutMs))
        return false;

    return (process.exitCode() == 0);
}

// Extracts SSH host key for current device
QString FirmwareUpdater::fetchHostKey()
{
    QStringList arguments = {
        "-batch",
        "-v",
        "-ssh",
        "-pw", remotePassword,
        QString("%1@%2").arg(remoteUser, currentRemoteIp),
        "exit"
    };

    QProcess process;
    process.setProgram(plinkPath);
    process.setArguments(arguments);
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start();

    if (!process.waitForStarted(3000)) {
        qDebug() << "Failed to start plink.";
        return QString();
    }

    if (!process.waitForFinished(10000)) {
        qDebug() << "Timeout while fetching host key.";
        process.kill();
        process.waitForFinished();
    }

    QString output = process.readAllStandardError() + process.readAllStandardOutput();;
    qDebug() << "Plink output:" << output;

    // Parse host key from plink verbose output
    static const QRegularExpression regex(R"((ssh-(ed25519|rsa|dss|ecdsa))\s+\d+\s+SHA256:[^\r\n]+)");
    QRegularExpressionMatch match = regex.match(output);
    if (match.hasMatch()) {
        QString hostKey = match.captured(0).trimmed();
        qDebug() << "\nExtracted host key:" << hostKey;
        return hostKey;
    } else {
        qDebug() << "Host key not found in output.";
        return QString();
    }
}

// Checks if remote directory exists
bool FirmwareUpdater::directoryExistsOnOrangePi(const QString& dirName)
{
    QString command = QString("sudo test -d /usr/sbin/%1 && echo exists").arg(dirName);
    QStringList arguments = {
        "-pw", remotePassword,
        "-batch",
        "-hostkey", serverHostKey,
        QString("%1@%2").arg(remoteUser, currentRemoteIp),
        command
    };

    QProcess process;
    process.start(plinkPath, arguments);
    if (!process.waitForFinished(5000)) return false;

    QString output = process.readAllStandardOutput().trimmed();
    return output.contains("exists");
}

// Creates backup folder if needed, adds suffix _date_N if name conflict
bool FirmwareUpdater::makeBackupIfExists()
{
    QString dirName = "wfb_server";

    if (!directoryExistsOnOrangePi(dirName))
        return true;

    QString date = QDate::currentDate().toString("ddMMyy");
    QString baseBackupName = QString("%1_backup_%2").arg(dirName, date);
    QString backupName = baseBackupName;
    int counter = 1;

    // Handle multiple backups in one day
    while (directoryExistsOnOrangePi(backupName)) {
        backupName = QString("%1_%2").arg(baseBackupName).arg(counter++);
    }

    QString moveCommand = QString("sudo mv /usr/sbin/%1 /usr/sbin/%2").arg(dirName, backupName);
    return executePlinkCommand(moveCommand, 10000);
}

// Creates fresh folder for firmware
bool FirmwareUpdater::createRemoteFolder()
{
    QString createCommand = "sudo mkdir -p /usr/sbin/wfb_server/";
    return executePlinkCommand(createCommand, 10000);
}

// Copies firmware files to remote device
bool FirmwareUpdater::copyFirmwareFolder(const QString& localDirName)
{
    QString localFolderPath = baseLocalPath + localDirName + "/wfb_server"; // localDirName = folder
    const QString remoteFolderPath = "/usr/sbin/";

    QStringList scpArgs = {
        "-pw", remotePassword,
        "-hostkey", serverHostKey,
        "-r",
        "-batch",
        localFolderPath,
        QString("%1@%2:%3").arg(remoteUser, currentRemoteIp, remoteFolderPath)
    };

    QProcess scpProcess;
    scpProcess.setProgram(pscpPath);
    scpProcess.setArguments(scpArgs);
    scpProcess.setProcessChannelMode(QProcess::MergedChannels);
    scpProcess.start();
    if (!scpProcess.waitForFinished(30000)) return false;

    if (scpProcess.exitCode() != 0) return false;

    QString chmodCommand = "sudo chmod -R a+x /usr/sbin/wfb_server";
    return executePlinkCommand(chmodCommand, 10000);
}

// Update WLAN config (only for device with IP ending with ".75")
bool FirmwareUpdater::updateWlanConfigIfNeeded()
{
    // only for 75 IP
    if (!currentRemoteIp.endsWith(".75"))
    {
        qDebug() << "updateWlanConfig skipped for non-75 device.";
        return true;
    }

    // Execute iwconfig to get wireless interfaces
    QString iwconfigCommand = "iwconfig";
    QStringList iwArguments = {
        "-pw", remotePassword,
        "-batch",
        "-hostkey", serverHostKey,
        QString("%1@%2").arg(remoteUser, currentRemoteIp),
        iwconfigCommand
    };

    QProcess iwProcess;
    iwProcess.start(plinkPath, iwArguments);
    if (!iwProcess.waitForFinished(10000)) {
        qDebug() << "Failed to execute iwconfig.";
        return false;
    }

    QString iwOutput = iwProcess.readAllStandardOutput();
    qDebug() << "\niwconfig output:" << iwOutput;

    // Parse wlan interfaces (wlxXXXXXXXXXXXX format)
    static const QRegularExpression ifaceRegex(R"(wlx[0-9a-fA-F]{12})");
    QRegularExpressionMatchIterator i = ifaceRegex.globalMatch(iwOutput);
    QStringList wlanInterfaces;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString iface = match.captured(0);
        if (!wlanInterfaces.contains(iface))
            wlanInterfaces << iface;
    }

    if (wlanInterfaces.isEmpty()) {
        qDebug() << "No wireless interfaces found!";
        return false;
    }

    QString wlanLine = wlanInterfaces.join(" ");
    qDebug() << "\nNew wlan config:" << wlanLine;

    // Replace wlan = ... line in config file
    QString sedCommand = QString(
                             "sudo sed -i '/^\\[wlan\\]/,/^\\[/ s/^wlan =.*/wlan = %1/' /usr/sbin/wfb_server/wfb_server.cfg"
                             ).arg(wlanLine);
    return executePlinkCommand(sedCommand, 10000);
}

// Restarts service
bool FirmwareUpdater::restartService()
{  
    QString restartCommand = "sudo systemctl restart wfb.service"; //&& reboot - exitCode!= 0
    return executePlinkCommand(restartCommand, 15000);
}

//Reboot OrangePi
bool FirmwareUpdater::rebootDevice()
{
    QString rebootCommand = "nohup sudo reboot &";
    return executePlinkCommand(rebootCommand, 3000);
}

// Full firmware update pipeline
void FirmwareUpdater::startUpdate(const QString& localDirName)
{
    QtConcurrent::run([=]() {

        QString hostKey = fetchHostKey();
        if (hostKey.isEmpty()) {
            emit updateFinished(false, "Failed to fetch host key!");
            return;
        }
        this->serverHostKey = hostKey;

        if (!makeBackupIfExists()) {
            emit updateFinished(false, "Backup creation error!");
            return;
        }

        if (!createRemoteFolder()) {
            emit updateFinished(false, "Directory creation error!");
            return;
        }

        if (!copyFirmwareFolder(localDirName)) {
            emit updateFinished(false, "Copy Firmware error!");
            return;
        }

        if (!updateWlanConfigIfNeeded()) {
            emit updateFinished(false, "WLAN config update error!");
            return;
        }

        if (!restartService()) {
            emit updateFinished(false, "Service Restart error!");
            return;
        }

        rebootDevice();
        emit updateFinished(true, "Firmware successfully installed!" );
    });
}
