#ifndef BINARYDATATHREAD_HPP
#define BINARYDATATHREAD_HPP
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolygon.h>
#include <QString>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <QStringList>
#include <QAction>
#include <QList>
#include <vtkFloatArray.h>
#include <string>
#include "binary_data_GiD.hpp"

class Binary_data_class : public Str_binary_data_GiD{

    public:

        Binary_data_class(std::string str);
        ~Binary_data_class();
        void setUpGiDtoVTK();
        void setScalarFromQT(const int& choice , const std::string& typeResult);
        vtkSmartPointer<vtkFloatArray> getScalars()const;
        vtkSmartPointer<vtkPoints> getvtkPoints()const;
        vtkSmartPointer<vtkCellArray> getvtkCellArray()const;
        QStringList getstrList()const ; 
        std::string getPath() const;
        void toTextFile();

    private:

        std::string pathToFile ; 
        QStringList strList ; 
        vtkSmartPointer<vtkPoints> points;
        vtkSmartPointer<vtkCellArray> array; 
        vtkSmartPointer<vtkFloatArray> scalars; 

    protected:
};
#endif 