#include "mainwindow.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyData.h>
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include "binary_data_GiD.hpp"

MainWindow::MainWindow(char *c)
{
    this->setupUi(this);

    GiD_PostInit();
	  //GiD_OpenPostResultFile("test.flavia.msh", GiD_PostAscii);
    Str_binary_data_GiD binary_data(string( string(c) + string("/Bar-Thermal-3034bSSE3-Pascal.flavia.res")));
	  binary_data.read_meshes();
	  //binary_data.write_meshes();
    while (auto one_result = binary_data.read_one_result()) {
		binary_data.results_.emplace_back(std::move(*one_result));
	  }

    auto points = vtkSmartPointer<vtkPoints>::New();
    auto verticales = vtkSmartPointer<vtkCellArray>::New();
    int nnode = 0; 
    for (auto &&mesh : binary_data.meshes_)
    {
      for (auto &&nodes : mesh.tab_of_nodes_)
      {
        cout<<"Node coord is :"<<nodes.number<<" "<<nodes.coord[0]<<" "<<nodes.coord[1]<<" "<<nodes.coord[2]<<endl;
        points->InsertNextPoint(nodes.coord[0],nodes.coord[1],nodes.coord[2]);
        nnode++;
      }
    }    
    auto pointsPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);
    
    auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qvtkWidget->setRenderWindow(renderWindow);

    auto scalars = vtkSmartPointer<vtkFloatArray>::New();
    scalars->SetNumberOfValues( nnode );
    for (float i = 0; i < nnode; i++)
    {
      scalars->SetValue(i,static_cast<float>(i)/nnode);
    }

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    cout<<"Numberofnode is "<<nnode<<endl;
    lut->SetNumberOfTableValues(nnode + 1 );
    lut->Build();
    lut->SetTableValue(0, colors->GetColor4d("Banana").GetData());
    lut->SetTableValue(1, colors->GetColor4d("Salmon").GetData());
    lut->SetTableValue(2, colors->GetColor4d("Tomato").GetData());
    lut->SetTableValue(3, colors->GetColor4d("Wheat").GetData());
    lut->SetTableValue(4, colors->GetColor4d("Lavender").GetData());
    lut->SetTableValue(5, colors->GetColor4d("Flesh").GetData());
    lut->SetTableValue(6, colors->GetColor4d("Raspberry").GetData());
    lut->SetTableValue(7,colors->GetColor4d("Black").GetData() );
    lut->SetTableValue(8, colors->GetColor4d("Mint").GetData());
    lut->SetTableValue(9, colors->GetColor4d("Peacock").GetData());

    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();
    vertexFilter->GetOutput()->GetPointData()->SetScalars(scalars);
    
  // Create a polydata object
    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(vertexFilter->GetOutput());

    auto polyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polyMapper->SetInputData(polyData);
    polyMapper->SetScalarRange(0, nnode);
    polyMapper->SetLookupTable(lut);

    
    auto polyActor = vtkSmartPointer<vtkActor>::New();
    polyActor->SetMapper(polyMapper);
    //polyActor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());
    polyActor->GetProperty()->SetPointSize(2);

  // VTK Renderer
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(polyActor);
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    this->qvtkWidget->renderWindow()->AddRenderer (renderer);
    this->qvtkWidget->renderWindow()->SetWindowName("AtilaSoftwareCalculator");

    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    //GiD_ClosePostResultFile();
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
