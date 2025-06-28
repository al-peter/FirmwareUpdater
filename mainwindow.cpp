#include "mainwindow.h"
#include "ui_mainwindow.h"

// Constructor: setup UI, initialize timers, buttons and ping check
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    firmwareBasePath = "D:/localFtpFiles/main/";

    setupDevices();

    pingTimer = new QTimer(this);
    connect(pingTimer, &QTimer::timeout, this, &MainWindow::checkAllDevices);
    pingTimer->start(5000); // periodic ping every 5 sec

    connect(ui->DownloadFirmware, &QPushButton::clicked, this, &MainWindow::onDownloadFirmwareClicked);
    connect(this, &MainWindow::downloadFinished, this, [=](bool success, const QString &message){
        QMessageBox::information(this, success ? "Success" : "Error", message);
    });

    connect(ui->Update75, &QPushButton::clicked, this, &MainWindow::updateFirmware75);
    connect(ui->Update100, &QPushButton::clicked, this, &MainWindow::updateFirmware100);
    connect(ui->Update125, &QPushButton::clicked, this, &MainWindow::updateFirmware125);
    connect(ui->Update185, &QPushButton::clicked, this, &MainWindow::updateFirmware185);

    checkAllDevices(); // initial check on app start

    fileSender = new MavlinkFileSender(this);
    fileSender->setTarget("10.59.59.197", 14550); // ip, port of target board to send via MavlinkFTP

    connect(fileSender, &MavlinkFileSender::fileSent, this, [=](bool success, const QString &msg){
        QMessageBox::information(this, success ? "Success" : "Error", msg);
    });

    connect(ui->sendFile, &QPushButton::clicked, this, &MainWindow::onSendFileClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Setup IPs, labels and buttons for each device
void MainWindow::setupDevices()
{
    devices = {
        {"192.168.144.75",  "75",  ui->labelConnectionStatus75,  ui->Update75 }, //10.59.59.95
        {"192.168.144.100", "100", ui->labelConnectionStatus100, ui->Update100},
        {"192.168.144.125", "125", ui->labelConnectionStatus125, ui->Update125},
        {"192.168.144.185", "185", ui->labelConnectionStatus185, ui->Update185}
    };
}

// Simple TCP ping to port 22
bool MainWindow::isHostReachable(const QString& ip, quint16 port, int timeout)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port);
    return socket.waitForConnected(timeout);
}

// Ping all devices in parallel using QtConcurrent
void MainWindow::checkAllDevices()
{
    for (const auto& device : devices)
    {
        if (updatingDevices.contains(device.ip)) continue;
        QtConcurrent::run([=]() {
            bool reachable = isHostReachable(device.ip, 22, 1000);
            QMetaObject::invokeMethod(this, [=]() {
                updateDeviceStatus(device, reachable);
            }, Qt::QueuedConnection);
        });
    }
}

// Update label and button based on ping result
void MainWindow::updateDeviceStatus(const DeviceInfo& device, bool reachable)
{
    if (reachable)
    {
        device.statusLabel->setText("Connected");
        device.statusLabel->setStyleSheet("color: green; background-color: rgb(255, 255, 255); font: 8pt Lucida Console; ");
        device.updateButton->setEnabled(true);
    }
    else
    {
        device.statusLabel->setText("Not Connected");
        device.statusLabel->setStyleSheet("color: red; background-color: rgb(255, 255, 255); font: 8pt Lucida Console; ");
        device.updateButton->setEnabled(false);
    }
}

// Start downloading firmware from FTP using wget
void MainWindow::onDownloadFirmwareClicked()
{
    baseFtpUrl = ui->firmwareUrl->text().trimmed(); //ftp://firmwareuser:orangepi@10.59.58.161/main/
    if(baseFtpUrl.isEmpty()){
        QMessageBox::warning(this,"WARNING", "Please, enter a firmware URL.");
        return;
    }
    if(!baseFtpUrl.endsWith("/")){
        baseFtpUrl += "/";
    }

    QStringList args;
    args << "-r"
         << "-nH"
         << "--cut-dirs=1"
         << "--ftp-user=firmwareuser"
         << "--ftp-password=orangepi"
         << "-P" << firmwareBasePath
         << baseFtpUrl;

    QString commandStr = "wget " + args.join(' ');
    qDebug() << "Running wget command:" << commandStr;

    QProcess *wgetProcess = new QProcess(this);
    connect(wgetProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitStatus == QProcess::NormalExit && exitCode == 0) {
                    qDebug() << "\nSuccessfully downloaded:" << baseFtpUrl << "\n";
                    emit downloadFinished(true, "Firmware successfully downloaded!");
                    updateAllVersions();
                } else {
                    qDebug() << "\nError while downloading:" << baseFtpUrl;
                    qDebug() << "\nstderr:" << wgetProcess->readAllStandardError();
                    emit downloadFinished(false, "Error! Firmware was not downloaded!");
                }
                wgetProcess->deleteLater();
            });

    connect(wgetProcess, &QProcess::errorOccurred,
            this, [](QProcess::ProcessError error) {
                qWarning() << "Process error during FTP download:" << error;
            });

    wgetProcess->start("C:/msys64/usr/bin/wget.exe", args);
}

// Parse downloaded firmware versions from config files
void MainWindow::updateAllVersions()
{
    struct Device {
        QString folder;
        QLabel* versionLabel;
    };

    QVector<Device> devices = {
        {"75", ui->WfbVersion75},
        {"100", ui->WfbVersion100},
        {"125", ui->WfbVersion125},
        {"185", ui->WfbVersion185}
    };

    for (const auto& device : devices)
    {
        QString cfgPath = firmwareBasePath + device.folder + "/wfb_server/wfb_server.cfg";
        QFile file(cfgPath);

        if (!file.exists()) {
            device.versionLabel->setText("Loaded Firmware: ---");
            continue;
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            device.versionLabel->setText("Loaded Firmware: ---");
            continue;
        }

        QTextStream in(&file);
        QString versionLine = in.readLine().trimmed();
        file.close();

        static const QRegularExpression regex(R"#(#wfb_server\s+v(\d+\.\d+\.\d+))#");
        QRegularExpressionMatch match = regex.match(versionLine);

        if (match.hasMatch()) {
            QString version = match.captured(1);
            device.versionLabel->setText("Loaded Firmware: " + version);
        } else {
            device.versionLabel->setText("Loaded Firmware: ---");
        }
    }
}

// Launch update process for selected device
void MainWindow::startFirmwareUpdate(const QString& ip, const QString& folder)
{
    updatingDevices.insert(ip);
    FirmwareUpdater *updater = new FirmwareUpdater(this,
                                                   "root", "orangepi", ip,
                                                   "C:/Program Files/PuTTY/plink.exe", "C:/Program Files/PuTTY/pscp.exe",
                                                   "D:/localFtpFiles/main/"
                                                   );

    connect(updater, &FirmwareUpdater::updateFinished, this, [=](bool success, const QString& message){
        QMessageBox::information(this, success ? "Success" : "Error", message);
        updatingDevices.remove(ip);
        updater->deleteLater();
    });

    updater->startUpdate(folder);
}

// Per-device button handlers
void MainWindow::updateFirmware75()  { startFirmwareUpdate("192.168.144.75",  "75"); } //10.59.59.95
void MainWindow::updateFirmware100() { startFirmwareUpdate("192.168.144.100", "100"); }
void MainWindow::updateFirmware125() { startFirmwareUpdate("192.168.144.125", "125"); }
void MainWindow::updateFirmware185() { startFirmwareUpdate("192.168.144.185", "185"); }

// Opens file dialog and sends selected file via MavlinkFileSender
void MainWindow::onSendFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select file to send");
    if (filePath.isEmpty()) return;

    fileSender->sendFile(filePath);
}


