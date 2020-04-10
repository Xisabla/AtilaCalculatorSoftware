#ifndef OBJVTKFROMGID_HPP
#define OBJVTKFROMGID_HPP
#include <QString>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <QStringList>
#include <vtkFloatArray.h>

#include "binary_data_class.hpp"

class ObjVtkfromGiD {
    public:
        static std::tuple< vtkSmartPointer<vtkPoints> , vtkSmartPointer<vtkCellArray> ,const int, QStringList > createPolygonFromMeshCoordinates(Binary_data_class *binary_data);
        static vtkSmartPointer<vtkFloatArray> setScalarsXYZ(Binary_data_class *binary_data, int choice); 
    protected:
    private:
       
};


#endif 