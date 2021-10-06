
project( LicenseChange )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Widgets REQUIRED )
find_package( Qt6Gui REQUIRED )

set( SRC main.cpp
    mainwindow.hpp
    mainwindow.cpp
    delayedexecutiontimer.cpp
    opts_dialog.cpp
    proxy.cpp
    textedit.cpp
    worker.cpp
    delayedexecutiontimer.hpp
    opts_dialog.hpp
    proxy.hpp
    textedit.hpp
    worker.hpp
    opts_dialog.ui )

qt6_add_resources( SRC resources.qrc )

if( WIN32 )
	list( APPEND SRC app.rc )
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../utils )

add_executable( LicenseChange ${SRC} )

add_dependencies( LicenseChange utils )

target_link_libraries( LicenseChange utils Qt6::Widgets Qt6::Gui Qt6::Core )
