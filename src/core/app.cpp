/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      app.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "core/main_window.h"
#include "logger/logger.h"
#include "version.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>
int main(int argc, char* argv[]) {
    Logger::info("AtilaCalculatorSoftware v", ACS_VERSION_MAJOR, ".", ACS_VERSION_MINOR, ".", ACS_VERSION_PATCH);

    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);

    MainWindow window(argv[1]);
    window.show();

    return QApplication::exec();
}