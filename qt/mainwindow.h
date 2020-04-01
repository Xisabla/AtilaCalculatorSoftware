#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(char *c);
    ~MainWindow();
public slots:
    void slotExit();
};

#endif // MAINWINDOW_H
