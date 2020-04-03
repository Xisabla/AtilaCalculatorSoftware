#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <tuple>
#include <QMainWindow>
#include <QString>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>

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
    std::tuple< vtkSmartPointer<vtkPoints> , vtkSmartPointer<vtkCellArray> ,const int > createTrianglesFromMeshCoordinates(Str_binary_data_GiD &binary_data);
};

#endif // MAINWINDOW_H
