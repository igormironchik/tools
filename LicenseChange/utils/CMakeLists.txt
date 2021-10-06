
project( CodeForWeb )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Widgets REQUIRED )
find_package( Qt6Gui REQUIRED )

set( SRC utils.cpp
    utils.hpp )

add_library( utils ${SRC} )

target_link_libraries( utils Qt6::Widgets Qt6::Gui Qt6::Core )
