/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      MainWindow.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MAINWINDOW_H
#define ATILACALCULATORSOFTWARE_MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QStringListModel>
#include <vtkAxesActor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>

#include "ui_MainWindow.h"

/**
 * @class   MainWindow
 * @brief   Qt main window
 */
class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
  public:
    /**
     * @param dataDirectory The path to the objects directory
     */
    MainWindow(char* dataDirectory);
  public slots:
    /**
     * Stop the application
     */
    void slotExit();

    /**
     * Open and read object file, then render it
     */
    void slotOpenFile();

  private:
    /**
     * Path to directory that stores the objects to open
     */
    QString objectDirectory;

    /**
     * Movable widget that shows the axes
     */
    vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget;

    // QStringListModel* model;

    /**
     * Initialize axes view
     */
    void initAxes();
    // void setVTK(Str_Result &res, const in& choice);
};

#endif