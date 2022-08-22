cmake_minimum_required(VERSION 3.16)
project(FaustLLVMTester LANGUAGES CXX)
include(ExternalProject)
include(CTest)

set(OSSIA_SDK "c:/ossia-sdk")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Faust REQUIRED)

ExternalProject_Add(
    faustlibs
    PREFIX ""
    GIT_REPOSITORY https://github.com/grame-cncm/faustlibraries
    GIT_TAG cf12b8bda9bb5513303a11746e50629a78524cd3 
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Add(
    testdsps
    PREFIX ""
    GIT_REPOSITORY https://github.com/ossia/score-user-library
    GIT_TAG master 
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
set(FAUST_LIBS_ROOT "${CMAKE_CURRENT_BINARY_DIR}/faustlibs-prefix/src/faustlibs")
set(FAUST_DSP_ROOT "${CMAKE_CURRENT_BINARY_DIR}/testdsps-prefix/src/testdsps/Presets/Faust")

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  if(MSVC)
    add_compile_options(-EHa)
  else()
    add_compile_options(-fms-extensions)
  endif()
endif()

add_executable(FaustTesterSingle FaustTester.cpp tables/STK/tables.cpp tables/Guitarix/tables.cpp)
add_executable(FaustTesterDouble FaustTester.cpp tables/STK/tables.cpp tables/Guitarix/tables.cpp)

target_compile_definitions(FaustTesterSingle
  PUBLIC
    FAUSTFLOAT=float
    "FAUST_LIBS_ROOT=${FAUST_LIBS_ROOT}"
)
target_compile_definitions(FaustTesterDouble
  PUBLIC
    FAUSTFLOAT=double
    "FAUST_LIBS_ROOT=${FAUST_LIBS_ROOT}"
)
target_link_libraries(FaustTesterSingle
  PUBLIC
    Faust
)
target_link_libraries(FaustTesterDouble
  PUBLIC
    Faust
) 

add_dependencies(FaustTesterSingle faustlibs testdsps)
add_dependencies(FaustTesterDouble faustlibs testdsps)

file(GLOB_RECURSE 
  FAUST_DSPS 
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS 
  RELATIVE "${FAUST_DSP_ROOT}"
  "${FAUST_DSP_ROOT}/*.dsp"
)

foreach(file ${FAUST_DSPS})
  if(NOT "${file}" MATCHES ".*layout.*")
    set(FILENAME "${FAUST_DSP_ROOT}/${file}")
    set(TESTNAME "${file}")
    string(REPLACE ".dsp" "" TESTNAME "${TESTNAME}")
    string(MAKE_C_IDENTIFIER "${TESTNAME}" TESTNAME )
    
    add_test(
      NAME ${TESTNAME}_single
      COMMAND FaustTesterSingle "${FILENAME}"
    )
    set_tests_properties(${TESTNAME}_single PROPERTIES
      FAIL_REGULAR_EXPRESSION "[eE]rror;ERROR;[fF]ail;FAIL;SEGFAULT"
    )
    add_test(
      NAME ${TESTNAME}_double
      COMMAND FaustTesterDouble "${FILENAME}"
    )
    set_tests_properties(${TESTNAME}_double PROPERTIES
      FAIL_REGULAR_EXPRESSION "[eE]rror;ERROR;[fF]ail;FAIL;SEGFAULT"
    )
  endif()
endforeach()