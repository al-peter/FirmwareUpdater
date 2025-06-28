#ifndef FIRMWAREUPDATER_H
#define FIRMWAREUPDATER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QDate>
#include <QMessageBox>
#include <QtConcurrent>
#include <QFile>
#include <QTextStream>

class FirmwareUpdater : public QObject
{
    Q_OBJECT

public:
    explicit FirmwareUpdater(QObject *parent,
                             const QString& user,
                             const QString& password,
                             const QString& ip,
                             const QString& plink,
                             const QString& pscp,
                             const QString& localBasePath)/*,
                             const QString& hostKey)*/;

    void startUpdate(const QString& dirName);
    QString fetchHostKey();

signals:
    void updateFinished(bool success, const QString& message);

private:
    const QString remoteUser;
    const QString remotePassword;
    const QString currentRemoteIp;
    const QString plinkPath;
    const QString pscpPath;
    const QString baseLocalPath;
    QString serverHostKey;

    bool executePlinkCommand(const QString& command, int timeoutMs = 10000);

    bool directoryExistsOnOrangePi(const QString& dirName);
    bool makeBackupIfExists();
    bool createRemoteFolder();
    bool copyFirmwareFolder(const QString& dirName);
    bool updateWlanConfigIfNeeded();
    bool restartService();
    bool rebootDevice();
};
#endif // FIRMWAREUPDATER_H
