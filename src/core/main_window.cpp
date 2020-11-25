/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      main_window.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/main_window.h"

#include "logger/logger.h"

#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif

//  --------------------------------------------------------------------------------------
//  MAIN WINDOW > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

MainWindow::MainWindow(char* dataDirectory) {
    Logger::info("Initializing MainWindow...");
    this->objectDirectory = QString::fromUtf8(dataDirectory);
    Logger::debug("Using default data directory: ",
                  dataDirectory == nullptr ? "./" : dataDirectory);

    this->setupUi(this);
    this->model = new QStringListModel(this);

    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    Logger::debug("Setting OpenGL Render Window");
#if VTK890
    this->qvtkWidget->setRenderWindow(renderWindow);
#else
    this->qvtkWidget->SetRenderWindow(renderWindow);
#endif

    // VTK Renderer
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    // Connect VTK with Qt
    Logger::debug("Setting MainWindow Render");
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
    Logger::debug("Connecting view action");
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->actionZoomOnArea, SIGNAL(triggered()), this, SLOT(slotZoomArea()));
    connect(this->actionResetCamera, SIGNAL(triggered()), this, SLOT(slotResetCamera()));
    connect(
    this->actionInteractWithObject, SIGNAL(triggered()), this, SLOT(slotInteractWithObject()));

    // Disable view action by default
    Logger::debug("Disabling unreachable view action");
    this->menuResults->setDisabled(true);
    this->actionZoomOnArea->setDisabled(true);
    this->actionResetCamera->setDisabled(true);
    this->actionInteractWithObject->setDisabled(true);

    // Disable undone actions
    Logger::debug("Disabling undone features actions");
    this->actionExportToText->setDisabled(true);
    Logger::info("Initializing MainWindow: Done");
}

//  --------------------------------------------------------------------------------------
//  MAIN WINDOW > ACTION SLOTS
//  --------------------------------------------------------------------------------------

void MainWindow::slotExit() {
    Logger::info("Exiting...");
    this->close();
    qApp->exit();
}

void MainWindow::slotOpenFile() {
    QString filename =
    QFileDialog::getOpenFileName(this, "Select a .res file", this->objectDirectory, "Res (*.res)");

    if (!filename.isEmpty()) {
        Logger::info("Opening resource file: ", filename.toStdString());

        // Load the binary data and show the object and scalar bar
        this->loadBinaryData(filename.toStdString());

        Logger::info("Resource file loaded.");
    } else {
        Logger::warn("Invalid or empty filename: ", filename.toStdString());
    }
}

void MainWindow::slotResetCamera() {
    Logger::info("Reset Camera");
#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
#endif
}

void MainWindow::slotZoomArea() {
    Logger::info("Reset Interactor to RubberZoom");
    vtkNew<vtkInteractorStyleRubberBandZoom> style;
#if VTK890
    this->qvtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(style);
#else
    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
#endif
}

void MainWindow::slotInteractWithObject() {
    Logger::info("Reset Interactor to Camera");
    vtkNew<vtkInteractorStyleTrackballCamera> style;
#if VTK890
    this->qvtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(style);
#else
    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
#endif
}

void MainWindow::slotResult(Result& result, const unsigned int& component) {
    Logger::info("Change result to ", result.getAnalysis(), ":", component);
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
    Logger::debug("Initializing axes module...");
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
    Logger::debug("Initializing axes module: Done");
}

void MainWindow::setVTK(Result& result, const int& component) {
    this->binary->loadResult(result, component);

    // Update information list
    Logger::trace("Initialize Information StringList");
    this->model->setStringList(this->binary->getInformationList());
    this->listView->setModel(this->model);
    this->listView->adjustSize();
    this->listView->setVisible(true);

    double range[2] = { this->binary->getScalars()->GetRange()[0],
                        this->binary->getScalars()->GetRange()[1] };

    // Lookup table
    Logger::debug("Instantiate VTK LookupTable");
    vtkNew<vtkLookupTable> lookupTable;
    lookupTable->SetNumberOfTableValues(this->binary->getScalars()->GetNumberOfTuples() + 1);
    lookupTable->SetTableRange(range);
    lookupTable->Build();

    Logger::debug("Set Result scalars");
    this->binary->getUnstructuredGrid()->GetPointData()->SetScalars(this->binary->getScalars());

    // Mapper
    Logger::debug("Instantiate VTK DataSetMapper");
    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(this->binary->getUnstructuredGrid());
    mapper->SetScalarRange(range);
    mapper->SetLookupTable(lookupTable);

    // Side scalar bar
    Logger::debug("Instantiate VTK ScalarBarActor");
    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle(std::string(result.getAnalysis() + std::to_string(component)).c_str());
    scalarBar->UnconstrainedFontSizeOn();
    scalarBar->SetNumberOfLabels(5);
    Logger::info("Bar ratio is ", scalarBar->GetBarRatio() / 2.0);
    scalarBar->SetBarRatio(scalarBar->GetBarRatio() / 2.0);

    // Poly actor (3D Object)
    vtkNew<vtkActor> polyActor;
    polyActor->SetMapper(mapper);

    // Add actors and reset camera
    Logger::debug("Add Renderer camera actors");
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

void MainWindow::loadBinaryData(const std::string& filename) {
    // Unload data if there is already some loaded
    if (this->binary != nullptr) this->unloadBinaryData();

    // Load binary data
    this->binary = new BinaryDataWrapper(filename);

    // Enable view actions
    Logger::info("Enable view menu actions");
    this->actionZoomOnArea->setEnabled(true);
    this->actionResetCamera->setEnabled(true);
    this->actionInteractWithObject->setEnabled(true);

    // Add results menus
    this->setBinaryResults();

    // Set the view
    this->setVTK(this->binary->getResults().front(), 0);
}

void MainWindow::unloadBinaryData() {
    Logger::debug("Unloading loaded data...");
    Logger::trace("Free binary");
    delete this->binary;

    Logger::trace("Remove Render view props");
#if VTK890
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#else
    this->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
#endif

    this->clearBinaryResults();
    Logger::debug("Unloading loaded data: Done");
}

void MainWindow::setBinaryResults() {
    Logger::debug("Setting results menu...");
    // Set result actions
    std::vector<float> steps;
    std::map<float, QMenu*> stepMenus;

    // Add results menus
    for (auto& result: this->binary->getResults()) {
        Logger::trace("Adding result sub-menu entry ", result.getStep());
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
                Logger::trace("Adding result sub-menu entry item ",
                              result.getComponents().at(i),
                              " (",
                              result.getStep(),
                              ")");
                resultItemAction =
                menu->addAction(QString::fromStdString(result.getComponents().at(i)));
            } else {
                Logger::trace("Adding result sub-menu entry item ", i, " (", result.getStep(), ")");
                resultItemAction = menu->addAction(QString::number(i));
            }

            connect(resultItemAction, &QAction::triggered, [this, &result, i]() {
                this->slotResult(result, i);
            });
        }
    }

    // Enable results menu
    Logger::trace("Enable results menu");
    this->menuResults->setEnabled(true);
    Logger::debug("Setting results menu: Done");
}

void MainWindow::clearBinaryResults() {
    Logger::debug("Clearing results menu: Done");
    this->menuResults->clear();
    Logger::debug("Clearing results menu: Done");
}