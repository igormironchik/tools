
project( CodeForWeb )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Widgets REQUIRED )
find_package( Qt6Gui REQUIRED )

set( SRC main.cpp
    mainwindow.hpp
    mainwindow.cpp )

qt6_add_resources( SRC resources.qrc )

if( WIN32 )
	list( APPEND SRC app.rc )
endif()

add_executable( CodeForWeb ${SRC} )

target_link_libraries( CodeForWeb Qt6::Widgets Qt6::Gui Qt6::Core )
