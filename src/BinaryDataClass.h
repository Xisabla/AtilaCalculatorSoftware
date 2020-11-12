/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryDataClass.h

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#ifndef ATILACALCULATORSOFTWARE_BINARYDATACLASS_H
#define ATILACALCULATORSOFTWARE_BINARYDATACLASS_H

#include "BinaryDataGiD.h"

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

// TODO:
// - Merge Binary_data & Binary_data_GiD into a single class BinaryData
// - Rework and rename Binary_data_class into VTKResource/ResourceWrapper/BinaryDataWrapper

// Class qui transforme l'objet binaire en objets vtk
class Binary_data_class : public Str_binary_data_GiD {

  public:
    // Construteur utilisant le construteur de la struct Str_binary_data_GiD afin d'utiliser la
    // librairie gidpost Il prépare obj pour qui soit compréhensible par l'utilisateur ou VTK
    Binary_data_class(std::string str);

    ~Binary_data_class();

    // Fonction qui transforme les informations obtenu avec Gidpost en objets VTK
    void setUpGiDtoVTK();

    // Fonction qui attribue les valeur a chaque point de l'objet en fonction du choix de résultat
    // par l'utilisateur
    void setScalarFromQT2(Str_Result& res, const int& choice);

    // Retourne le tableau contenant la valeur de chaque point
    vtkSmartPointer<vtkFloatArray> getScalars() const;

    // Retourne le tableau des points et leurs coordonnées
    vtkSmartPointer<vtkPoints> getvtkPoints() const;

    // Retourne le tableau contenant les formes (triangles, carrés, cube ...)
    vtkSmartPointer<vtkCellArray> getvtkCellArray() const;

    // Retourne la Grid, elle contient les points et les formes en fonction de leur position
    vtkSmartPointer<vtkUnstructuredGrid> getUGrid() const;

    // Retourne la list qui contient les informations sur l'objet binaires
    QStringList getstrList() const;

    // Retourne le chemin vers le fichier binaire
    std::string getPath() const;

    // Fonction qui permet de créer deux fichiers .msh et .res contenant les informations des mesh
    // et des résultats
    void toTextFile();

    // Fonction dictant avec quelle forme les points de l'objet binaire doivent etre reliés
    vtkSmartPointer<vtkCell> createVTKCell(const std::string& mesh_name_, const int& ndim_);

  private:
    vtkSmartPointer<vtkUnstructuredGrid> uGrid;
    std::string pathToFile;
    QStringList strList;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> array;
    vtkSmartPointer<vtkFloatArray> scalars;
};

#endif // ATILACALCULATORSOFTWARE_BINARYDATACLASS_H
