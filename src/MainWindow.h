/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      MainWindow.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef BorderWidgetQt_H
#define BorderWidgetQt_H

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

  private:
    QString objectDirectory;
    vtkSmartPointer<vtkOrientationMarkerWidget> widget;
    QStringListModel* model;

    /**
     * Initialize axes view
     */
    void initAxes();
    // void setVTK(Str_Result &res, const in& choice);
};

#endif