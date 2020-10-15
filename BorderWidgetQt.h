#ifndef BorderWidgetQt_H
#define BorderWidgetQt_H

#include <QMainWindow>
#include <vtkSmartPointer.h>

class vtkBorderWidget;

#include "ui_BorderWidgetQt.h"

class BorderWidgetQt : public QMainWindow, private Ui::BorderWidgetQt {
    Q_OBJECT
  public:
    // Constructor/Destructor
    BorderWidgetQt();
    ~BorderWidgetQt() { }

  private:
    vtkSmartPointer<vtkBorderWidget> BorderWidget;
};

#endif