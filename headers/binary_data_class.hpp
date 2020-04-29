#ifndef BINARYDATATHREAD_HPP
#define BINARYDATATHREAD_HPP
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolygon.h>
#include <vtkQuadraticTriangle.h>
#include <QString>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <QStringList>
#include <vtkQuad.h>
#include <vtkPolyhedron.h>
#include <vtkPyramid.h>
#include <vtkHexahedron.h>
#include <vtkFloatArray.h>
#include <vtkUnstructuredGrid.h>
#include <string>
#include <vtkLine.h>
#include <vtkQuadraticPyramid.h>
#include "binary_data_GiD.hpp"

class Binary_data_class : public Str_binary_data_GiD{

    public:

        Binary_data_class(std::string str);
        ~Binary_data_class();
        void setUpGiDtoVTK();
        void setScalarFromQT2(Str_Result& res,const int& choice );
        vtkSmartPointer<vtkFloatArray> getScalars()const;
        vtkSmartPointer<vtkPoints> getvtkPoints()const;
        vtkSmartPointer<vtkCellArray> getvtkCellArray()const;
        vtkSmartPointer<vtkUnstructuredGrid> getUGrid()const;
        QStringList getstrList()const ; 
        std::string getPath() const;
        void toTextFile();
        vtkSmartPointer<vtkCell> createVTKCell(const string& mesh_name_,const int& ndim_) ;

    private:
        vtkSmartPointer<vtkUnstructuredGrid> uGrid;
        std::string pathToFile ; 
        QStringList strList ; 
        vtkSmartPointer<vtkPoints> points;
        vtkSmartPointer<vtkCellArray> array; 
        vtkSmartPointer<vtkFloatArray> scalars; 
    protected:
};
#endif 