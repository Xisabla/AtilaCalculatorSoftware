#include "objVtkfromGiD.hpp"
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolygon.h>


std::tuple< vtkSmartPointer<vtkPoints> , vtkSmartPointer<vtkCellArray> ,const int, QStringList > ObjVtkfromGiD::createPolygonFromMeshCoordinates(Binary_data_class *binary_data){
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto cellArray = vtkSmartPointer<vtkCellArray>::New();
    QStringList List;
    int nb_points = 0; 
    for (auto &&mesh : binary_data->meshes_)
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
                auto polygone = vtkSmartPointer<vtkPolygon>::New();
                polygone->GetPointIds()->SetNumberOfIds(mesh.nnode_);
                for (auto i = 0 ; i < mesh.nnode_ ; i++){
                    polygone->GetPointIds()->SetId ( i, element[i]--);
                }
                cellArray->InsertNextCell ( polygone );
            }
            else break;        
        }
    }
    return std::make_tuple(points,cellArray,nb_points,List);
    throw std::invalid_argument("binary data");   
}