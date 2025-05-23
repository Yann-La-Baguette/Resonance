#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qrcodegen.hpp"
#include "udpsender.h"
#include "AudioDeviceManager.h"

#include <iostream>
#include <QImage>
#include <QPainter>
#include <QNetworkInterface>

#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QMediaDevices>

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

    QMap<QString, QString> getIPAddresses();

private slots:
    void on_comboBoxAppareil_activated(int index);

    void on_IPAdressLineEdit_editingFinished();

    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;

    UDPSender sender;
    bool isSending = false;
};
#endif // MAINWINDOW_H
