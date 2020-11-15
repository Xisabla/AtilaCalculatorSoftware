/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      App.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char* argv[]) {
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);

    MainWindow window(argv[1]);
    window.show();

    return QApplication::exec();
}