/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      main_window.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MAIN_WINDOW_H
#define ATILACALCULATORSOFTWARE_MAIN_WINDOW_H

#include "binary_data_wrapper.h"
#include "ui_MainWindow.h"

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
    explicit MainWindow(char* dataDirectory);

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
     * @param result Result to visualise
     * @param component Choice for setVTK method
     */
    void slotResult(Result& result, const unsigned int& component);

  private:
    /**
     * Initialize axes view
     */
    void initAxes();

    /**
     * Load the VKT result and show it
     * @param result Result read by the binary data
     * @param component
     */
    void setVTK(Result& result, const int& component);

    /**
     * Read binary data and load it, then print the scalars and show the object, also set the
     * "Results" menu @see setBinaryResults
     * @param filename Path to the resource gzFile to read data from
     */
    void loadBinaryData(const std::string& filename);

    /**
     * @brief Clear and destruct loaded binary data
     */
    void unloadBinaryData();

    /**
     * @brief Add action item to "Results" menu for each component of each loaded results of binary
     * data
     */
    void setBinaryResults();

    /**
     * @brief Remove all actions of the "Results" menu
     */
    void clearBinaryResults();

    /**
     * Path to directory that stores the objects to open
     */
    QString objectDirectory;

    /**
     * Movable widget that shows the axes
     */
    vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget;

    /**
     * @brief Information module model
     */
    QStringListModel* model;

    /**
     * Data imported from the current .res file
     */
    BinaryDataWrapper* binary = nullptr;
};

#endif