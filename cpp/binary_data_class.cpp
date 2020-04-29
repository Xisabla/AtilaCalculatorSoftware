#include "binary_data_class.hpp"
#include  <fstream>
using namespace std ;

Binary_data_class::Binary_data_class(string str):Str_binary_data_GiD(str),pathToFile(str){
    this->read_meshes();
    while (auto one_result = read_one_result()) {
	    results_.emplace_back(move(*one_result));
	}
    this->setUpGiDtoVTK();
}
string Binary_data_class::getPath() const {
    return pathToFile;
}
Binary_data_class::~Binary_data_class(){
    
}
vtkSmartPointer<vtkPoints> Binary_data_class::getvtkPoints()const{
    return this->points;
}
vtkSmartPointer<vtkCellArray> Binary_data_class::getvtkCellArray()const{
    return this->array;
}
QStringList Binary_data_class::getstrList()const {
    return this->strList;
} 
void Binary_data_class::setUpGiDtoVTK(){
    this->points = vtkSmartPointer<vtkPoints>::New();
    this->array = vtkSmartPointer<vtkCellArray>::New();
    this->uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    for (auto &&mesh : this->meshes_)
    {
        this->strList<< (QString::fromStdString("Mesh Name : ").toUpper()+ QString::fromStdString(mesh.mesh_name_))
                    << (QString::fromStdString("Mesh Dimension : ").toUpper()+ QString::number(mesh.ndim_))
                    << (QString::fromStdString("Element Type : ").toUpper()+ QString::fromStdString(mesh.element_name_))
                    << (QString::fromStdString("Number of nodes : ").toUpper()+ QString::number(mesh.nnode_));
        for (auto &&nodes : mesh.tab_of_nodes_)
        {
            this->points->InsertNextPoint(nodes.coord[0],nodes.coord[1],nodes.coord[2]);
        }
        this->uGrid->SetPoints(this->points);
        for(auto a= 0 ; a<mesh.nb_of_elements_;a++){
            auto[id, element] = mesh.get_an_element(a);
            if(id == a+1){  
                auto polygone = this->createVTKCell(mesh.element_name_,mesh.ndim_);
                if (polygone != NULL){
                    polygone->GetPointIds()->SetNumberOfIds(mesh.nnode_);
                    for (auto i = 0; i<mesh.nnode_ ; i++){
                        //crée une forme en fonction de l'id des points donnée par les elements
                        //Il y a un décalage de car lors de l'insertion des points dans le vtkPoints, le point avec l'id 1 devient le point 0 
                        //Donc le point id 1 et a la position 0 dans le tableau 
                        polygone->GetPointIds()->SetId ( i, element[i]-1);
                    }
                    this->array->InsertNextCell( polygone );
                    this->uGrid->InsertNextCell( polygone->GetCellType(), polygone->GetPointIds() );
                }
                
            }
            else break;        
        }
    } 
}
void Binary_data_class::setScalarFromQT2(Str_Result& res,const int& choice ){
        this->scalars = vtkSmartPointer<vtkFloatArray>::New();
        while (this->strList.size()>4)
        {
            this->strList.removeLast();
        }
        this->strList<< (QString::fromStdString("Result analysis ").toUpper()+ QString::fromStdString(res.analysis_))
                    << (QString::fromStdString("Result ").toUpper()+ QString::fromStdString(res.results_))
                    <<(QString::fromStdString("Step ").toUpper()+ QString::number(res.step_))
                    <<(QString::fromStdString("Choice ").toUpper()+ QString::number(choice));
        this->scalars->SetNumberOfValues( res.number_of_results_ );
        for (auto i = 0; i < res.number_of_results_; ++i) {
	        auto [node_number, data] = res.get_one_result(i);
            this->scalars->SetValue(i,data[choice]);
        }
}
vtkSmartPointer<vtkFloatArray> Binary_data_class::getScalars()const{
    return this->scalars; 
}
void Binary_data_class::toTextFile(){
    GiD_PostInit();
	GiD_OpenPostResultFile("test.flavia.msh", GiD_PostAscii);
	this->read_meshes();
	this->write_meshes();
    ofstream SaveFile("test.flavia.res");
    for (auto& res : this->results_) {
		SaveFile << "Result " << "\"" << res.analysis_ << "\"" << "  Result   " << res.results_ << "  Step  " << res.step_ << "  "<< std::endl;;
		SaveFile << "Values";
		//for (auto& comp : res.component_names_) {
		//	SaveFile << comp << " ";
		//}
		SaveFile << std::endl;
		for (auto i = 0; i < res.number_of_results_; ++i) {
			auto [node_number, data] = res.get_one_result(i);
			SaveFile << node_number << " ";
			for (auto j = 0; j < res.result_size_; ++j) {
				SaveFile << data[j] << " ";
			}
			SaveFile << std::endl;
		}
		SaveFile << "End Values" << std::endl;
	}
	SaveFile.close();
	GiD_ClosePostResultFile();
	GiD_PostDone();
}
vtkSmartPointer<vtkUnstructuredGrid> Binary_data_class::getUGrid()const{
    return uGrid ; 
}
vtkSmartPointer<vtkCell> Binary_data_class::createVTKCell(const string& str,const int& ndim_) {
    if (ndim_ == 2 )
    {
        if (str == "Triangle")
        {
            auto cell = vtkSmartPointer<vtkQuadraticTriangle>::New(); 
            return cell ; 
        }
        else if ( str == "Pyramid")
        {
            auto cell = vtkSmartPointer<vtkQuadraticPyramid>::New();
            return cell ; 
        }
        else if (str == "Quadrilateral")
        {
            auto cell = vtkSmartPointer<vtkQuad>::New();
            return cell ;
        }
        
        else
        {
            auto cell = vtkSmartPointer<vtkPolygon>::New();
            return cell ; 
        }
        
    }
    else if (ndim_ == 3 )
    {
        if (str == "Hexahedra")
        {
            auto cell = vtkSmartPointer<vtkHexahedron>::New(); 
            return cell ; 
        }
        else if(str == "Pyramid" )
        {
            auto cell = vtkSmartPointer<vtkPyramid>::New(); 
            return cell ; 
        }
        
        else
        {
            auto cell = vtkSmartPointer<vtkPolyhedron>::New(); 
            return cell ; 
        }
        
    }
    else
    {
        if (str == "Linear")
        {
            auto cell = vtkSmartPointer<vtkLine>::New();
            return cell ; 
        }
        else if (str == "Point")
        {
            return NULL ; 
        }
        else
        {
            return NULL ; 
        }
        
        
    }
    
    
}