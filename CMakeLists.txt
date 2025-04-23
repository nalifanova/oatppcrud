cmake_minimum_required(VERSION 3.28)
project(OatCrud VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#################################################################
option(USE_MYMATH "Use provided math implementation" OFF)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(config.h.in config.h)

#################################################################
# Make libs from src files
set(SOURCE_FILES
    src/component/AppComponent.hpp
    src/component/DatabaseComponent.hpp
    src/component/SwaggerComponent.hpp

    src/controller/StaticController.hpp
    src/controller/UserController.hpp

    src/db/UserDb.hpp
    src/dto/StatusDto.hpp
    src/dto/UserDto.hpp

    src/handler/ErrorHandler.cpp
    src/handler/ErrorHandler.hpp

    src/service/UserService.cpp
    src/service/UserService.hpp
)
set(PROJECT_LIB OatLib)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_FILES})
add_library(${PROJECT_LIB} ${SOURCE_FILES})
target_include_directories(${PROJECT_LIB} PUBLIC src)
#################################################################
# First add executables
add_executable(${PROJECT_NAME}
    main.cpp
)
#################################################################
# add oatpp lib
add_subdirectory(vendors/oatpp-1.3.0)
add_library(oatpp::oatpp ALIAS oatpp)
set(OATPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OATPP_INSTALL OFF CACHE BOOL "" FORCE)
# add oatpp-swagger lib
add_subdirectory(vendors/oatpp-swagger-1.3.0)
add_library(oatpp::oatpp-swagger ALIAS oatpp-swagger)
# add oatpp-sqlite lib
add_subdirectory(vendors/oatpp-sqlite-1.3.0)
add_library(oatpp::oatpp-sqlite ALIAS oatpp-sqlite)

#################################################################
# add libressl 4.0.0
# include(FindPkgConfig) # <-- include pkg-config needed by FindLibreSSL.cmake script
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") # <-- use FindLibreSSL.cmake in /cmake folder
include_directories(${CMAKE_SOURCE_DIR}/vendors/libressl-install/include)
message("libres_include=" "${CMAKE_SOURCE_DIR}/vendors/libressl-install/include")

add_library(LibreSSL::TLS STATIC IMPORTED)
set_target_properties(LibreSSL::TLS PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vendors/libressl-install/lib/libtls.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendors/libressl-install/include"
)
add_library(LibreSSL::SSL STATIC IMPORTED)
set_target_properties(LibreSSL::SSL PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vendors/libressl-install/lib/libssl.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendors/libressl-install/include"
)
add_library(LibreSSL::Crypto STATIC IMPORTED)
set_target_properties(LibreSSL::Crypto PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vendors/libressl-install/lib/libcrypto.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendors/libressl-install/include"
)

#################################################################
# add oatpp-libressl lib
add_subdirectory(vendors/oatpp-libressl-1.3.0)
add_library(oatpp::oatpp-libressl ALIAS oatpp-libressl)

target_link_libraries(${PROJECT_LIB}
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-swagger
    PUBLIC oatpp::oatpp-sqlite
    PUBLIC oatpp::oatpp-libressl

    # LibreSSL libraries
    PUBLIC LibreSSL::TLS
    PUBLIC LibreSSL::SSL
    PUBLIC LibreSSL::Crypto
)
#################################################################
## define certificates path
add_definitions(
    -DCERT_PEM_PATH="${CMAKE_CURRENT_LIST_DIR}/cert/test_key.pem"
    -DCERT_CRT_PATH="${CMAKE_CURRENT_LIST_DIR}/cert/test_cert.crt"
)

set(OATPP_SWAGGER_RES_PATH "${CMAKE_SOURCE_DIR}/vendors/oatpp-swagger-1.3.0/res")
set(DATABASE_MIGRATIONS "${CMAKE_CURRENT_SOURCE_DIR}/sql")
add_definitions(
    ## SQlite db file
    -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite3"
    ## PATH to db migration scripts
    -DDATABASE_MIGRATIONS="${DATABASE_MIGRATIONS}"
    ## define path to swagger-ui statis resources folder
    -DOATPP_SWAGGER_RES_PATH="${OATPP_SWAGGER_RES_PATH}"
)
message("OATPP_SWAGGER_RES_PATH=" ${OATPP_SWAGGER_RES_PATH})
message("DATABASE_MIGRATIONS=" ${DATABASE_MIGRATIONS})
#################################################################
# add the binary tree to the search path for include files
# so that we will find config.h
target_link_libraries(${PROJECT_NAME}
    PUBLIC ${PROJECT_LIB}
)
add_dependencies(${PROJECT_NAME} ${PROJECT_LIB})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
)
