
project( test.utils )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

find_package( Qt6Core REQUIRED )
find_package( Qt6Widgets REQUIRED )
find_package( Qt6Gui REQUIRED )
find_package( Qt6Test REQUIRED )

set( CMAKE_AUTOMOC ON )

set( SRC main.cpp )
    
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../../utils )

add_executable( test.utils ${SRC} )

add_dependencies( test.utils utils )

target_link_libraries( test.utils utils Qt6::Widgets Qt6::Gui Qt6::Test Qt6::Core )

add_test( NAME test.utils
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.utils
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
