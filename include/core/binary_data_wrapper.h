/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      binary_data_wrapper.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_BINARY_DATA_WRAPPER_H
#define ATILACALCULATORSOFTWARE_BINARY_DATA_WRAPPER_H

#include "core/binary_data.h"
#include "logger/logger.h"

#include <QString>
#include <QStringList>
#include <fstream>
#include <string>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkHexahedron.h>
#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolygon.h>
#include <vtkPolyhedron.h>
#include <vtkPyramid.h>
#include <vtkQuad.h>
#include <vtkQuadraticPyramid.h>
#include <vtkQuadraticTriangle.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

/**
 * @class BinaryDataWrapper
 * @brief Wrapper around the BinaryData class that allows to interact with VTK
 */
class BinaryDataWrapper : public BinaryData {
  public:
    /**
     * @param file Path to the res gzFile
     */
    explicit BinaryDataWrapper(const std::string& file);

    /**
     * @brief Empty but needed for unstructuredGrid destruction
     */
    ~BinaryDataWrapper();

    /**
     * @brief Load the given component of the given result, set inner values
     * @param result Result to load
     * @param component Component to load
     */
    void loadResult(Result& result, const int& component);

    /**
     * @brief Re-read points, use to update hidden/shown elements
     */
    void reload();

    /**
     * @brief Prevent an element from being plot
     * @param element Element to prevent from plotting
     */
    void hideElement(const std::string& element);

    /**
     * @brief Set a hidden element to non hidden
     * @param element Element to set
     */
    void showElement(const std::string& element);

    /**
     * @brief Set hidden element to non hidden and non hidden element to shown
     * @param element Element to toggle
     */
    void toggleElement(const std::string& element);

    /**
     * @brief Set all elements to non hidden
     */
    void showAllElements();

    /**
     * @return The DataSet of points of the loaded and converted GiD resource file
     */
    vtkSmartPointer<vtkUnstructuredGrid> getUnstructuredGrid() const;

    /**
     * @return Observed values of the loaded result
     */
    vtkSmartPointer<vtkFloatArray> getScalars() const;

    /**
     * @return A set of the loaded elements
     */
    std::set<std::string> getElements() const;

    /**
     * @return A set of all the hidden elements
     */
    std::set<std::string> getHiddenElements() const;

    /**
     * @return True if the given element is set on hidden
     */
    bool isElementHidden(const std::string& element) const;

    /**
     * @return Information about the current results as a QT usable object
     */
    QStringList getInformationList() const;

  private:
    /**
     * @brief Read information from the mesh and write it to the informationList
     */
    void loadMeshInformation(Mesh& mesh);

    /**
     * @brief Read points from BinaryData values loaded with GiD (meshes)
     */
    void convertFromGiD();

    /**
     * @brief Find and return the good polygon fitting the mesh elements and dimensions
     * @param meshElement Element of the mesh
     * @param dimCount Number of dimensions of the count
     * @return The well fitting polygon as a VTKCell object
     */
    static vtkSmartPointer<vtkCell> getPolygonVTKCell(const std::string& meshElement,
                                                      const unsigned int& dimCount);

    /**
     * @brief DataSet of all the points converted from GiD representation
     */
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid;

    /**
     * @brief Mesh nodes as vtkPoints object
     */
    vtkSmartPointer<vtkPoints> points;

    /**
     * @brief Observed values
     */
    vtkSmartPointer<vtkFloatArray> scalars;

    /**
     * @brief All elements loaded by the mesh
     */
    std::set<std::string> elements;

    /**
     * @brief GiD elements of the mesh that won't be shown
     */
    std::set<std::string> hiddenElements;

    /**
     * @brief Information about the current results
     */
    QStringList informationList;
};

#endif // ATILACALCULATORSOFTWARE_BINARY_DATA_WRAPPER_H
