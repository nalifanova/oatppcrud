cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

###################################################################################################
## These variables are passed to oatpp-module-install.cmake script
## use these variables to configure module installation

set(OATPP_THIS_MODULE_NAME oatpp-sqlite) ## name of the module (also name of folders in installation dirs)
set(OATPP_THIS_MODULE_VERSION "1.3.0") ## version of the module (also sufix of folders in installation dirs)
set(OATPP_THIS_MODULE_LIBRARIES oatpp-sqlite) ## list of libraries to find when find_package is called
set(OATPP_THIS_MODULE_TARGETS oatpp-sqlite) ## list of targets to install
set(OATPP_THIS_MODULE_DIRECTORIES oatpp-sqlite) ## list of directories to install

###################################################################################################

project(${OATPP_THIS_MODULE_NAME}
        VERSION ${OATPP_THIS_MODULE_VERSION}
        LANGUAGES C CXX
        ## HOMEPAGE_URL "https://github.com/oatpp/oatpp-sqlite"
        ## DESCRIPTION "Something about sqlite"
)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(OATPP_DIR_SRC "Path to oatpp module directory (sources)")
option(OATPP_DIR_LIB "Path to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib)")
option(OATPP_BUILD_TESTS "Build tests for this module" ON)
option(OATPP_INSTALL "Install module binaries" ON)

set(OATPP_MODULES_LOCATION "INSTALLED" CACHE STRING "Location where to find oatpp modules. can be [INSTALLED|EXTERNAL|CUSTOM]")

###################################################################################################
## SQLite amalgamation

option(OATPP_SQLITE_AMALGAMATION "Use SQLite amalgamation. SQLite amalgamation will be installed together with this module." OFF)

if(OATPP_SQLITE_AMALGAMATION)
    list(APPEND OATPP_THIS_MODULE_LIBRARIES sqlite)
    list(APPEND OATPP_THIS_MODULE_TARGETS sqlite)
    list(APPEND OATPP_THIS_MODULE_DIRECTORIES sqlite)
endif()

###################################################################################################
## get oatpp main module in specified location

set(OATPP_MODULES_LOCATION_INSTALLED INSTALLED)
set(OATPP_MODULES_LOCATION_EXTERNAL EXTERNAL)
set(OATPP_MODULES_LOCATION_CUSTOM CUSTOM)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_INSTALLED)

    message("Finding oatpp in location=INSTALLED")

#    find_package(oatpp ${OATPP_THIS_MODULE_VERSION} REQUIRED)

    get_target_property(OATPP_INCLUDE oatpp::oatpp INTERFACE_INCLUDE_DIRECTORIES)
    message("OATPP_INCLUDE=${OATPP_INCLUDE}")

#    get_target_property(OATPP_TEST_INCLUDE oatpp::oatpp-test INTERFACE_INCLUDE_DIRECTORIES)
#    message("OATPP_TEST_INCLUDE=${OATPP_TEST_INCLUDE}")

elseif(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)

    message("Finding oatpp in location=EXTERNAL")

    include(ExternalProject)

    set(MODULE_WAIT_DEPS ON)

    set(LIB_OATPP_EXTERNAL "lib_oatpp_external")
    ExternalProject_Add(${LIB_OATPP_EXTERNAL}
            GIT_REPOSITORY "https://github.com/oatpp/oatpp.git"
            GIT_TAG origin/master
            CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DOATPP_INSTALL=OFF -DOATPP_BUILD_TESTS=OFF
            INSTALL_COMMAND cmake -E echo "SKIP INSTALL '${LIB_OATPP_EXTERNAL}'"
    )

    ExternalProject_Get_Property(${LIB_OATPP_EXTERNAL} BINARY_DIR)
    set(OATPP_DIR_LIB ${BINARY_DIR}/src)

    ExternalProject_Get_Property(${LIB_OATPP_EXTERNAL} SOURCE_DIR)
    set(OATPP_DIR_SRC ${SOURCE_DIR}/src)

    message("OATPP_DIR_SRC --> '${OATPP_DIR_SRC}'")
    message("OATPP_DIR_LIB --> '${OATPP_DIR_LIB}'")

elseif(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_CUSTOM)

    message("Finding oatpp in location=CUSTOM")

    message("OATPP_DIR_SRC --> '${OATPP_DIR_SRC}'")
    message("OATPP_DIR_LIB --> '${OATPP_DIR_LIB}'")

else()
    message("FATAL_ERROR Unknown location to find oatpp '${OATPP_MODULES_LOCATION}'")
endif()

if(OATPP_DIR_LIB)
    link_directories(${OATPP_DIR_LIB})
endif()

###################################################################################################
## get dependencies

message("\n############################################################################")
message("## ${OATPP_THIS_MODULE_NAME} module. Resolving dependencies...\n")

if (NOT OATPP_SQLITE_AMALGAMATION)

    #In case of vcpkg, specific finding is required because of unofficiality
#    if (VCPKG_TOOLCHAIN)
#        message("\nvcpkg is on\n")
#        find_package(unofficial-sqlite3 CONFIG REQUIRED)
#    else ()
#        find_package(SQLite3 REQUIRED)
#    endif ()

    message("SQLite3_INCLUDE_DIRS=${SQLite3_INCLUDE_DIRS}")
    message("SQLite3_LIBRARIES=${SQLite3_LIBRARIES}")
    message("SQLite3_VERSION=${SQLite3_VERSION}")

endif ()

message("\n############################################################################\n")

###################################################################################################
## define targets

include(cmake/module-utils.cmake)

add_subdirectory("src")

if(OATPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()
