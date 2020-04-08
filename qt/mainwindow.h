#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <tuple>
#include <QMainWindow>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <QStringList>
#include <QStringListModel>
#include <vtkOrientationMarkerWidget.h>
#include <vtkNamedColors.h>

#include "ui_mainwindow.h"
#include "binary_data_class.hpp"


class MainWindow : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT
public:
    Binary_data_class *binary;
    MainWindow(char *c);
    ~MainWindow();
public slots:
    void slotExit();
    void slotOpenFile();
private : 
    vtkSmartPointer<vtkOrientationMarkerWidget> widget;
    vtkSmartPointer<vtkFloatArray> scalars;
    QStringListModel *model;
    void setScalars(const int &nb_points);
    void setAxes(vtkSmartPointer<vtkNamedColors> &colors);
};

#endif // MAINWINDOW_H
