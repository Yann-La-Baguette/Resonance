#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMap<QString, QString> ips = getIPAddresses();

    // Récupération des périphériques audio d'entrée
    const QList<QAudioDevice> audioDevices = QMediaDevices::audioInputs();
    for (const QAudioDevice &device : audioDevices) {
        ui->comboBoxAppareil->addItem(device.description(), QVariant::fromValue(device));
    }

    on_comboBoxAppareil_activated(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMap<QString, QString> MainWindow::getIPAddresses() {
    QMap<QString, QString> ipAddresses;

    // Liste des interfaces réseau
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &interface : interfaces) {
        // Vérifie si l'interface est active et non une loopback
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            // Vérifie si l'interface est WiFi ou Ethernet
            QString interfaceType;
            if (interface.name().contains("wlan", Qt::CaseInsensitive) ||
                interface.humanReadableName().contains("Wi-Fi", Qt::CaseInsensitive)) {
                interfaceType = "WiFi";
            } else if (interface.name().contains("eth", Qt::CaseInsensitive) ||
                       interface.humanReadableName().contains("Ethernet", Qt::CaseInsensitive)) {
                interfaceType = "Ethernet";
            } else {
                continue; // Ignore les autres interfaces
            }

            // Récupère les adresses IP de l'interface
            for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipAddresses[interfaceType] = entry.ip().toString();
                }
            }
        }
    }

    return ipAddresses;
}


void MainWindow::on_comboBoxAppareil_activated(int index)
{
    if (index >= 0) {
        QAudioDevice selectedDevice = ui->comboBoxAppareil->currentData().value<QAudioDevice>();
        qDebug() << "Périphérique sélectionné :" << selectedDevice.description();
        // Vous pouvez maintenant utiliser `selectedDevice` pour configurer l'entrée audio
        AudioDeviceManager::instance().setAudioDevice(ui->comboBoxAppareil->currentData().value<QAudioDevice>());

        sender.updateAudioDevice();
    }
}


void MainWindow::on_IPAdressLineEdit_editingFinished() {
    QString ipAddress = ui->IPAdressLineEdit->text(); // Récupère le texte du QLineEdit

    // Valider l'adresse IP
    QHostAddress address;
    if (address.setAddress(ipAddress)) {
        qDebug() << "Adresse IP valide : " << ipAddress;

        // Mettre à jour l'adresse IP dans l'objet Sender
        sender.updateIpAddress(ipAddress);
    } else {
        qWarning() << "Adresse IP invalide : " << ipAddress;
    }
}

// MainWindow.cpp
void MainWindow::on_startButton_clicked()
{
    QString ipAddress = ui->IPAdressLineEdit->text();
    QHostAddress address;
    if (!address.setAddress(ipAddress)) {
        qWarning() << "Adresse IP invalide : " << ipAddress;
        // Affiche un message d’erreur à l’utilisateur si besoin
        QMessageBox::warning(this, "Erreur", "Adresse IP invalide !");
        return;
    }

    if (!isSending) {
        // Démarrer l’envoi audio
        sender.startSending();
        isSending = true;
        ui->startButton->setText("Arrêter l'envoi");
        qDebug() << "Envoi audio démarré";
    } else {
        // Arrêter l’envoi audio
        sender.stopSending();
        isSending = false;
        ui->startButton->setText("Démarrer l'envoi");
        qDebug() << "Envoi audio arrêté";
    }
}

