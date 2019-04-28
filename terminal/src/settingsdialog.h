#ifndef _SETTINGSDIALOG_H_
#define _SETTINGSDIALOG_H_

#include <QDialog>
#include <QSerialPort>
#include <QValidator>
#include <QWidget>

namespace Ui {
class SettingsDialog;
}

class QIntValidator;
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    virtual ~SettingsDialog();
    Settings settings() const;
	bool setSettings(SettingsDialog::Settings setVal);
private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);
private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

	void updateConfig();
	void readComConfig();
	void writeComConfig();
private:
    Ui::SettingsDialog *ui;
    Settings currentSettings;
    QIntValidator *intValidator;
};

#endif // SETTINGSDIALOG_H
