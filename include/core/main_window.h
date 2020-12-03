/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      main_window.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_MAIN_WINDOW_H
#define ATILACALCULATORSOFTWARE_MAIN_WINDOW_H

#include "core/binary_data_wrapper.h"
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
#include <vtkVertexGlyphFilter.h>

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
     * @brief Stop the application
     */
    void slotExit();

    /**
     * @brief Open and read object file, then render it
     */
    void slotOpenFile();

    /**
     * @brief Allow the user to select an area to zoom on
     */
    void slotZoomArea();

    /**
     * @brief Reset the interactor to allow object interaction after zoomArea
     */
    void slotInteractWithObject();

    /**
     * @brief Shows the objects node if toggled on checked, otherwise hide it
     */
    void slotShowNodes();

    /**
     * @brief Reset the camera view as default
     */
    void slotResetCamera();

    /**
     * @brief Select the result to visualise
     * @param result Result to visualise
     * @param component Choice for showResult method
     */
    void slotResult(Result& result, const unsigned int& component);

  private:
    /**
     * @brief Initialize axes view
     */
    void initAxes();

    /**
     * @brief Load result and show elements
     * @param result Result read by the binary data
     * @param component Component to show
     */
    void showResult(Result& result, const int& component);

    /**
     * @brief Show the 3D polygon of the given result
     * @param result Result read by the binary data
     * @param component Component to show
     */
    void show3DPoly(Result& result, const int& component);

    /**
     * @brief Show the nodes of the result
     * @param result Result read by the binary data
     * @param component Component from which nodes will be read
     */
    void showNodes();

    /**
     * @brief Read binary data and load it, then print the scalars and show the object, also set the
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
     * @brief Path to directory that stores the objects to open
     */
    QString objectDirectory;

    /**
     * @brief Movable widget that shows the axes
     */
    vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget;

    /**
     * @brief Information module model
     */
    QStringListModel* informationListModel;

    /**
     * @brief Elements list model
     */
    QStringListModel* elementsListModel;

    /**
     * @brief Actor of node showing
     */
    vtkProp* nodeActor = nullptr;

    /**
     * @brief Data imported from the current .res file
     */
    BinaryDataWrapper* binary = nullptr;
};

#endif
