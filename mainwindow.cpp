#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTimer>
#include <QScreen>
#include <QProcess>
#include <QMessageBox>

// Inicializace okna

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    // Zadání rozměrů okna, vycentrování na střed obrazovky a nastavení barvy pozadí

    ui->setupUi(this);
    setFixedSize(1172, 596);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
       QScreen *screen = QGuiApplication::primaryScreen(); // Použití QScreen
       QRect screenGeometry = screen->geometry();
       int x = (screenGeometry.width() - width()) / 2;
       int y = (screenGeometry.height() - height()) / 2;
       move(x, y);

    this->setStyleSheet("background-color: #004884; color: white;");

   // Vypnutí gradientu a nastavení barev pro tlačítka

       ui->rebootButton->setStyleSheet("background-color: #b50000; color: white;");
       ui->shutdownButton->setStyleSheet("background-color: #b50000; color: white;");
       ui->terminalRootButton->setStyleSheet("background-color: #b50000; color: white;");
       ui->spustitButton->setStyleSheet("background-color: #fb4400; color: white;");
       ui->infoproURL->setStyleSheet("background-color: #ffffff; color: black;");
       ui->terminalbutton->setStyleSheet("background-color: #3d3d3d; color:white");

   // Časovač pro obnovování stavu síťových rozhraní

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateIPAddress);
    timer->start(2000); // Aktualizovat každé 2s

    connect(ui->nastavitButton, &QPushButton::clicked, this, &MainWindow::loadUrl);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// // // // // // // // // //

// Tlačítka //

void MainWindow::on_rebootButton_clicked()
{
    QProcess::startDetached("systemctl", QStringList() << "reboot");
 }


void MainWindow::on_shutdownButton_clicked()
{
    QProcess::startDetached("systemctl", QStringList() << "poweroff");
 }


void MainWindow::on_spustitButton_clicked()
{
    QProcess::startDetached("killall", QStringList() << "chromium");

    QString autorunPath = "/home/infopro/infopro/infopro_autorun";
    QFileInfo autorunFile(autorunPath);

    if (autorunFile.exists() && autorunFile.isFile()) {
        QProcess::startDetached("bash", QStringList() << autorunPath);
        this->close();
    } else {
        QMessageBox::critical(this, "Chyba", "Není zadána žádná adresa");
    }
}

void MainWindow::on_sitButton_clicked()
{
    QProcess::startDetached("nm-connection-editor");
}


void MainWindow::on_displejButton_clicked()
{
   QProcess::startDetached("xfce4-display-settings");
}


void MainWindow::on_zvukButton_clicked()
{
  QProcess::startDetached("pavucontrol");
}

// // // // // // // // // // // // // // // //

// Tlačítka servis //

void MainWindow::on_infoproAutorunButton_clicked()
{
QProcess::startDetached("mousepad", QStringList() << "/home/infopro/infopro/infopro_autorun");
}


void MainWindow::on_hostsButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "sudo nano /etc/hosts");
}


void MainWindow::on_hostnameButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "sudo nano /etc/hostname");
}


void MainWindow::on_crontabButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "sudo nano /etc/crontab");
}


void MainWindow::on_terminalbutton_clicked()
{
QProcess::startDetached("xfce4-terminal");
}


void MainWindow::on_terminalRootButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "sudo su");
}


void MainWindow::on_htopButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "htop");
}


void MainWindow::on_sensorsButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "watch sensors");
}

void MainWindow::on_systemctlButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "systemctl status x11vnc sshd");
}


void MainWindow::on_firewallButton_clicked()
{
QProcess::startDetached("xfce4-terminal", QStringList() << "--command" << "sudo firewall-config");
}


// // // // // // // // // // // // // // // //

// Zobrazení síťových rozhraní //

void MainWindow::displayIPAddress()
{
    QString ipAddress;

    // Získání seznamu síťových rozhraní
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        // Vyfiltruj pouze aktivní a IPv4 rozhraní
        if (interface.isValid() && interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
            !interface.hardwareAddress().isEmpty()) {

            QString interfaceType;
            // Přepis názvů rozhraní
            if (interface.name().startsWith("w") || interface.name().startsWith("e")) {
                interfaceType = interface.name().startsWith("w") ? "WiFi" : "Ethernet";
            } else {
                interfaceType = interface.name();
            }

            // Získání seznamu IP adres pro každé rozhraní
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipAddress += interfaceType + "\n";
                    ipAddress += entry.ip().toString() + "\n";
                    ipAddress += interface.hardwareAddress() + "\n\n";
                }
            }
        }
    }
    ui->ipShow->setText(ipAddress);
}

void MainWindow::updateIPAddress()
{
    QString ipAddress;

    // Získání seznamu síťových rozhraní
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        // Vyfiltruj pouze aktivní a IPv4 rozhraní
        if (interface.isValid() && interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
            !interface.hardwareAddress().isEmpty()) {

            QString interfaceType;
            // Přepis názvů rozhraní
            if (interface.name().startsWith("w") || interface.name().startsWith("e")) {
                interfaceType = interface.name().startsWith("w") ? "WiFi" : "Ethernet";
            } else {
                interfaceType = interface.name();
            }

            // Získání seznamu IP adres pro každé rozhraní
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipAddress += interfaceType + "\n";
                    ipAddress += entry.ip().toString() + "\n";
                    ipAddress += interface.hardwareAddress() + "\n\n";
                }
            }
        }
    }
    ui->ipShow->setText(ipAddress);
}

// // // // // // // // // // // // // // // // // // //

//Nastavení a náhled URL pro InfoPro

void MainWindow::loadUrl()
{
    QString url = ui->infoproURL->text();
        QString command = "chromium --kiosk " + url;

    // Otevření souboru pro zápis
    QFile file("/home/infopro/infopro/infopro_autorun");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) { // Přidání QIODevice::Truncate smaže obsah souboru, pokud existuje
        // Zápis příkazu do souboru
        QTextStream out(&file);
        out << command;
        file.close();
        QProcess::execute("chmod", QStringList() << "+x" << "/home/infopro/infopro/infopro_autorun");
    } else {
        qDebug() << "Nepodařilo se otevřít soubor pro zápis.";
    }
    ui->infoproNahled->setUrl(QUrl(url)); // Použijte setUrl pro nastavení URL adresy pro WebEngineView
}

