/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryDataClass.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "BinaryDataClass.h"

//  --------------------------------------------------------------------------------------
//  BINARY_DATA_WRAPPER
//  --------------------------------------------------------------------------------------

BinaryDataWrapper::BinaryDataWrapper(std::string file): BinaryData(file) {
    this->readMeshes();
    this->readResults();
    this->convertFromGiD();
}

BinaryDataWrapper::~BinaryDataWrapper() { }

//  --------------------------------------------------------------------------------------
//  BINARY_DATA_WRAPPER > GETTERS
//  --------------------------------------------------------------------------------------

vtkSmartPointer<vtkUnstructuredGrid> BinaryDataWrapper::getUnstructuredGrid() const {
    return this->unstructuredGrid;
}

vtkSmartPointer<vtkFloatArray> BinaryDataWrapper::getScalars() const { return this->scalars; }

QStringList BinaryDataWrapper::getInformationList() const { return this->informationList; }

//  --------------------------------------------------------------------------------------
//  BINARY_DATA_WRAPPER > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

void BinaryDataWrapper::loadResult(Result& result, const unsigned int& component) {
    this->scalars = vtkNew<vtkFloatArray>();
    this->scalars->SetNumberOfValues(result.getValuesCount());

    for (unsigned int i = 0; i < result.getValuesCount(); i++) {
        auto [id, value] = result.getResult(i);
        this->scalars->SetValue(i, value[component]);
    }

    while (this->informationList.size() > 4) this->informationList.removeLast();

    this->informationList << (QString::fromStdString("Result analysis ").toUpper() +
                              QString::fromStdString(result.getAnalysis()))
                          << (QString::fromStdString("Result ").toUpper() +
                              QString::fromStdString(result.getResultType()))
                          << (QString::fromStdString("Step ").toUpper() +
                              QString::number(result.getStep()))
                          << (QString::fromStdString("Choice ").toUpper() +
                              QString::number(component));
}

//  --------------------------------------------------------------------------------------
//  BINARY_DATA_WRAPPER > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void BinaryDataWrapper::loadMeshInformation(Mesh::Mesh& mesh) {
    this->informationList << (QString::fromStdString("Mesh Name : ").toUpper() +
                              QString::fromStdString(mesh.getName()))
                          << (QString::fromStdString("Mesh Dimension : ").toUpper() +
                              QString::number(mesh.getDimCount()))
                          << (QString::fromStdString("Element Type : ").toUpper() +
                              QString::fromStdString(mesh.getElementName()))
                          << (QString::fromStdString("Number of nodes : ").toUpper() +
                              QString::number(mesh.getNodeCount()));
}

void BinaryDataWrapper::convertFromGiD() {
    this->points = vtkNew<vtkPoints>();
    this->unstructuredGrid = vtkNew<vtkUnstructuredGrid>();

    for (Mesh::Mesh& mesh: this->meshes) {
        this->loadMeshInformation(mesh);

        for (Mesh::Node& node: mesh.getNodes()) this->points->InsertNextPoint(node.getCoords());

        this->unstructuredGrid->SetPoints(this->points);

        for (unsigned int i = 0; i < mesh.getElementCount(); i++) {
            auto [id, nid] = mesh.getElement(i);

            if (id != i + 1) break;

            auto polygon = this->getPolygonVTKCell(mesh.getElementName(), mesh.getDimCount());
            if (polygon != NULL) {
                polygon->GetPointIds()->SetNumberOfIds(mesh.getNodeCount());
                for (auto j = 0; j < mesh.getNodeCount(); j++)
                    polygon->GetPointIds()->SetId(j, nid[j] - 1);

                this->unstructuredGrid->InsertNextCell(polygon->GetCellType(),
                                                       polygon->GetPointIds());
            }
        }
    }
}

vtkSmartPointer<vtkCell> BinaryDataWrapper::getPolygonVTKCell(const std::string& meshElement,
                                                              const unsigned int& dimCount) {
    if (dimCount == 2) {
        if (meshElement == "Triangle")
            return vtkSmartPointer<vtkQuadraticTriangle>::New();
        else if (meshElement == "Pyramid")
            return vtkSmartPointer<vtkQuadraticPyramid>::New();
        else if (meshElement == "Quadrilateral")
            return vtkSmartPointer<vtkQuad>::New();

        return vtkSmartPointer<vtkPolygon>::New();
    } else if (dimCount == 3) {
        if (meshElement == "Hexahedra")
            return vtkSmartPointer<vtkHexahedron>::New();
        else if (meshElement == "Pyramid")
            return vtkSmartPointer<vtkPyramid>::New();

        return vtkSmartPointer<vtkPolyhedron>::New();
    } else {
        if (meshElement == "Linear") return vtkSmartPointer<vtkLine>::New();

        return NULL;
    }
}