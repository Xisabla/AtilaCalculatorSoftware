#include "mainwindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

MainWindow::MainWindow()
{
    this->setupUi(this);

    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qvtkWidget->setRenderWindow(renderWindow);
     // Sphere
    auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    auto sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    auto sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    sphereActor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());

  // VTK Renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(sphereActor);
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());
    this->qvtkWidget->renderWindow()->AddRenderer (renderer);
    this->qvtkWidget->renderWindow()->SetWindowName(
        "AtilaSoftwareCalculator");
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}
const void MainWindow::setTextinTextZone(const QString& a ){
    
}
MainWindow::~MainWindow()
{
    delete this;
}
void MainWindow::slotExit()
{
    qApp->exit();
}
