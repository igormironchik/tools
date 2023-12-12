
cmake_minimum_required( VERSION 3.1 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( tools )

enable_testing()

set( BUILD_WIDGETS_EXAMPLES OFF CACHE INTERNAL "" FORCE )
add_subdirectory( 3rdparty/widgets )

add_subdirectory( CodeForWeb )
add_subdirectory( GitStat )
add_subdirectory( LicenseChange )
add_subdirectory( Magnifier )
add_subdirectory( PhraseGen )
