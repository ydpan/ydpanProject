#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QSerialPort>
class QLabel;
namespace Ui {
class MainWindow;
}

class Console;
class SettingsDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
private:
    void initActionsConnections();
private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;
    QLabel *m_pStatus;
    Console *m_pConsole;
    SettingsDialog *m_pSettings;
    QSerialPort *m_pSerial;
};

#endif // MAINWINDOW_H
