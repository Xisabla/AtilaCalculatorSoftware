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
    this->objDirectory = QString::fromUtf8(c);
   //this->binary = new Binary_data_class(string(c)+string("/RecHarmonicTest-3032b-GiD1201.flavia.res"));

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    this->qvtkWidget->setRenderWindow(renderWindow);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    this->model = new QStringListModel(this);
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    this->qvtkWidget->renderWindow()->AddRenderer (renderer);
    this->qvtkWidget->renderWindow()->SetWindowName("AtilaSoftwareCalculator");
    this->setAxes(colors);
    renderer->ResetCamera();

    this->signalMapper = new QSignalMapper (this) ; 

    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
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
  QString fileName = QFileDialog::getOpenFileName(this,
                        "Select a .res file",
                        this->objDirectory,
                        "Res (*.res)");
  if (!fileName.isEmpty()){
    cout<<"File name is "<<fileName.toStdString()<<endl; 
    //delete(this->binary);   
    //this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
    this->binary = new Binary_data_class(fileName.toStdString());
    for (auto &&res : this->binary->results_)
    {
      QMenu *menu = this->menuResults->addMenu(QString::fromStdString(res.analysis_));
      for (int i = 0 ; i<res.result_size_ ; i++){
        QAction *a = menu->addAction(QString::number(i));
        connect(a, SIGNAL(triggered()), this->signalMapper, SLOT(map()));
        const QString tokens =  QString::fromStdString(res.analysis_)+QString::fromStdString(";")+QString::number(i);
        
        this->signalMapper->setMapping(a,tokens);
      }
    }
    connect (this->signalMapper, SIGNAL(mapped(const QString&)), this, SLOT(slotResult(const QString& )));
    this->setVTK(0,this->binary->results_.front().analysis_);
  }
}
void MainWindow::slotResult(const QString& typeResult){
    this->qvtkWidget->renderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
    QStringList tokens = typeResult.split (";");

    if (tokens.count () == 2) {
        string strTypeResult = tokens.at(0).toStdString ();
        int choice = tokens.at(1).toInt ();
        this->setVTK(choice,strTypeResult);  
    }
  }
void MainWindow::setVTK(const int& choice ,const std::string& typeResult){
    this->binary->setScalarFromQT(choice,typeResult);

    this->model->setStringList(this->binary->getstrList());
    this->listView->setModel(this->model); 

    double range[2]= {this->binary->getScalars()->GetRange()[0],this->binary->getScalars()->GetRange()[1]};
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(this->binary->getScalars()->GetNumberOfTuples() + 1 );
    lut->SetTableRange(range);
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
    scalarBar->SetTitle( string(typeResult + std::__cxx11::to_string(choice)).c_str()  );
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