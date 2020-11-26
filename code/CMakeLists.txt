cmake_minimum_required (VERSION 3.0)
message("CMAKE LEVEL: DEWS >>>>>>>>>>>>>>>>>>>>>>>>>")

project(dews)

set( DEWS_INC ${CMAKE_CURRENT_SOURCE_DIR}/dews/inc )

add_subdirectory( dews )
add_subdirectory( dews-tests )

message("CMAKE LEVEL: DEWS <<<<<<<<<<<<<<<<<<<<<<<<<") 


