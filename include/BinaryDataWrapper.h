/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryDataClass.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_BINARYDATAWRAPPER_H
#define ATILACALCULATORSOFTWARE_BINARYDATAWRAPPER_H

#include "BinaryData.h"

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
    BinaryDataWrapper(const std::string& file);

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
     * @return The DataSet of points of the loaded and converted GiD resource file
     */
    vtkSmartPointer<vtkUnstructuredGrid> getUnstructuredGrid() const;

    /**
     * @return Observed values of the loaded result
     */
    vtkSmartPointer<vtkFloatArray> getScalars() const;

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
     * @brief Information about the current results
     */
    QStringList informationList;
};

#endif // ATILACALCULATORSOFTWARE_BINARYDATAWRAPPER_H
