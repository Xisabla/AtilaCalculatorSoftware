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
//  BINARY DATA WRAPPER > SETTERS
//  --------------------------------------------------------------------------------------

void BinaryDataWrapper::hideElement(const std::string& element) {
    this->hiddenElements.insert(element);
}

void BinaryDataWrapper::showElement(const std::string& element) {
    if (this->hiddenElements.count(element) != 0) this->hiddenElements.erase(element);
}

void BinaryDataWrapper::toggleElement(const std::string& element) {
    if (this->isElementHidden(element)) {
        this->showElement(element);
    } else {
        this->hideElement(element);
    }
}

void BinaryDataWrapper::showAllElements() { this->hiddenElements = {}; }

//  --------------------------------------------------------------------------------------
//  BINARY DATA WRAPPER > GETTERS
//  --------------------------------------------------------------------------------------

vtkSmartPointer<vtkUnstructuredGrid> BinaryDataWrapper::getUnstructuredGrid() const {
    return this->unstructuredGrid;
}

vtkSmartPointer<vtkFloatArray> BinaryDataWrapper::getScalars() const { return this->scalars; }

std::set<std::string> BinaryDataWrapper::getElements() const { return this->elements; }

std::set<std::string> BinaryDataWrapper::getHiddenElements() const { return this->hiddenElements; }

bool BinaryDataWrapper::isElementHidden(const std::string& element) const {
    return this->hiddenElements.count(element) > 0;
}

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

void BinaryDataWrapper::reload() {
    Logger::info("Reloading points");

    this->convertFromGiD();
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
    this->elements = {};

    for (Mesh& mesh: this->meshes) {
        // Append information
        Logger::trace("Loading mesh information");
        this->loadMeshInformation(mesh);

        // Append element
        if (this->elements.count(mesh.getElementName()) == 0)
            this->elements.insert(mesh.getElementName());

        // Skip if element is hidden
        if (this->hiddenElements.count(mesh.getElementName()) > 0) {
            Logger::warn("Skipped element ", mesh.getElementName());
            continue;
        }

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
