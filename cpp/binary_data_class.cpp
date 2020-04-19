#include "binary_data_class.hpp"
#include  <fstream>
using namespace std ;

Binary_data_class::Binary_data_class(string str):Str_binary_data_GiD(str),pathToFile(str){
    read_meshes();
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
        for(auto i= 0 ; i<mesh.nb_of_elements_;i++){
            auto[id, element] = mesh.get_an_element(i);
            if(id == i+1){
                auto polygone = vtkSmartPointer<vtkPolygon>::New();
                polygone->GetPointIds()->SetNumberOfIds(mesh.nnode_);
                for (auto i = 0 ; i < mesh.nnode_ ; i++){
                    polygone->GetPointIds()->SetId ( i, element[i]--);
                }
                this->array->InsertNextCell( polygone );
            }
            else break;        
        }
    } 
}
void Binary_data_class::setScalarFromQT(const int& choice , const string& typeResult ){
        this->scalars = vtkSmartPointer<vtkFloatArray>::New();
        for (auto &&res : this->results_)
        {
            if (res.analysis_ == typeResult)
            {
                if(this->strList.size()>4){
                    this->strList.removeLast();
                    this->strList.removeLast();
                }
                this->strList<< (QString::fromStdString("Result analysis ").toUpper()+ QString::fromStdString(res.analysis_))
                            << (QString::fromStdString("Result ").toUpper()+ QString::fromStdString(res.results_));
                this->scalars->SetNumberOfValues( res.number_of_results_ );
                for (auto i = 0; i < res.number_of_results_; ++i) {
	                auto [node_number, data] = res.get_one_result(i);
                    this->scalars->SetValue(i,data[choice]);
                }
                break;
            }
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
    for (auto& res : binary_data.results_) {
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