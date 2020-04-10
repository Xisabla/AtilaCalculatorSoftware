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
#include <vtkScalarBarActor.h>
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
    connect(this->actionDisplacement_X, SIGNAL(triggered()), this, SLOT(slotDisplacementXYZ()));
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
}
void MainWindow::slotOpenFile(){
  cout<<"Opening of a file .."<<endl;
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()){
    cout<<"File name is "<<fileName.toStdString()<<endl; 
    //delete(this->binary);   
    //this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
    this->binary = new Binary_data_class(fileName.toStdString());

    this->model->setStringList(this->binary->getstrList());

    this->listView->setModel(this->model); 

    //this->setScalars(nb_points);
   // this->scalars = ObjVtkfromGiD::setScalarsXYZ(this->binary,1); 
    this->binary->setScalarXYZ(1);
    double range[2]= {this->binary->getScalars()->GetRange()[0],this->binary->getScalars()->GetRange()[1]};
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(this->binary->getScalars()->GetNumberOfTuples() + 1 );
    lut->SetTableRange(range);
    cout<<range[0] << " " <<range[1] <<endl; 
    lut->Build();

    auto polyData = vtkSmartPointer<vtkPolyData>::New();

    polyData->SetPoints(this->binary->getvtkPoints());
    polyData->SetPolys(this->binary->getvtkCellArray());
    polyData->GetPointData()->SetScalars(this->binary->getScalars());

    auto polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputData(polyData);
    polyMapper->SetScalarRange(range);
    polyMapper->SetLookupTable(lut);

    auto scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(polyMapper->GetLookupTable());
    scalarBar->SetTitle("Displacement X");
    scalarBar->UnconstrainedFontSizeOn ();
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetBarRatio(scalarBar->GetBarRatio()/2.0);

    auto polyActor = vtkSmartPointer<vtkActor>::New();
    polyActor->SetMapper(polyMapper);
    //polyActor->GetProperty()->SetPointSize(2);

    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(polyActor);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(scalarBar);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();

  }
}
void MainWindow::slotDisplacementXYZ(){
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
    this->binary->setScalarXYZ(0);
    double range[2]= {this->binary->getScalars()->GetRange()[0],this->binary->getScalars()->GetRange()[1]};
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(this->binary->getScalars()->GetNumberOfTuples() + 1 );
    lut->SetTableRange(range);
    cout<<range[0] << " " <<range[1] <<endl; 
    lut->Build();

    auto polyData = vtkSmartPointer<vtkPolyData>::New();

    polyData->SetPoints(this->binary->getvtkPoints());
    polyData->SetPolys(this->binary->getvtkCellArray());
    polyData->GetPointData()->SetScalars(this->binary->getScalars());

    auto polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputData(polyData);
    polyMapper->SetScalarRange(range);
    polyMapper->SetLookupTable(lut);

    auto scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(polyMapper->GetLookupTable());
    scalarBar->SetTitle("Displacement X");
    scalarBar->UnconstrainedFontSizeOn ();
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetBarRatio(scalarBar->GetBarRatio()/2.0);

    auto polyActor = vtkSmartPointer<vtkActor>::New();
    polyActor->SetMapper(polyMapper);
    //polyActor->GetProperty()->SetPointSize(2);

    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(polyActor);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(scalarBar);
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
    
  }