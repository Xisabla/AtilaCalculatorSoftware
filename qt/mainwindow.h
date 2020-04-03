#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <tuple>
#include <QMainWindow>
#include <QString>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <QStringList>
#include <QStringListModel>

#include "ui_mainwindow.h"
#include "binary_data_GiD.hpp"


class MainWindow : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow(char *c);
    ~MainWindow();
public slots:
    void slotExit();
private : 
    vtkSmartPointer<vtkFloatArray> scalars;
    QStringListModel *model;
    std::tuple< vtkSmartPointer<vtkPoints> , vtkSmartPointer<vtkCellArray> ,const int > createTrianglesFromMeshCoordinates(Str_binary_data_GiD &binary_data);
    const void setScalars(const int &nb_points);
};

#endif // MAINWINDOW_H
