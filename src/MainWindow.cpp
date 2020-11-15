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
//  MAIN WINDOW > PUBLIC METHODS
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

    this->setWindowTitle(QString::fromStdString("AtilaSoftwareCalculator"));

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
//  MAIN WINDOW > ACTION SLOTS
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

    if (this->binary != NULL) {
        // TODO: Move this in a private method "unloadBinaryData"
        // TODO: Clean results menu (maybe do this in another method "cleanBinaryResults")
        delete this->binary;
#if VTK890
        this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
        this->qvtkWidget->GetRenderWindow()
        ->GetRenderers()
        ->GetFirstRenderer()
        ->RemoveAllViewProps();
#endif
    }

    // TODO: Move this in a private method "loadBinaryData"
    // Load binary data
    this->binary = new BinaryDataWrapper(filename.toStdString());

    // Enable view actions
    this->actionZoomOnArea->setEnabled(true);
    this->actionResetCamera->setEnabled(true);
    this->actionInteractWithObject->setEnabled(true);

    // Set result actions
    std::vector<float> steps;
    std::map<float, QMenu*> stepMenus;

    // Add results menus
    // TODO: Maybe move this in another method "addBinaryResults"
    for (auto&& result: this->binary->getResults()) {
        if (!std::binary_search(steps.begin(), steps.end(), result.getStep())) {
            steps.push_back(result.getStep());
            QMenu* stepMenuItem = this->menuResults->addMenu(QString::number(result.getStep()));
            stepMenus.insert(std::pair<float, QMenu*>(result.getStep(), stepMenuItem));
        }

        QMenu* menu =
        stepMenus[result.getStep()]->addMenu(QString::fromStdString(result.getAnalysis()));

        // Add results menu action
        for (unsigned int i = 0; i < result.getComponentCount(); i++) {
            QAction* resultItemAction;

            if (result.getComponents().size() > 0) {
                resultItemAction =
                menu->addAction(QString::fromStdString(result.getComponents().at(i)));
            } else {
                resultItemAction = menu->addAction(QString::number(i));
            }

            connect(resultItemAction, &QAction::triggered, [this, &result, i]() {
                this->slotResult(result, i);
            });
        }
    }

    // Set the view
    this->setVTK(this->binary->getResults().front(), 0);
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

void MainWindow::slotResult(Result& res, const int& choice) {
#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#endif
    this->setVTK(res, choice);
}

//  --------------------------------------------------------------------------------------
//  MAIN WINDOW > PRIVATE METHODS
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

void MainWindow::setVTK(Result& res, const int& choice) {
    this->binary->loadResult(res, choice);

    // Update information list
    this->model->setStringList(this->binary->getInformationList());
    this->listView->setModel(this->model);
    this->listView->adjustSize();
    this->listView->setVisible(true);

    double range[2] = { this->binary->getScalars()->GetRange()[0],
                        this->binary->getScalars()->GetRange()[1] };

    // Lookup table
    vtkNew<vtkLookupTable> lookupTable;
    lookupTable->SetNumberOfTableValues(this->binary->getScalars()->GetNumberOfTuples() + 1);
    lookupTable->SetTableRange(range);
    lookupTable->Build();

    this->binary->getUnstructuredGrid()->GetPointData()->SetScalars(this->binary->getScalars());

    // Mapper
    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(this->binary->getUnstructuredGrid());
    mapper->SetScalarRange(range);
    mapper->SetLookupTable(lookupTable);

    // Side scalar bar
    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle(std::string(res.getAnalysis() + std::to_string(choice)).c_str());
    scalarBar->UnconstrainedFontSizeOn();
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetBarRatio(scalarBar->GetBarRatio() / 2.0);

    // Poly actor (3D Object)
    vtkNew<vtkActor> polyActor;
    polyActor->SetMapper(mapper);

    // Add actors and reset camera
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