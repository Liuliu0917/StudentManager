cmake_minimum_required(VERSION 3.19)
project(StudentManager LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(StudentManager
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    global.h
    global.cpp
    logindialog.h logindialog.cpp logindialog.ui
    studentmanage.h studentmanage.cpp studentmanage.ui
    coursemanage.h coursemanage.cpp coursemanage.ui
    scoremanage.h scoremanage.cpp scoremanage.ui

)

target_link_libraries(StudentManager
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS StudentManager
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET StudentManager
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})