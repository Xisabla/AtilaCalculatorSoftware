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

void sampleLogListener(size_t index,
                       const std::pair<LogMetaData, std::string>& entry,
                       const std::string& formatted,
                       const LogEntries& entries) {
    // std::cout << "Listener: " << index << std::endl;
}

int main(int argc, char* argv[]) {
    Logger::setVerbosity(true);
    Logger::setVerboseLevel(Debug);
    Logger::logToFile("../logs.log");
    Logger::addEntryListener(sampleLogListener);

    Logger::info(
    "AtilaCalculatorSoftware v", ACS_VERSION_MAJOR, ".", ACS_VERSION_MINOR, ".", ACS_VERSION_PATCH);


    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);
    Logger::info("QApplication created");

    MainWindow window(argv[1]);
    window.show();

    static int status = QApplication::exec();

    Logger::info("QApplication terminated.");

    return status;
}