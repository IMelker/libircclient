cmake_minimum_required(VERSION 3.10)

# ---------------------------------------------------------------------------------------
# Start libircclient project
# ---------------------------------------------------------------------------------------
include(cmake/utils.cmake)
include(cmake/ide.cmake)

libircclient_extract_version()

project(libircclient VERSION ${LIBIRCCLIENT_VERSION})
message(STATUS "Build libircclient: ${LIBIRCCLIENT_VERSION}")

include(GNUInstallDirs)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
option(LIBIRCCLIENT_ENABLE_THREADS "Compile with multithread support" OFF)
option(LIBIRCCLIENT_ENABLE_IPV6 "Compile with IPv6 support" OFF)
option(LIBIRCCLIENT_ENABLE_OPENSSL "Compile with OpenSSL support" OFF)
option(LIBIRCCLIENT_ENABLE_TWITCH "Compile with channels sharp prefix" OFF)
option(LIBIRCCLIENT_ENABLE_DEBUG "Compile with debug information" OFF)
if (NOT DEFINED LIBIRCCLIENT_ENABLE_DEBUG)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LIBIRCCLIENT_ENABLE_DEBUG ON)
    endif()
endif()

set(LIBIRCCLIENT_COMPILE_DEFINITIONS "")
set(LIBIRCCLIENT_LINK_LIB "")
if (LIBIRCCLIENT_ENABLE_THREADS)
    set(LIBIRCCLIENT_COMPILE_DEFINITIONS "${LIBIRCCLIENT_COMPILE_DEFINITIONS} -DENABLE_THREADS -D_REENTRANT")
    set(LIBIRCCLIENT_LINK_LIB "${LIBIRCCLIENT_LINK_LIB}pthread")
endif()

if (LIBIRCCLIENT_ENABLE_IPV6)
    set(LIBIRCCLIENT_COMPILE_DEFINITIONS "${LIBIRCCLIENT_COMPILE_DEFINITIONS} -DENABLE_IPV6")
endif()

if (LIBIRCCLIENT_ENABLE_OPENSSL)
    set(LIBIRCCLIENT_COMPILE_DEFINITIONS "${LIBIRCCLIENT_COMPILE_DEFINITIONS} -DENABLE_SSL")
    set(LIBIRCCLIENT_LINK_LIB "${LIBIRCCLIENT_LINK_LIB}crypto ssl")
endif()

if (LIBIRCCLIENT_ENABLE_TWITCH)
    set(LIBIRCCLIENT_COMPILE_DEFINITIONS "${LIBIRCCLIENT_COMPILE_DEFINITIONS} -DENABLE_TWITCH")
endif()

if (LIBIRCCLIENT_ENABLE_DEBUG)
    set(LIBIRCCLIENT_COMPILE_DEFINITIONS "${LIBIRCCLIENT_COMPILE_DEFINITIONS} -DENABLE_DEBUG")
endif()

# ---------------------------------------------------------------------------------------
# Set LIBIRCCLIENT_MASTER_PROJECT to ON if we are building libircclient
# ---------------------------------------------------------------------------------------
# Check if libircclient is being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED LIBIRCCLIENT_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(LIBIRCCLIENT_MASTER_PROJECT ON)
    else()
        set(LIBIRCCLIENT_MASTER_PROJECT OFF)
    endif()
endif()

option(LIBIRCCLIENT_BUILD_ALL "Build all artifacts" OFF)

# build shared option
option(LIBIRCCLIENT_BUILD_SHARED "Build shared library" OFF)

# example options
option(LIBIRCCLIENT_BUILD_EXAMPLE "Build example" ${LIBIRCCLIENT_MASTER_PROJECT})

# testing options
# option(LIBIRCCLIENT_BUILD_TESTS "Build tests" OFF)

# bench options
#option(LIBIRCCLIENT_BUILD_BENCH "Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)" OFF)

# install options
option(LIBIRCCLIENT_INSTALL "Generate the install target" ${LIBIRCCLIENT_MASTER_PROJECT})


message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Library config
# ---------------------------------------------------------------------------------------
if (NOT CMAKE_C_STANDARD_COMPILER)
    set(CMAKE_C_STANDARD_COMPILER gcc)
endif()

if(NOT C_STANDARD)
    set(C_STANDARD 99)
    set(C_STANDARD_REQUIRED ON)
endif()

# ---------------------------------------------------------------------------------------
# Static/Shared library
# ---------------------------------------------------------------------------------------
set(LIBIRCCLIENT_SRCS src/libircclient.c)

if(LIBIRCCLIENT_BUILD_SHARED OR BUILD_SHARED_LIBS)
    add_library(libircclient SHARED ${LIBIRCCLIENT_SRCS} ${LIBIRCCLIENT_ALL_HEADERS})
    target_compile_definitions(libircclient PUBLIC ${LIBIRCCLIENT_COMPILE_DEFINITIONS} LIBIRCCLIENT_SHARED_LIB)
else()
    add_library(libircclient STATIC ${LIBIRCCLIENT_SRCS} ${LIBIRCCLIENT_ALL_HEADERS})
    target_compile_definitions(libircclient PUBLIC ${LIBIRCCLIENT_COMPILE_DEFINITIONS})
endif()

add_library(irc::client ALIAS libircclient)

target_compile_definitions(libircclient PUBLIC LIBIRCCLIENT_COMPILED_LIB)
target_include_directories(libircclient PUBLIC
                            "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# ---------------------------------------------------------------------------------------
# Link third-party libraries
# ---------------------------------------------------------------------------------------
target_link_libraries(libircclient PUBLIC ${LIBIRCCLIENT_LINK_LIB})

set_target_properties(libircclient PROPERTIES VERSION ${LIBIRCCLIENT_VERSION} SOVERSION ${LIBIRCCLIENT_VERSION_MAJOR})
set_target_properties(libircclient PROPERTIES DEBUG_POSTFIX d)

# ---------------------------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------------------------
if(LIBIRCCLIENT_BUILD_EXAMPLE OR LIBIRCCLIENT_BUILD_ALL)
    message(STATUS "Generating example(s)")
    add_subdirectory(examples)
endif()

#if(LIBIRCCLIENT_BUILD_TESTS OR LIBIRCCLIENT_BUILD_ALL)
#    message(STATUS "Generating tests")
#    enable_testing()
#    add_subdirectory(tests)
#endif()

#if(LIBIRCCLIENT_BUILD_BENCH OR LIBIRCCLIENT_BUILD_ALL)
#    message(STATUS "Generating benchmarks")
#    add_subdirectory(bench)
#endif()

# ---------------------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------------------
#if(LIBIRCCLIENT_INSTALL)
#    message(STATUS "Generating install")
#    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/libircclientConfig.cmake.in")
#    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/libircclientConfig.cmake")
#    set(config_targets_file "libircclientConfigTargets.cmake")
#    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/libircclientConfigVersion.cmake")
#    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/libircclient")
#    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
#    set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")

    # ---------------------------------------------------------------------------------------
    # Include files
    # ---------------------------------------------------------------------------------------
#    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
#    install(
#        TARGETS libircclient
#        EXPORT libircclient
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # ---------------------------------------------------------------------------------------
    # Install pkg-config file
    # ---------------------------------------------------------------------------------------
    #get_target_property(PKG_CONFIG_DEFINES spdlog INTERFACE_COMPILE_DEFINITIONS)
    #string(REPLACE ";" " -D" PKG_CONFIG_DEFINES "${PKG_CONFIG_DEFINES}")
    #string(CONCAT PKG_CONFIG_DEFINES "-D" "${PKG_CONFIG_DEFINES}")
    #configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
    #install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

    # ---------------------------------------------------------------------------------------
    # Install CMake config files
    # ---------------------------------------------------------------------------------------
    #install(EXPORT libircclient DESTINATION ${export_dest_dir} NAMESPACE libircclient:: FILE ${config_targets_file})

    #include(CMakePackageConfigHelpers)
    #configure_file("${project_config_in}" "${project_config_out}" @ONLY)

    #write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    #install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

    # ---------------------------------------------------------------------------------------
    # Support creation of installable packages
    # ---------------------------------------------------------------------------------------
    #include(cmake/spdlogCPack.cmake)
#endif()
