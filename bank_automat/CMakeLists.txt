cmake_minimum_required(VERSION 3.19)
project(bank_automat LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(bank_automat
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    environment.h environment.cpp
    accountinfo.h accountinfo.cpp accountinfo.ui

    data.h data.cpp data.ui
    choosecard.h choosecard.cpp choosecard.ui
    withdraw.h withdraw.cpp withdraw.ui

    deposit.h deposit.cpp deposit.ui
    transfer.h transfer.cpp transfer.ui
    ApiClient.h
    ApiClient.cpp
)


target_link_libraries(bank_automat
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

include(GNUInstallDirs)

install(TARGETS bank_automat
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET bank_automat
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

