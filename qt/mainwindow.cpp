#include "mainwindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNew.h>
#include <vtkRendererCollection.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkAxesActor.h>
#include <vtkLookupTable.h>
#include <QVTKInteractor.h>
#include <QString>
#include <QtWidgets>

#include "objVtkfromGiD.hpp"

MainWindow::MainWindow(char *c )
{
    this->setupUi(this);

   //this->binary = new Binary_data_class(string(c)+string("/RecHarmonicTest-3032b-GiD1201.flavia.res"));
	  
   // auto [points , triangles , nb_points] = this->createTrianglesFromMeshCoordinates(this->binary);

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    this->qvtkWidget->setRenderWindow(renderWindow);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    this->model = new QStringListModel(this);
   /* this->setScalars(nb_points);
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(nb_points + 1 );
    lut->Build();
    
    // Create a polydata object
    auto polyData = vtkSmartPointer<vtkPolyData>::New();

    polyData->SetPoints(points);
    polyData->SetPolys(triangles);
    polyData->GetPointData()->SetScalars(this->scalars);

    auto polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputData(polyData);
    polyMapper->SetLookupTable(lut);
    
    auto polyActor = vtkSmartPointer<vtkActor>::New();
    polyActor->SetMapper(polyMapper);
    polyActor->GetProperty()->SetPointSize(2);*/

    // VTK Renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    //renderer->AddActor(polyActor);

    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    this->qvtkWidget->renderWindow()->AddRenderer (renderer);
    this->qvtkWidget->renderWindow()->SetWindowName("AtilaSoftwareCalculator");
    this->setAxes(colors);
    renderer->ResetCamera();

    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
}
void MainWindow::setScalars(const int &nb_points){
    this->scalars = vtkSmartPointer<vtkFloatArray>::New();
    scalars->SetNumberOfValues( nb_points );
    for (float i = 0; i < nb_points; i++)
    {
      scalars->SetValue(i,static_cast<float>(i)/nb_points);
    }
}
void MainWindow::setAxes(vtkSmartPointer<vtkNamedColors> &colors){
    auto axes = vtkSmartPointer<vtkAxesActor>::New();
    this->widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    double rgba[4]{0.0, 0.0, 0.0, 0.0};
    colors->GetColor("Carrot",rgba);
    this->widget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    this->widget->SetOrientationMarker( axes );
    this->widget->SetInteractor( this->qvtkWidget->interactor() );
    this->widget->SetViewport( 0.0, 0.0, 0.2, 0.2 );
    this->widget->SetEnabled( 1 );
    this->widget->InteractiveOn();
}

MainWindow::~MainWindow()
{
    delete this;
}
void MainWindow::slotExit()
{
    cout<<"Out"<<endl;
    qApp->exit();
    delete this ; 
}
void MainWindow::slotOpenFile(){
  cout<<"Opening of a file .."<<endl;
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()){
    cout<<"File name is "<<fileName.toStdString()<<endl; 
    //delete(this->binary);   
    //this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
    this->binary = new Binary_data_class(fileName.toStdString());

    auto [points , cellArray , nb_points , List] = ObjVtkfromGiD::createPolygonFromMeshCoordinates(this->binary);

    this->model->setStringList(List);

    this->listView->setModel(this->model); 

    this->setScalars(nb_points);

    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(nb_points + 1 );
    lut->Build();

    auto polyData = vtkSmartPointer<vtkPolyData>::New();

    polyData->SetPoints(points);
    polyData->SetPolys(cellArray);
    polyData->GetPointData()->SetScalars(this->scalars);

    auto polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputData(polyData);
    polyMapper->SetLookupTable(lut);
    
    auto polyActor = vtkSmartPointer<vtkActor>::New();
    polyActor->SetMapper(polyMapper);
    polyActor->GetProperty()->SetPointSize(2);

    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(polyActor);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();

  }
}