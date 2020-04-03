#include "mainwindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkAxesActor.h>
#include <vtkLookupTable.h>
#include <vtkOrientationMarkerWidget.h>
#include <QString>

MainWindow::MainWindow(char *c )
{
    this->setupUi(this);
    Str_binary_data_GiD binary_data(string( string(c) + string("/Bar-Thermal-3034bSSE3-Pascal.flavia.res")));
	  binary_data.read_meshes();
    while (auto one_result = binary_data.read_one_result()) {
		binary_data.results_.emplace_back(std::move(*one_result));
	  }
    
    auto [points , triangles , nb_points] = this->createTrianglesFromMeshCoordinates(binary_data);

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qvtkWidget->setRenderWindow(renderWindow);
    
    this->setScalars(nb_points);
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
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
    polyActor->GetProperty()->SetPointSize(2);
    

    // VTK Renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(polyActor);
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    this->qvtkWidget->renderWindow()->AddRenderer (renderer);
    this->qvtkWidget->renderWindow()->SetWindowName("AtilaSoftwareCalculator");

    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}
const void MainWindow::setScalars(const int &nb_points){
    this->scalars = vtkSmartPointer<vtkFloatArray>::New();
    scalars->SetNumberOfValues( nb_points );
    for (float i = 0; i < nb_points; i++)
    {
      scalars->SetValue(i,static_cast<float>(i)/nb_points);
    }
}
std::tuple< vtkSmartPointer<vtkPoints> , vtkSmartPointer<vtkCellArray> ,const int > MainWindow::createTrianglesFromMeshCoordinates(Str_binary_data_GiD &binary_data){
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto triangles = vtkSmartPointer<vtkCellArray>::New();
    this->model = new QStringListModel(this);
    QStringList List;
    int nb_points = 0; 
    for (auto &&mesh : binary_data.meshes_)
    {
      List<< (QString::fromStdString("Mesh Name : ").toUpper()+ QString::fromStdString(mesh.mesh_name_))
          << (QString::fromStdString("Mesh Dimension : ").toUpper()+ QString::number(mesh.ndim_))
          << (QString::fromStdString("Element Type : ").toUpper()+ QString::fromStdString(mesh.element_name_))
          << (QString::fromStdString("Number of nodes : ").toUpper()+ QString::number(mesh.nnode_));
      for (auto &&nodes : mesh.tab_of_nodes_)
      {
        points->InsertNextPoint(nodes.coord[0],nodes.coord[1],nodes.coord[2]);
        nb_points++;
      }
      for(auto i= 0 ; i<mesh.nb_of_elements_;i++){
        auto[id, element] = mesh.get_an_element(i);
        if(id == i+1){
          auto triangle = vtkSmartPointer<vtkTriangle>::New();
          triangle->GetPointIds()->SetId ( 0, element[0]--);
          triangle->GetPointIds()->SetId ( 1, element[1]--);
          triangle->GetPointIds()->SetId ( 2, element[2]--);
          triangles->InsertNextCell ( triangle );
          }
        else break;        
      }
    }
    this->model->setStringList(List);
    this->listView->setModel(model);
    return std::make_tuple(points,triangles,nb_points);
    throw std::invalid_argument("binary data");   
}
MainWindow::~MainWindow()
{
    delete this;
}
void MainWindow::slotExit()
{
    qApp->exit();
}
