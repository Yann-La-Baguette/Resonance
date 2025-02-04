#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qrcodegen.hpp"
#include "udpsender.h"

#include <iostream>
#include <QImage>
#include <QPainter>
#include <QNetworkInterface>


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void generateQRCode(const QString name, const QString &text);
    QMap<QString, QString> getIPAddresses();

private slots:
    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;

    UDPSender sender;
};
#endif // MAINWINDOW_H
