# ./external/boost/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.boost.experimental )
set( VERSION 1.0 )
set( DESCRIPTION "Experimental boost header-only lib." )

# Provide target for "boost::exp" library.
add_library( boost::exp UNKNOWN
    IMPORTED GLOBAL )

set_target_properties( boost::exp PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} )
