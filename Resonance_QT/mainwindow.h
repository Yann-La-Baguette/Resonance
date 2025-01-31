#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qrcodegen.hpp"

#include <iostream>
#include <QImage>
#include <QPainter>


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

    void generateQRCode(const QString &text);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
