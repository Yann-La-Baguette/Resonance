#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    generateQRCode("https://www.youtube.com/watch?v=dQw4w9WgXcQ&pp=ygUXbmV2ZXIgZ29ubmEgZ2l2ZSB5b3UgdXA%3D");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateQRCode(const QString &text) {
    // Générer le QR code avec la bibliothèque
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);

    // Déterminer la taille du QR code en pixels
    int size = qr.getSize();
    int scale = 500 / size; // On adapte la taille pour que l'image fasse 500x500 pixels

    // Créer une image à partir du QR code avec une taille plus grande
    QImage image(500, 500, QImage::Format_RGB888);
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

    // Sauvegarder l'image ou l'afficher
    image.save("qrcode_high_res.png");
}
