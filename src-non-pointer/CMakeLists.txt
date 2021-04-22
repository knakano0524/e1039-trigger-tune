cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TriggerTuning CXX)

file(GLOB          sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB     dist_headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB non_dist_headers ${PROJECT_SOURCE_DIR}/*LinkDef.h)
list(REMOVE_ITEM dist_headers ${non_dist_headers})

# ROOT dict generation
add_custom_command (
  OUTPUT TriggerTuning_Dict.cc
  COMMAND rootcint
  ARGS -f TriggerTuning_Dict.cc -c
    -I${ROOT_PREFIX}/include/
    ${dist_headers}
    ${PROJECT_SOURCE_DIR}/LinkDef.h
  DEPENDS ${dist_headers} ${PROJECT_SOURCE_DIR}/LinkDef.h
)

# ROOT
find_program(ROOTCONF "root-config")
if(ROOTCONF)
  message("-- Detecting ROOT:    found at ${ROOTCONF}")
else()
  message(FATAL_ERROR "-- Detecting ROOT:    not found")
endif()
execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs   OUTPUT_VARIABLE ROOT_LINK    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I$ENV{OFFLINE_MAIN}/include/ ${ROOT_CFLAGS}")

add_library(TriggerTuning SHARED ${sources} TriggerTuning_Dict.cc)
target_link_libraries(TriggerTuning  fun4all interface_main geom_svc ktracker phg4hit PHPythia8 g4detectors UtilAna SQPrimaryGen)
install(TARGETS TriggerTuning DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_custom_target(install_pcm ALL COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/lib COMMAND cp -up *_rdict.pcm ${CMAKE_INSTALL_PREFIX}/lib)
add_dependencies(install_pcm TriggerTuning)
