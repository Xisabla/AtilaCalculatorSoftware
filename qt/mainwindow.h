#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <QStringList>
#include <QStringListModel>
#include <vtkOrientationMarkerWidget.h>
#include <vtkNamedColors.h>

#include "ui_mainwindow.h"
#include "binary_data_class.hpp"

//Class gérant QT/VTK et lie l'objet binaire 
class MainWindow : public QMainWindow , public Ui::MainWindow
{
    Q_OBJECT
public:

    //Objet contenant tous les informations de l'objet binaire, ainsi que les méthodes en lien avec VTK
    mutable Binary_data_class *binary = NULL;

    //Constructeur initialise les nécessaires à l'ouverture de l'interface
    MainWindow(char *c);
    
    ~MainWindow();
public slots:

    //Fonction liée au bouton(l'action) Exit, elle permet de fermer l'interface  
    void slotExit();

    //Fonction liée au bouton To Text, elle permet de créer des fichiers .msh et .res, pouvant etre utiles à la compréhension du projet
    void slotToText();

    //Fonction liée au bouton Open File, elle ouvre une fenetre permettant de préciser dans quel dossier se trouve le fichier binaire
    //La fonction initialise l'objet binaire, l'onglet Results, les outils permettants la manipulation du rendu obj/VTK 
    void slotOpenFile();

    //Fonction liée au bouton Reset Camera, elle remet la caméra à sa position d'origine
    void slotResetCamera(); 

    //Fonction faisant le lien entre le choix de résultat par l'utilisateur et l'affichage
    void slotResult(Str_Result& res,const int& choice );

    //Fonction permettant de zoomer sur une zone selectionnée 
    void slotZoomArea(); 

    //Fonction permettant d'interagir avec l'objet affiché 
    void slotInteractObj();

private : 

    QString objDirectory ;

    //Permet de faire le lien entre les axes xyz et l'espace
    vtkSmartPointer<vtkOrientationMarkerWidget> widget;

    //Permet l'affichage des informations sur l'interface 
    QStringListModel *model;

    //Initialise les axes 
    void setAxes(vtkSmartPointer<vtkNamedColors> &colors);

    //Fonction faisant le lien entre l'affichage dans la QVTKOpenWidget et l'objet binaire
    //Initialise et affiche la bar scalar, celle ci permet de connaitre la valeur des points en fonction de couleur
    void setVTK( Str_Result &res,const int& choice );
};

#endif // MAINWINDOW_H
