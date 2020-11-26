cmake_minimum_required( VERSION 3.0 )

message("CMAKE LEVEL: DEWS-TESTS >>>>>>>>>>>>>>>>>>>>>>>>>>>")

set( CMAKE_CXX_STANDARD 17 )
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

aux_source_directory( src DEWS_TESTS_SRC_FILES )

add_executable( dewstests ${DEWS_TESTS_SRC_FILES} )

target_include_directories( dewstests PUBLIC inc )
target_include_directories( dewstests PUBLIC ${DEWS_INC})

target_link_libraries( dewstests dews )

message("CMAKE LEVEL: DEWS-TESTS <<<<<<<<<<<<<<<<<<<<<<<<<<")


