/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      binary_data_wrapper.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/binary_data_wrapper.h"

//  --------------------------------------------------------------------------------------
//  BINARY DATA WRAPPER
//  --------------------------------------------------------------------------------------

BinaryDataWrapper::BinaryDataWrapper(const std::string& file): BinaryData(file) {
    this->readMeshes();
    this->readResults();
    this->convertFromGiD();
}

BinaryDataWrapper::~BinaryDataWrapper() = default;

//  --------------------------------------------------------------------------------------
//  BINARY DATA WRAPPER > GETTERS
//  --------------------------------------------------------------------------------------

vtkSmartPointer<vtkUnstructuredGrid> BinaryDataWrapper::getUnstructuredGrid() const {
    return this->unstructuredGrid;
}

vtkSmartPointer<vtkFloatArray> BinaryDataWrapper::getScalars() const { return this->scalars; }

QStringList BinaryDataWrapper::getInformationList() const { return this->informationList; }

//  --------------------------------------------------------------------------------------
//  BINARY DATA WRAPPER > PUBLIC METHODS
//  --------------------------------------------------------------------------------------

void BinaryDataWrapper::loadResult(Result& result, const int& component) {
    Logger::info("Reading result ", result.getAnalysis(), ":", component, "...");
    this->scalars = vtkNew<vtkFloatArray>();
    this->scalars->SetNumberOfValues(result.getValuesCount());

    // Set scalar value for each result values
    for (unsigned int i = 0; i < result.getValuesCount(); i++) {
        auto [id, value] = result.getResult(i);
        this->scalars->SetValue(i, value[component]);
    }

    // Keep only relevant information
    // NOTE: Can be optimized by not adding irrelevant information
    while (this->informationList.size() > 4) this->informationList.removeLast();

    Logger::debug("Updating information list");
    this->informationList << (QString::fromStdString("Result analysis ").toUpper() +
                              QString::fromStdString(result.getAnalysis()))
                          << (QString::fromStdString("Result ").toUpper() +
                              QString::fromStdString(result.getResultType()))
                          << (QString::fromStdString("Step ").toUpper() +
                              QString::number(result.getStep()))
                          << (QString::fromStdString("Choice ").toUpper() +
                              QString::number(component));

    Logger::info("Reading result ", result.getAnalysis(), ":", component, " : Done");
}

//  --------------------------------------------------------------------------------------
//  BINARY DATA WRAPPER > PRIVATE METHODS
//  --------------------------------------------------------------------------------------

void BinaryDataWrapper::loadMeshInformation(Mesh& mesh) {
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
    Logger::debug("Converting mesh values to VTK elements...");
    this->points = vtkNew<vtkPoints>();
    this->unstructuredGrid = vtkNew<vtkUnstructuredGrid>();

    for (Mesh& mesh: this->meshes) {
        // Append information
        Logger::trace("Loading mesh information");
        this->loadMeshInformation(mesh);

        // Add node to points
        Logger::trace("Inserting nodes as points");
        for (Node& node: mesh.getNodes()) this->points->InsertNextPoint(node.getCoords());
        this->unstructuredGrid->SetPoints(this->points);

        for (unsigned int i = 0; i < mesh.getElementCount(); i++) {
            Logger::trace("Adding element ", i, " to the VTK UnstructuredGrid");
            auto [id, nid] = mesh.getElement(i);

            if (id != i + 1) break;

            auto polygon =
            BinaryDataWrapper::getPolygonVTKCell(mesh.getElementName(), mesh.getDimCount());

            if (polygon != nullptr) {
                // Set the number of points in the polygon
                polygon->GetPointIds()->SetNumberOfIds(mesh.getNodeCount());
                // Set each point of the polygon
                for (auto j = 0; j < mesh.getNodeCount(); j++)
                    polygon->GetPointIds()->SetId(j, nid[j] - 1);

                this->unstructuredGrid->InsertNextCell(polygon->GetCellType(),
                                                       polygon->GetPointIds());
            }
        }
    }

    Logger::debug("Converting mesh values to VTK elements: Done");
}

vtkSmartPointer<vtkCell> BinaryDataWrapper::getPolygonVTKCell(const std::string& meshElement,
                                                              const unsigned int& dimCount) {
    if (dimCount == 2) {
        // 2D Elements
        if (meshElement == "Triangle") return vtkSmartPointer<vtkQuadraticTriangle>::New();
        else if (meshElement == "Pyramid")
            return vtkSmartPointer<vtkQuadraticPyramid>::New();
        else if (meshElement == "Quadrilateral")
            return vtkSmartPointer<vtkQuad>::New();

        return vtkSmartPointer<vtkPolygon>::New();
    } else if (dimCount == 3) {
        // 3D Elements
        if (meshElement == "Hexahedra") return vtkSmartPointer<vtkHexahedron>::New();
        else if (meshElement == "Pyramid")
            return vtkSmartPointer<vtkPyramid>::New();

        return vtkSmartPointer<vtkPolyhedron>::New();
    } else {
        if (meshElement == "Linear") return vtkSmartPointer<vtkLine>::New();

        return nullptr;
    }
}