#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadUrl();
    void setNetworkInterfaceUp(const QString &interfaceName);
    void setNetworkInterfaceDown(const QString &interfaceName);

private slots:
    void displayIPAddress();
    void updateIPAddress();


    void on_sitButton_clicked();
    void on_displejButton_clicked();
    void on_zvukButton_clicked();

    void on_rebootButton_clicked();
    void on_shutdownButton_clicked();

    void on_spustitButton_clicked();
    void on_infoproAutorunButton_clicked();

    void on_hostsButton_clicked();

    void on_hostnameButton_clicked();

    void on_crontabButton_clicked();

    void on_terminalbutton_clicked();

    void on_terminalRootButton_clicked();

    void on_htopButton_clicked();

    void on_sensorsButton_clicked();

    void on_systemctlButton_clicked();

    void on_firewallButton_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *process;
};

#endif // MAINWINDOW_H
