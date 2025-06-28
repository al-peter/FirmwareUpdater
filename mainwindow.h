#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSet>
#include <QVector>
#include <QRegularExpression>
#include <QTcpSocket>
#include <QFileDialog>
#include "firmwareupdater.h"
#include "mavlinkfilesender.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct DeviceInfo {
    QString ip;
    QString folder;
    QLabel* statusLabel;
    QPushButton* updateButton;
    //QString hostKey;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void downloadFinished(bool success, const QString& message);

private slots:
    void onDownloadFirmwareClicked();
    void checkAllDevices();
    void updateFirmware75();
    void updateFirmware100();
    void updateFirmware125();
    void updateFirmware185();
    void onSendFileClicked();

private:
    Ui::MainWindow *ui;
    QTimer *pingTimer;
    QVector<DeviceInfo> devices;
    QSet<QString> updatingDevices;
    QString firmwareBasePath;
    QString baseFtpUrl;

    MavlinkFileSender *fileSender = nullptr;

    void setupDevices();
    void updateDeviceStatus(const DeviceInfo& device, bool reachable);
    void updateAllVersions();
    void startFirmwareUpdate(const QString& ip, const QString& folder);
    bool isHostReachable(const QString& ip, quint16 port = 22, int timeout = 1000);
};
#endif // MAINWINDOW_H
