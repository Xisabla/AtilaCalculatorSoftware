#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
const void MainWindow::setTextinTextZone(const QString& a ){
    ui->textEdit->setText(a);
}
MainWindow::~MainWindow()
{
    delete ui;
}
