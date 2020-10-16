/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      MainWindow.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "MainWindow.h"

#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif

//  --------------------------------------------------------------------------------------
MainWindow::MainWindow(char* dataDirectory) {

    this->objectDirectory = QString::fromUtf8(dataDirectory);

    this->setupUi(this);

    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
#if VTK890
    this->qvtkWidget->setRenderWindow(renderWindow);
#else
    this->qvtkWidget->SetRenderWindow(renderWindow);
#endif

    // VTK Renderer
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    // Connect VTK with Qt
#if VTK890
    this->qvtkWidget->renderWindow()->AddRenderer(renderer);
    this->qvtkWidget->renderWindow()->SetWindowName("AtilaSoftwareCalculator");
#else
    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    this->qvtkWidget->GetRenderWindow()->SetWindowName("AtilaSoftwareCalculator");
#endif

    this->initAxes();
    renderer->ResetCamera();

    // Actions
    // TODO - Bind MenuBar actions
}

//  --------------------------------------------------------------------------------------
void MainWindow::initAxes() {
    vtkNew<vtkNamedColors> colors;

    double rgba[4] = { .0, .0, .0, .0 };
    colors->GetColor("rgb", rgba);

    vtkNew<vtkAxesActor> axes;
    vtkNew<vtkOrientationMarkerWidget> widget;

    widget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    widget->SetOrientationMarker(axes);
#if VTK890
    widget->SetInteractor(this->qvtkWidget->interactor());
#else
    widget->SetInteractor(this->qvtkWidget->GetInteractor());
#endif
    widget->SetViewport(.0, .0, .2, .2);
    widget->SetEnabled(1);

    this->widget = widget;
}