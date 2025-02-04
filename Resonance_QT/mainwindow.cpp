#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMap<QString, QString> ips = getIPAddresses();

    if (!ips.isEmpty()) {
        for (const QString &key : ips.keys()) {
            qDebug() << key << "IP:" << ips[key];
            generateQRCode(key, ips[key]);
        }
    } else {
        qDebug() << "Aucune adresse IP trouvée.";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateQRCode(QString name, const QString &text) {
    // Générer le QR code avec la bibliothèque
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);

    // Déterminer la taille du QR code en pixels
    int size = qr.getSize();
    int scale = 300 / size; // On adapte la taille pour que l'image fasse 500x500 pixels

    // Créer une image à partir du QR code avec une taille plus grande
    QImage image(300, 300, QImage::Format_RGB888);
    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);  // Fond blanc
    painter.setPen(Qt::NoPen);  // Pas de bordure autour des modules
    painter.setBrush(Qt::black);  // Couleur des modules (noir)

    // Dessiner les modules du QR code en fonction de la taille ajustée
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (qr.getModule(x, y)) {
                // Dessiner un carré pour chaque module, avec un facteur de mise à l'échelle
                painter.drawRect(x * scale, y * scale, scale, scale);
            }
        }
    }

    name = name + ".png";
    // Sauvegarder l'image ou l'afficher
    image.save(name);
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

void MainWindow::on_comboBox_activated(int index)
{
    QPixmap pixmapWiFi("./WiFi.png");
    QPixmap pixmapEthernet("./Ethernet.png");
    switch (index) {
    case 0: // Ethernet
        ui->QRCodeLabel->setPixmap(pixmapEthernet);
        break;

    case 1: // Wi-Fi
        ui->QRCodeLabel->setPixmap(pixmapWiFi);
        break;

    default:
        break;
    }
}

