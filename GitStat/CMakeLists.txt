
project( CodeForWeb )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt6Core REQUIRED )
find_package( Qt6Widgets REQUIRED )
find_package( Qt6Gui REQUIRED )

set( SRC main.cpp )

add_definitions( -DARGS_QSTRING_BUILD )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/args-parser )

add_executable( GitStat ${SRC} )

target_link_libraries( GitStat Qt6::Widgets Qt6::Gui Qt6::Core )
