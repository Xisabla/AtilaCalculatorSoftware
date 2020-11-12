/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryDataClass.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "BinaryDataClass.h"

using namespace std;

Binary_data_class::Binary_data_class(std::string str): Str_binary_data_GiD(str), pathToFile(str) {
    this->read_meshes();
    while (auto one_result = read_one_result()) {
        results_.emplace_back(move(*one_result));
    }
    this->setUpGiDtoVTK();
}
std::string Binary_data_class::getPath() const { return pathToFile; }
Binary_data_class::~Binary_data_class() { }
vtkSmartPointer<vtkPoints> Binary_data_class::getvtkPoints() const { return this->points; }
vtkSmartPointer<vtkCellArray> Binary_data_class::getvtkCellArray() const { return this->array; }
QStringList Binary_data_class::getstrList() const { return this->strList; }
void Binary_data_class::setUpGiDtoVTK() {
    this->points = vtkSmartPointer<vtkPoints>::New();
    this->array = vtkSmartPointer<vtkCellArray>::New();
    this->uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    for (auto&& mesh: this->meshes_) {
        this->strList << (QString::fromStdString("Mesh Name : ").toUpper() +
                          QString::fromStdString(mesh.getName()))
                      << (QString::fromStdString("Mesh Dimension : ").toUpper() +
                          QString::number(mesh.getDimCount()))
                      << (QString::fromStdString("Element Type : ").toUpper() +
                          QString::fromStdString(mesh.getElementName()))
                      << (QString::fromStdString("Number of nodes : ").toUpper() +
                          QString::number(mesh.getNodeCount()));
        for (auto&& nodes: mesh.getNodes()) {
            this->points->InsertNextPoint(nodes.getX(), nodes.getY(), nodes.getZ());
        }
        this->uGrid->SetPoints(this->points);
        for (auto a = 0; a < mesh.getElementCount(); a++) {
            auto [id, element] = mesh.getElement(a);
            if (id == a + 1) {
                auto polygone = this->createVTKCell(mesh.getElementName(), mesh.getDimCount());
                if (polygone != NULL) {
                    polygone->GetPointIds()->SetNumberOfIds(mesh.getNodeCount());
                    for (auto i = 0; i < mesh.getNodeCount(); i++) {
                        // crée une forme en fonction de l'id des points donnée par les elements
                        // Il y a un décalage de car lors de l'insertion des points dans le
                        // vtkPoints, le point avec l'id 1 devient le point 0 Donc le point id 1 et
                        // a la position 0 dans le tableau
                        polygone->GetPointIds()->SetId(i, element[i] - 1);
                    }
                    this->array->InsertNextCell(polygone);
                    this->uGrid->InsertNextCell(polygone->GetCellType(), polygone->GetPointIds());
                }

            } else
                break;
        }
    }
}
void Binary_data_class::setScalarFromQT2(Str_Result& res, const int& choice) {
    this->scalars = vtkSmartPointer<vtkFloatArray>::New();
    while (this->strList.size() > 4) {
        this->strList.removeLast();
    }
    this->strList << (QString::fromStdString("Result analysis ").toUpper() +
                      QString::fromStdString(res.analysis_))
                  << (QString::fromStdString("Result ").toUpper() +
                      QString::fromStdString(res.results_))
                  << (QString::fromStdString("Step ").toUpper() + QString::number(res.step_))
                  << (QString::fromStdString("Choice ").toUpper() + QString::number(choice));
    this->scalars->SetNumberOfValues(res.number_of_results_);
    for (auto i = 0; i < res.number_of_results_; ++i) {
        auto [node_number, data] = res.get_one_result(i);
        this->scalars->SetValue(i, data[choice]);
    }
}
vtkSmartPointer<vtkFloatArray> Binary_data_class::getScalars() const { return this->scalars; }
void Binary_data_class::toTextFile() {
    GiD_PostInit();
    GiD_OpenPostResultFile("test.flavia.msh", GiD_PostAscii);
    this->read_meshes();
    this->write_meshes();
    ofstream SaveFile("test.flavia.res");
    for (auto& res: this->results_) {
        SaveFile << "Result "
                 << "\"" << res.analysis_ << "\""
                 << "  Result   " << res.results_ << "  Step  " << res.step_ << "  " << std::endl;
        ;
        SaveFile << "Values";
        // for (auto& comp : res.component_names_) {
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
vtkSmartPointer<vtkUnstructuredGrid> Binary_data_class::getUGrid() const { return uGrid; }
vtkSmartPointer<vtkCell> Binary_data_class::createVTKCell(const std::string& str,
                                                          const int& ndim_) {
    if (ndim_ == 2) {
        if (str == "Triangle") {
            auto cell = vtkSmartPointer<vtkQuadraticTriangle>::New();
            return cell;
        } else if (str == "Pyramid") {
            auto cell = vtkSmartPointer<vtkQuadraticPyramid>::New();
            return cell;
        } else if (str == "Quadrilateral") {
            auto cell = vtkSmartPointer<vtkQuad>::New();
            return cell;
        }

        else {
            auto cell = vtkSmartPointer<vtkPolygon>::New();
            return cell;
        }

    } else if (ndim_ == 3) {
        if (str == "Hexahedra") {
            auto cell = vtkSmartPointer<vtkHexahedron>::New();
            return cell;
        } else if (str == "Pyramid") {
            auto cell = vtkSmartPointer<vtkPyramid>::New();
            return cell;
        }

        else {
            auto cell = vtkSmartPointer<vtkPolyhedron>::New();
            return cell;
        }

    } else {
        if (str == "Linear") {
            auto cell = vtkSmartPointer<vtkLine>::New();
            return cell;
        } else if (str == "Point") {
            return NULL;
        } else {
            return NULL;
        }
    }
}