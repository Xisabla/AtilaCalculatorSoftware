#include "mainwindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyData.h>
#include "binary_data_GiD.hpp"

MainWindow::MainWindow(char *c)
{
    this->setupUi(this);

    GiD_PostInit();
	  GiD_OpenPostResultFile("test.flavia.msh", GiD_PostAscii);
    Str_binary_data_GiD binary_data(string( string(c) + string("/Bar-Thermal-3034bSSE3-Pascal.flavia.res")));
	  binary_data.read_meshes();
	  binary_data.write_meshes();
    while (auto one_result = binary_data.read_one_result()) {
		binary_data.results_.emplace_back(std::move(*one_result));
	  }

    auto points = vtkSmartPointer<vtkPoints>::New();
    auto verticales = vtkSmartPointer<vtkCellArray>::New();
    for (auto &&mesh : binary_data.meshes_)
    {
      for (auto &&nodes : mesh.tab_of_nodes_)
      {
        cout<<"Node coord is :"<<nodes.number<<" "<<nodes.coord[0]<<" "<<nodes.coord[1]<<" "<<nodes.coord[2]<<endl;
        points->InsertNextPoint(nodes.coord[0],nodes.coord[1],nodes.coord[2]);
      }
    }
    
    auto pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);
    
    auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    qvtkWidget->setRenderWindow(renderWindow);
  
  // Create a polydata object
    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(vertexFilter->GetOutput());

    auto polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputData(polyData);
    auto polyActor = vtkSmartPointer<vtkActor>::New();
    polyActor->SetMapper(polyMapper);
    polyActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    polyActor->GetProperty()->SetPointSize(20);
  // VTK Renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(polyActor);
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());
    this->qvtkWidget->renderWindow()->AddRenderer (renderer);
    this->qvtkWidget->renderWindow()->SetWindowName("AtilaSoftwareCalculator");
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    GiD_ClosePostResultFile();
	  GiD_PostDone();
}

MainWindow::~MainWindow()
{
    delete this;
}
void MainWindow::slotExit()
{
    qApp->exit();
}
