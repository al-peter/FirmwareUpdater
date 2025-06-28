/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label75;
    QLabel *labelConnectionStatus75;
    QLabel *WfbVersion75;
    QPushButton *Update75;
    QVBoxLayout *verticalLayout_4;
    QLabel *label100;
    QLabel *labelConnectionStatus100;
    QLabel *WfbVersion100;
    QPushButton *Update100;
    QVBoxLayout *verticalLayout_5;
    QLabel *label125;
    QLabel *labelConnectionStatus125;
    QLabel *WfbVersion125;
    QPushButton *Update125;
    QVBoxLayout *verticalLayout_6;
    QLabel *label185;
    QLabel *labelConnectionStatus185;
    QLabel *WfbVersion185;
    QPushButton *Update185;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *FtpUrlLable;
    QLineEdit *firmwareUrl;
    QPushButton *DownloadFirmware;
    QPushButton *sendFile;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(863, 487);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 160, 841, 271));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label75 = new QLabel(layoutWidget);
        label75->setObjectName(QString::fromUtf8("label75"));
        label75->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 9pt \"Bahnschrift\";\n"
"color: rgb(0, 0, 0);"));
        label75->setFrameShape(QFrame::StyledPanel);
        label75->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label75);

        labelConnectionStatus75 = new QLabel(layoutWidget);
        labelConnectionStatus75->setObjectName(QString::fromUtf8("labelConnectionStatus75"));
        labelConnectionStatus75->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        labelConnectionStatus75->setFrameShape(QFrame::Panel);
        labelConnectionStatus75->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(labelConnectionStatus75);

        WfbVersion75 = new QLabel(layoutWidget);
        WfbVersion75->setObjectName(QString::fromUtf8("WfbVersion75"));
        WfbVersion75->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 8pt \"Lucida Console\";\n"
"color: rgb(0, 0, 0);"));
        WfbVersion75->setFrameShape(QFrame::StyledPanel);
        WfbVersion75->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(WfbVersion75);

        Update75 = new QPushButton(layoutWidget);
        Update75->setObjectName(QString::fromUtf8("Update75"));
        Update75->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe MDL2 Assets\";"));

        verticalLayout_3->addWidget(Update75);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label100 = new QLabel(layoutWidget);
        label100->setObjectName(QString::fromUtf8("label100"));
        label100->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 9pt \"Bahnschrift\";\n"
"color: rgb(0, 0, 0);"));
        label100->setFrameShape(QFrame::StyledPanel);
        label100->setTextFormat(Qt::AutoText);
        label100->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label100);

        labelConnectionStatus100 = new QLabel(layoutWidget);
        labelConnectionStatus100->setObjectName(QString::fromUtf8("labelConnectionStatus100"));
        labelConnectionStatus100->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        labelConnectionStatus100->setFrameShape(QFrame::Box);
        labelConnectionStatus100->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(labelConnectionStatus100);

        WfbVersion100 = new QLabel(layoutWidget);
        WfbVersion100->setObjectName(QString::fromUtf8("WfbVersion100"));
        WfbVersion100->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 8pt \"Lucida Console\";\n"
"color: rgb(0, 0, 0);"));
        WfbVersion100->setFrameShape(QFrame::StyledPanel);
        WfbVersion100->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(WfbVersion100);

        Update100 = new QPushButton(layoutWidget);
        Update100->setObjectName(QString::fromUtf8("Update100"));
        Update100->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe MDL2 Assets\";"));

        verticalLayout_4->addWidget(Update100);


        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label125 = new QLabel(layoutWidget);
        label125->setObjectName(QString::fromUtf8("label125"));
        label125->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 9pt \"Bahnschrift\";\n"
"color: rgb(0, 0, 0);"));
        label125->setFrameShape(QFrame::StyledPanel);
        label125->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label125);

        labelConnectionStatus125 = new QLabel(layoutWidget);
        labelConnectionStatus125->setObjectName(QString::fromUtf8("labelConnectionStatus125"));
        labelConnectionStatus125->setEnabled(true);
        labelConnectionStatus125->setAutoFillBackground(false);
        labelConnectionStatus125->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        labelConnectionStatus125->setFrameShape(QFrame::Box);
        labelConnectionStatus125->setFrameShadow(QFrame::Plain);
        labelConnectionStatus125->setScaledContents(false);
        labelConnectionStatus125->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(labelConnectionStatus125);

        WfbVersion125 = new QLabel(layoutWidget);
        WfbVersion125->setObjectName(QString::fromUtf8("WfbVersion125"));
        WfbVersion125->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 8pt \"Lucida Console\";\n"
"color: rgb(0, 0, 0);"));
        WfbVersion125->setFrameShape(QFrame::StyledPanel);
        WfbVersion125->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(WfbVersion125);

        Update125 = new QPushButton(layoutWidget);
        Update125->setObjectName(QString::fromUtf8("Update125"));
        Update125->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe MDL2 Assets\";"));

        verticalLayout_5->addWidget(Update125);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label185 = new QLabel(layoutWidget);
        label185->setObjectName(QString::fromUtf8("label185"));
        label185->setAutoFillBackground(false);
        label185->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 700 9pt \"Bahnschrift\";\n"
"color: rgb(0, 0, 0);"));
        label185->setFrameShape(QFrame::StyledPanel);
        label185->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label185);

        labelConnectionStatus185 = new QLabel(layoutWidget);
        labelConnectionStatus185->setObjectName(QString::fromUtf8("labelConnectionStatus185"));
        labelConnectionStatus185->setAutoFillBackground(false);
        labelConnectionStatus185->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        labelConnectionStatus185->setFrameShape(QFrame::Box);
        labelConnectionStatus185->setTextFormat(Qt::AutoText);
        labelConnectionStatus185->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(labelConnectionStatus185);

        WfbVersion185 = new QLabel(layoutWidget);
        WfbVersion185->setObjectName(QString::fromUtf8("WfbVersion185"));
        WfbVersion185->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"font: 8pt \"Lucida Console\";\n"
"color: rgb(0, 0, 0);"));
        WfbVersion185->setFrameShape(QFrame::StyledPanel);
        WfbVersion185->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(WfbVersion185);

        Update185 = new QPushButton(layoutWidget);
        Update185->setObjectName(QString::fromUtf8("Update185"));
        Update185->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe MDL2 Assets\";"));

        verticalLayout_6->addWidget(Update185);


        horizontalLayout_2->addLayout(verticalLayout_6);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(50, 50, 230, 68));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        FtpUrlLable = new QLabel(layoutWidget1);
        FtpUrlLable->setObjectName(QString::fromUtf8("FtpUrlLable"));
        FtpUrlLable->setStyleSheet(QString::fromUtf8("font: 700 9pt \"Consolas\";\n"
"color: rgb(0, 0, 0);"));
        FtpUrlLable->setFrameShadow(QFrame::Plain);

        horizontalLayout->addWidget(FtpUrlLable);

        firmwareUrl = new QLineEdit(layoutWidget1);
        firmwareUrl->setObjectName(QString::fromUtf8("firmwareUrl"));

        horizontalLayout->addWidget(firmwareUrl);


        verticalLayout->addLayout(horizontalLayout);

        DownloadFirmware = new QPushButton(layoutWidget1);
        DownloadFirmware->setObjectName(QString::fromUtf8("DownloadFirmware"));
        DownloadFirmware->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe MDL2 Assets\";"));

        verticalLayout->addWidget(DownloadFirmware);

        sendFile = new QPushButton(centralwidget);
        sendFile->setObjectName(QString::fromUtf8("sendFile"));
        sendFile->setGeometry(QRect(690, 10, 161, 31));
        sendFile->setStyleSheet(QString::fromUtf8("font: 9pt \"Segoe MDL2 Assets\";"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 863, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label75->setText(QCoreApplication::translate("MainWindow", "75", nullptr));
        labelConnectionStatus75->setText(QCoreApplication::translate("MainWindow", "Checking for connection...", nullptr));
        WfbVersion75->setText(QCoreApplication::translate("MainWindow", "LoadedFirmware ...", nullptr));
        Update75->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        label100->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        labelConnectionStatus100->setText(QCoreApplication::translate("MainWindow", "Checking for connection...", nullptr));
        WfbVersion100->setText(QCoreApplication::translate("MainWindow", "LoadedFirmware ...", nullptr));
        Update100->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        label125->setText(QCoreApplication::translate("MainWindow", "125", nullptr));
        labelConnectionStatus125->setText(QCoreApplication::translate("MainWindow", "Checking for connection...", nullptr));
        WfbVersion125->setText(QCoreApplication::translate("MainWindow", "LoadedFirmware ...", nullptr));
        Update125->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        label185->setText(QCoreApplication::translate("MainWindow", "185", nullptr));
        labelConnectionStatus185->setText(QCoreApplication::translate("MainWindow", "Checking for connection...", nullptr));
        WfbVersion185->setText(QCoreApplication::translate("MainWindow", "Loaded Firmware ...", nullptr));
        Update185->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        FtpUrlLable->setText(QCoreApplication::translate("MainWindow", "Firmware URL:", nullptr));
        firmwareUrl->setText(QCoreApplication::translate("MainWindow", "ftp://firmwareuser:orangepi@10.59.58.161/main", nullptr));
        DownloadFirmware->setText(QCoreApplication::translate("MainWindow", "Download Wfb Firmware", nullptr));
        sendFile->setText(QCoreApplication::translate("MainWindow", "Send File on Board", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
