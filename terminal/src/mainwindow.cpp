#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include "console.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setWindowIcon(QIcon(":/images/logo.png"));
	m_pConsole = new Console;
	m_pConsole->setEnabled(false);
    setCentralWidget(m_pConsole);

    m_pSerial = new QSerialPort(this);

    m_pSettings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    m_pStatus = new QLabel;
    ui->statusBar->addWidget(m_pStatus);

    initActionsConnections();

    connect(m_pSerial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(m_pSerial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(m_pConsole, &Console::getData, this, &MainWindow::writeData);
	
}

MainWindow::~MainWindow()
{
    delete m_pSettings;
	delete m_pConsole;
	delete m_pStatus;
    delete ui;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = m_pSettings->settings();
    m_pSerial->setPortName(p.name);
    m_pSerial->setBaudRate(p.baudRate);
    m_pSerial->setDataBits(p.dataBits);
    m_pSerial->setParity(p.parity);
    m_pSerial->setStopBits(p.stopBits);
    m_pSerial->setFlowControl(p.flowControl);
    if (m_pSerial->open(QIODevice::ReadWrite)) {
		m_pConsole->setEnabled(true);
		//m_pConsole->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), m_pSerial->errorString());

        showStatusMessage(tr("Open %1 error ").arg(p.name));
    }
}

void MainWindow::closeSerialPort()
{
    if (m_pSerial->isOpen())
        m_pSerial->close();
	m_pConsole->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::about()
{
//     QMessageBox::about(this, tr("About Simple Terminal"),
//                        tr("The <b>Simple Terminal</b> example demonstrates how to "
//                           "use the Qt Serial Port module in modern GUI applications "
//                           "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::writeData(const QByteArray &data)
{
    m_pSerial->write(data);
}

void MainWindow::readData()
{
    QByteArray data = m_pSerial->readAll();
	m_pConsole->putData(data);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_pSerial->errorString());
        closeSerialPort();
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, m_pSettings, &SettingsDialog::show);
    connect(ui->actionClear, &QAction::triggered, m_pConsole, &Console::clear);
    //connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    //connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_pStatus->setText(message);
}
