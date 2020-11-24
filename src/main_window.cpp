/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      main_window.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "main_window.h"

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
    this->menuResults->setDisabled(true);
    this->actionZoomOnArea->setDisabled(true);
    this->actionResetCamera->setDisabled(true);
    this->actionInteractWithObject->setDisabled(true);

    // Disable undone actions
    this->actionExportToText->setDisabled(true);
}

//  --------------------------------------------------------------------------------------
//  MAIN WINDOW > ACTION SLOTS
//  --------------------------------------------------------------------------------------

void MainWindow::slotExit() {
    this->close();
    qApp->exit();
}

void MainWindow::slotOpenFile() {
    QString filename =
    QFileDialog::getOpenFileName(this, "Select a .res file", this->objectDirectory, "Res (*.res)");

    // Stop if no given file/action cancelled
    if (filename.isEmpty()) return;

    // Load the binary data and show the object and scalar bar
    this->loadBinaryData(filename.toStdString());
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

void MainWindow::slotResult(Result& result, const unsigned int& component) {
#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#endif
    this->setVTK(result, static_cast<int>(component));
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

void MainWindow::setVTK(Result& result, const int& component) {
    this->binary->loadResult(result, component);

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
    scalarBar->SetTitle(std::string(result.getAnalysis() + std::to_string(component)).c_str());
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

void MainWindow::loadBinaryData(std::string filename) {
    // Unload data if there is already some loaded
    if (this->binary != nullptr) this->unloadBinaryData();

    // Load binary data
    this->binary = new BinaryDataWrapper(filename);

    // Enable view actions
    this->actionZoomOnArea->setEnabled(true);
    this->actionResetCamera->setEnabled(true);
    this->actionInteractWithObject->setEnabled(true);

    // Add results menus
    this->setBinaryResults();

    // Set the view
    this->setVTK(this->binary->getResults().front(), 0);
}

void MainWindow::unloadBinaryData() {
    delete this->binary;

#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#endif

    this->clearBinaryResults();
}

void MainWindow::setBinaryResults() {

    // Set result actions
    std::vector<float> steps;
    std::map<float, QMenu*> stepMenus;

    // Add results menus
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

            if (!result.getComponents().empty()) {
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

    // Enable results menu
    this->menuResults->setEnabled(true);
}

void MainWindow::clearBinaryResults() { this->menuResults->clear(); }