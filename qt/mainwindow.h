#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow , private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    const void setTextinTextZone(const QString& a );
    ~MainWindow();
public slots:
    void slotExit();
};

#endif // MAINWINDOW_H
