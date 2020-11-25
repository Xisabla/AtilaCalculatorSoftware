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
    // <--
    Logger::info(
    "AtilaCalculatorSoftware v", ACS_VERSION_MAJOR, ".", ACS_VERSION_MINOR, ".", ACS_VERSION_PATCH);
    Logger::warn("Be careful ", "something maybe went wrong");
    Logger::trace("Reading node id=", 5);
    Logger::info("Info here");
    Logger::debug("Debug here");
    Logger::info("Info here");
    Logger::warn("Warn here");
    Logger::error("Error here");
    Logger::fatal("Fatal here");

    for(auto &l : *Logger::getLogs()) std::cout << l.second << std::endl;

    // TODO: Logger Other option

    // ::getLogs()  --> returns an object Logs which extends from std::vector<pair<...>>
    //      ->filter()
    //      ->formatted()      // string vector

    // TODO: Logger

    // Logger::setVerbose(true);

    // Logger::setVerboseLevel(Debug);
    // Logger::setVerboseLevelRange({ Debug, Error });
    // Logger::setVerboseLevels({ Debug, Fatal });

    // Logger::getEntries();        // --> entries vector
    // Logger::getEntries(filter);  // --> entries vector (filter is same as verbose)
    // Logger::getLogs();           // --> vector of formatted logs
    // Logger::getLogs(filter)      // --> vector of formatted logs (filter is same as verbose)

    // Logger::getStream(); // --> get ostringstream, initialize with current entries, enable giving
    // new entry to stream Logger::setStreamFilter(filter); // (filter is same as verbose)
    // Logger::setStreamLogging(true); // Sure? What happens of set on false

    // Logger::logToFile(file); // --> same as getStream but with a file, (file = char*, string or
    // ofstream) Logger::setFileFilter(filter); // ?? (filter is same as verbose)
    // Logger::setFileLogging(true);

    // Logger::init(argc, argv); // --> read from cli arguments all infos:
    //      --verbose=0 --verbose=0,1,4 --verbose=Trace --verbose=Trace,Debug,Fatal -->
    //      setVerbose(Level)
    //      --log=file.txt                                                     --> setFile()

    return 0;
    // -->

    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);

    MainWindow window(argv[1]);
    window.show();

    return QApplication::exec();
}