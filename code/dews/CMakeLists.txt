cmake_minimum_required( VERSION 3.0 )
message("CMAKE LEVEL: DEWS >>>>>>>>>>>>>>>>>>>>>>>>>>>>")

set( CMAKE_CXX_STANDARD 17 )
set( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}" )

FILE( GLOB_RECURSE DEWS_INC_FILES "inc/*.hpp" )
aux_source_directory( src DEWS_SRC_FILES )

add_library( dews SHARED
    ${DEWS_INC_FILES}
    ${DEWS_SRC_FILES}
)

target_include_directories( dews PUBLIC inc )


message("CMAKE LEVEL: DEWS <<<<<<<<<<<<<<<<<<<<<<<<<<<<")




