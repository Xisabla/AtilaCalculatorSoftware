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
//  PUBLIC METHODS
//  --------------------------------------------------------------------------------------

MainWindow::MainWindow(char* dataDirectory) {

    this->objectDirectory = QString::fromUtf8(dataDirectory);

    this->setupUi(this);
    this->model = new QStringListModel(this);

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

    this->listView->setVisible(false);
    this->initAxes();
    renderer->ResetCamera();

    // Actions
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->actionZoomOnArea, SIGNAL(triggered()), this, SLOT(slotZoomArea()));
    connect(this->actionResetCamera, SIGNAL(triggered()), this, SLOT(slotResetCamera()));
    connect(
    this->actionInteractWithObject, SIGNAL(triggered()), this, SLOT(slotInteractWithObject()));

    // Disable view action by default
    this->actionZoomOnArea->setDisabled(true);
    this->actionResetCamera->setDisabled(true);
    this->actionInteractWithObject->setDisabled(true);
}

//  --------------------------------------------------------------------------------------
//  ACTION SLOTS
//  --------------------------------------------------------------------------------------

void MainWindow::slotExit() {
    this->close();
    qApp->exit(0);
}

void MainWindow::slotOpenFile() {
    QString filename =
    QFileDialog::getOpenFileName(this, "Select a .res file", this->objectDirectory, "Res (*.res)");

    // Stop if no given file/action cancelled
    if (filename.isEmpty()) return;

    if (this->binary == NULL) {
        // Load binary data
        this->binary = new Binary_data_class(filename.toStdString());

        // Enable view actions
        this->actionZoomOnArea->setEnabled(true);
        this->actionResetCamera->setEnabled(true);
        this->actionInteractWithObject->setEnabled(true);

        // Set result actions
        std::vector<float> steps;
        std::map<float, QMenu*> stepMenus;

        for (auto&& res: this->binary->results_) {
            if (!std::binary_search(steps.begin(), steps.end(), res.step_)) {
                steps.push_back(res.step_);
                QMenu* stepMenuItem = this->menuResults->addMenu(QString::number(res.step_));
                stepMenus.insert(std::pair<float, QMenu*>(res.step_, stepMenuItem));
            }

            QMenu* menu = stepMenus[res.step_]->addMenu(QString::fromStdString(res.analysis_));

            for (int i = 0; i < res.result_size_; i++) {
                QAction* resultItemAction;

                if (res.component_names_.size() > 0) {
                    resultItemAction =
                    menu->addAction(QString::fromStdString(res.component_names_.at(i)));
                } else {
                    resultItemAction = menu->addAction(QString::number(i));
                }

                connect(resultItemAction, &QAction::triggered, [this, &res, i]() {
                    this->slotResult(res, i);
                });
            }
        }

        // Set the view
        this->setVTK(this->binary->results_.front(), 0);
    } else {
        // NOTE: Crashes when trying to load twice another .res file
        delete this->binary;
        this->binary = NULL;
#if VTK890
        this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
        this->qvtkWidget->GetRenderWindow()
        ->GetRenderers()
        ->GetFirstRenderer()
        ->RemoveAllViewProps();
#endif
    }
}

void MainWindow::slotResetCamera() {
#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
#endif
}

void MainWindow::slotZoomArea() {
    vtkNew<vtkInteractorStyleRubberBandZoom> style;
#if VTK890
    this->qvtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(style);
#else
    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
#endif
}

void MainWindow::slotInteractWithObject() {
    vtkNew<vtkInteractorStyleTrackballCamera> style;
#if VTK890
    this->qvtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(style);
#else
    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
#endif
}

void MainWindow::slotResult(Str_Result& res, const int& choice) {
#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#endif
    this->setVTK(res, choice);
}

//  --------------------------------------------------------------------------------------
//  PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void MainWindow::initAxes() {
    vtkNew<vtkNamedColors> colors;

    double rgba[4] = { .0, .0, .0, .0 };
    colors->GetColor("rgb", rgba);

    vtkNew<vtkAxesActor> axes;
    vtkNew<vtkOrientationMarkerWidget> axesWidget;

    axesWidget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    axesWidget->SetOrientationMarker(axes);
#if VTK890
    axesWidget->SetInteractor(this->qvtkWidget->interactor());
#else
    axesWidget->SetInteractor(this->qvtkWidget->GetInteractor());
#endif
    axesWidget->SetViewport(.0, .0, .2, .2);
    axesWidget->SetEnabled(1);

    this->axesWidget = axesWidget;
}

void MainWindow::setVTK(Str_Result& res, const int& choice) {
    this->binary->setScalarFromQT2(res, choice);

    this->model->setStringList(this->binary->getstrList());
    this->listView->setModel(this->model);
    this->listView->adjustSize();
    this->listView->setVisible(true);

    double range[2] = { this->binary->getScalars()->GetRange()[0],
                        this->binary->getScalars()->GetRange()[1] };

    vtkNew<vtkLookupTable> lut;

    lut->SetNumberOfTableValues(this->binary->getScalars()->GetNumberOfTuples() + 1);
    lut->SetTableRange(range);
    lut->Build();

    this->binary->getUGrid()->GetPointData()->SetScalars(this->binary->getScalars());
    vtkNew<vtkDataSetMapper> mapper;

    mapper->SetInputData(this->binary->getUGrid());
    mapper->SetScalarRange(range);
    mapper->SetLookupTable(lut);

    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle(string(res.analysis_ + std::to_string(choice)).c_str());
    scalarBar->UnconstrainedFontSizeOn();
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetBarRatio(scalarBar->GetBarRatio() / 2.0);

    vtkNew<vtkActor> polyActor;
    polyActor->SetMapper(mapper);

#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(polyActor);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(scalarBar);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(polyActor);
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(scalarBar);
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
#endif
}