#include "binary_data_class.hpp"

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
void Binary_data_class::setScalarXYZ(int choice ){
    if ( choice>= 0 && choice <= 2  ){
        this->scalars = vtkSmartPointer<vtkFloatArray>::New(); 
        auto &res = this->results_.front(); 
        if(this->strList.size()>4){
            this->strList.removeLast();
            this->strList.removeLast();
        }
        this->strList << (QString::fromStdString("Result analysis ").toUpper()+ QString::fromStdString(res.analysis_))
                      <<(QString::fromStdString("Result ").toUpper()+ QString::fromStdString(res.results_));
        this->scalars->SetNumberOfValues( res.number_of_results_ );
        for (auto i = 0; i < res.number_of_results_; ++i) {
	        auto [node_number, data] = res.get_one_result(i);
	        //cout<<node_number << " " << typeid(data[0]).name()<<data[0]<<endl ;
            this->scalars->SetValue(i,data[choice]);
	}  
    }else throw invalid_argument("choice");
    
}
vtkSmartPointer<vtkFloatArray> Binary_data_class::getScalars()const{
    return this->scalars; 
}