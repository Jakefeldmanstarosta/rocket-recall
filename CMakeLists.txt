cmake_minimum_required(VERSION 3.5)

project(RocketRecallProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    QuizApp.cpp
    QuizApp.h
    quizlistwindow.cpp
    quizlistwindow.h
    quizlistwindow.ui
    quizrowwidget.h

    quizeditwindow.cpp
    quizeditwindow.h
    quizeditwindow.ui
)


qt_add_executable(RocketRecallProject
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    PlayQuizWindow.h
    PlayQuizWindow.cpp
)

target_link_libraries(RocketRecallProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(RocketRecallProject PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RocketRecallProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RocketRecallProject)
endif()
