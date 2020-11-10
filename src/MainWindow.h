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
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>

#include "binary_data_class.hpp"
#include "ui_MainWindow.h"

/**
 * @class MainWindow
 * @brief Qt main window
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

    /**
     * Reset the camera view as default
     */
    void slotResetCamera();

    /**
     * Allow the user to select an area to zoom on
     */
    void slotZoomArea();

    /**
     * Reset the interactor to allow object interaction after zoomArea
     */
    void slotInteractWithObject();

    /**
     * Select the result to visualise
     * @param res Result to visualise
     * @param choice Choice for setVTK method
     */
    void slotResult(Str_Result& res, const int& choice);

  private:
    /**
     * Path to directory that stores the objects to open
     */
    QString objectDirectory;

    /**
     * Movable widget that shows the axes
     */
    vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget;

    QStringListModel* model;

    /**
     * Data imported from the current .res file
     */
    Binary_data_class* binary = NULL;

    /**
     * Initialize axes view
     */
    void initAxes();

    /**
     * Load the VKT result and show it
     * @param res Result read by the binary data
     * @param choice
     */
    void setVTK(Str_Result& res, const int& choice);
};

#endif