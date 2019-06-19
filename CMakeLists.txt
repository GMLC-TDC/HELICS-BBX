#
# Copyright © 2017-2019, Battelle Memorial Institute; Lawrence Livermore National
# Security, LLC; Alliance for Sustainable Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause
#

cmake_minimum_required(VERSION 3.6)
cmake_policy(VERSION 3.6)

project(HELICS-BBX VERSION 0.0.1)

# -----------------------------------------------------------------------------
# HELICS-BBX Version number
# -----------------------------------------------------------------------------
set(HELICS-BBX_VERSION_BUILD)
set(HELICS-BBX_DATE "03-10-19")

set(
    HELICS-BBX_VERSION_UNDERSCORE
    "${HELICS-BBX_VERSION_MAJOR}_${HELICS-BBX_VERSION_MINOR}_${HELICS-BBX_VERSION_PATCH}"
)
if(HELICS-BBX_VERSION_BUILD)
    set(HELICS-BBX_VERSION "${HELICS-BBX_VERSION}-${HELICS-BBX_VERSION_BUILD}")
    set(
        HELICS-BBX_VERSION_UNDERSCORE
        "${HELICS-BBX_VERSION_UNDERSCORE}-${HELICS-BBX_VERSION_BUILD}"
    )
endif()
set(HELICS-BBX_VERSION_STRING "${HELICS-BBX_VERSION} (${HELICS-BBX_DATE})")

# -----------------------------------------------------------------------------
# set the module path and include some common macros
# -----------------------------------------------------------------------------
set(
    CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${PROJECT_SOURCE_DIR}/config/cmake/"
)
include(extraMacros)
include(CMakeDependentOption)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(HELICS-BBX_CUR_PROJ ON)
else()
    set(HELICS-BBX_CUR_PROJ OFF)
endif()
# include(BuildType)
# -----------------------------------------------------------------------------
# set the install path to a local directory
# -----------------------------------------------------------------------------
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(WIN32)
        if(MSYS)
            set(
                CMAKE_INSTALL_PREFIX
                "/usr/local/HELICS-BBX_${HELICS-BBX_VERSION_UNDERSCORE}/"
                CACHE PATH "default install path"
                FORCE
            )
        else(MSYS)
            set(
                CMAKE_INSTALL_PREFIX
                "C:/local/HELICS-BBX_${HELICS-BBX_VERSION_UNDERSCORE}/"
                CACHE PATH "default install path"
                FORCE
            )
        endif(MSYS)
    endif(WIN32)
endif()

show_variable(
    AUTOBUILD_INSTALL_PATH
    PATH
    "location to install the autobuild libraries and Headers"
    "${PROJECT_BINARY_DIR}/libs"
)

mark_as_advanced(AUTOBUILD_INSTALL_PATH)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/logs)
if (HELICS-BBX_CUR_PROJ)
if (DEFINED BUILD_TESTING)
option(BUILD_HELICS-BBX_TESTS "Enable the test executables to be built" ${BUILD_TESTING})
else()
option(BUILD_HELICS-BBX_TESTS "Enable the test executables to be built" ON)
endif()

else()
endif()
# enable testing
if(BUILD_HELICS-BBX_TESTS)
    include(CTest)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prohibit in-source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(
        FATAL_ERROR
            "In-source build is not supported. Please, use an empty directory for building the project."
    )
endif()


	include(compiler_flags)
	message(STATUS "setting version build options to \"${CXX_STANDARD_FLAG}\"")
	


 if (HELICS-BBX_CUR_PROJ)
cmake_dependent_option(
    STATIC_STANDARD_LIB
    "Link against a static standard lib"
    OFF
    "NOT USE_LIBCXX"
    OFF
)
include(ucm)
mark_as_advanced(STATIC_STANDARD_LIB)
if(STATIC_STANDARD_LIB)
    ucm_set_runtime(STATIC)
else()
    ucm_set_runtime(DYNAMIC)
endif()

if(MSVC)
    option(EMBEDDED_DEBUG_INFO "embed debug info into obj files" OFF)
    mark_as_advanced(EMBEDDED_DEBUG_INFO)
    if(EMBEDDED_DEBUG_INFO)
        ucm_set_embedded_debug(EMBEDDED)
    else()
        ucm_set_embedded_debug(EXTERNAL)
    endif()
endif(MSVC)
endif (HELICS-BBX_CUR_PROJ)
# -------------------------------------------------------------
# add coverage target
# -------------------------------------------------------------
if(BUILD_HELICS-BBX_TESTS)
    cmake_dependent_option(
        TEST_CODE_COVERAGE
        "Build a target for testing code coverage"
        OFF
        "BUILD_HELICS-BBX_TESTS"
        OFF
    )
    if(TEST_CODE_COVERAGE)
        if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
            include(CodeCoverage)

            set(
                COVERAGE_EXCLUDES
                'usr/*'
                'dependencies/*'
                'ThirdParty/*'
                'tests/*'
                'interfaces/*'
                'examples/*'
            )
            setup_target_for_coverage(
                NAME
                HELICS-BBX_coverage # New target name
                EXECUTABLE
                CTest # Executable in PROJECT_BINARY_DIR
            )
        else()
            message(
                FATAL_ERROR
                    "CMAKE_BUILD_TYPE must be set to Coverage for testing code coverage"
            )
        endif()
    endif(TEST_CODE_COVERAGE)
endif()
mark_as_advanced(TEST_CODE_COVERAGE)



option(GENERATE_DOXYGEN_DOC "Generate Doxygen doc target" OFF)

if(GENERATE_DOXYGEN_DOC)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)

        show_variable(
            DOXYGEN_OUTPUT_DIR
            PATH
            "location to put Doxygen docs"
            "${PROJECT_BINARY_DIR}/docs"
        )
        configure_file(
            ${HELICS-BBX_CURRENT_SOURCE_DIR}/config/Doxyfile.in
            ${HELICS-BBX_CURRENT_BINARY_DIR}/Doxyfile @ONLY
        )
        add_custom_target(
            doc ${DOXYGEN_EXECUTABLE} ${HELICS-BBX_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${DOXYGET_OUTPUT_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    endif(DOXYGEN_FOUND)
endif(GENERATE_DOXYGEN_DOC)

# if(MINGW) add_definitions(-DBOOST_USE_WINDOWS_H)
# add_definitions(-DWIN32_LEAN_AND_MEAN) endif()

# -------------------------------------------------------------
# Update git submodules
# -------------------------------------------------------------
include(updateGitSubmodules)

include(GNUInstallDirs)

# -------------------------------------------------------------
# setting the RPATH
# -------------------------------------------------------------
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already (but later on when
# installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_MACOSX_RPATH ON)
set(
    CMAKE_BUILD_RPATH
    "./;${AUTOBUILD_INSTALL_PATH}/bin;${AUTOBUILD_INSTALL_PATH}/lib;${AUTOBUILD_INSTALL_PATH}/lib64"
)

# add the automatically determined parts of the RPATH which point to directories
# outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(
    FIND
        CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}" isSystemDir
)
if("${isSystemDir}" STREQUAL "-1")
    list(
        APPEND
            CMAKE_INSTALL_RPATH CMAKE_INSTALL_RPATH
            "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
    )
endif()

list(
    FIND
        CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir
)
if("${isSystemDir}" STREQUAL "-1")
    list(
        APPEND
            CMAKE_INSTALL_RPATH
            "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    )
endif()

# add the local directory to the rpath
list(APPEND CMAKE_INSTALL_RPATH "./")


# -----------------------------------------------------------------------------
# create the fmt header only targets
# -----------------------------------------------------------------------------
#include(addfmt)

# --------------------------------------------------------------
#  Create the target for jsoncpp
# -----------------------------------------------------------
#include(addJsoncpp)
#target_include_directories(
#    HELICS-BBX_base_includes SYSTEM
#    INTERFACE
#    $<TARGET_PROPERTY:jsoncpp_lib,INTERFACE_INCLUDE_DIRECTORIES>
#)

# -----------------------------------------------------------------------------
# CMAKE Subdirectories
# -----------------------------------------------------------------------------
set(BUILD_C_SHARED_LIB OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/helics)
add_subdirectory(src)

# -----------------------------------------------------------------------------
# Build the tests
# -----------------------------------------------------------------------------

if(BUILD_HELICS-BBX_TESTS AND BUILD_TESTING)
    mark_as_advanced(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# -----------------------------------------------------------------------------
# Setup Examples
# -----------------------------------------------------------------------------


#add_subdirectory(docs)

install(
    FILES LICENSE NOTICE
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT libs
)
# -------------------------------------------------------------
# CPack
# -------------------------------------------------------------
option(
    ENABLE_PACKAGE_BUILD
    "Add projects for making packages and installers for HELICS" OFF
)

if(ENABLE_PACKAGE_BUILD)
    set(CPACK_PACKAGE_NAME "Helics")
    set(CPACK_PACKAGE_VENDOR "GMLC")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HELICS")
    set(CPACK_PACKAGE_VERSION "${HELICS-BBX_VERSION}")
    set(CPACK_PACKAGE_VERSION_MAJOR ${HELICS-BBX_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${HELICS-BBX_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${HELICS-BBX_VERSION_PATCH})

    
    if(WIN32)
        set(CPACK_RESOURCE_FILE_LICENSE "${HELICS-BBX_SOURCE_DIR}\\\\LICENSE")
    else(WIN32)
        set(CPACK_RESOURCE_FILE_LICENSE "${HELICS-BBX_SOURCE_DIR}/LICENSE")
    endif(WIN32)

    set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Application")
    set(CPACK_COMPONENT_LIBS_DISPLAY_NAME "Libraries")
    set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "Headers")
    set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime Libraries")


    set(
        CPACK_COMPONENT_APPLICATION_DESCRIPTION
        "Executables and helper applications for HELICS"
    )
    set(
        CPACK_COMPONENT_LIBS_DESCRIPTION
        "Libraries for compiling and linking with HELICS"
    )
    set(
        CPACK_COMPONENT_HEADERS_DESCRIPTION
        "Headers for linking and compiling with HELICS-BBX"
    )

    set(CPACK_COMPONENT_RUNTIME_REQUIRED ON)

    set(
        CPACK_PACKAGE_EXECUTABLES
        "helics_bbx"
        "Helics Black Box Executable"
    )

    if(INSTALL_SYSTEM_LIBRARIES)
        include(InstallRequiredSystemLibraries)
    endif()

    if(WIN32)
        set(
            CPACK_PACKAGE_ICON
            "${HELICS-BBX_SOURCE_DIR}\\\\docs\\\\img\\\\HELICS.ico"
        )
        set(CPACK_NSIS_MUI_ICON "${HELICS-BBX_SOURCE_DIR}/docs/img/HELICS.ico")
        set(CPACK_NSIS_INSTALL_ROOT "C:\\\\local")
        set(
            CPACK_NSIS_URL_INFO_ABOUT
            "https://www.github.com/GMLC-TDC/HELICS"
        )
        set(CPACK_NSIS_HELP_LINK "https://helics.readthedocs.io/en/latest")
        set(CPACK_NSIS_CONTACT "helicsteam@helics.org")
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
		set(CPACK_PACKAGE_INSTALL_DIRECTORY HELICS-BBX_${HELICS-BBX_VERSION_UNDERSCORE})
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${CMAKE_INSTALL_BINDIR})
        set(
            CPACK_NSIS_MENU_LINKS
            "https://www.github.com/GMLC-TDC/HELICS"
            "HELICS Github"
            "https://helics.readthedocs.io/en/latest"
            "Helics Documentation"
            "https://www.helics.org"
            "Helics Web page"
            "https://www.youtube.com/channel/UCPa81c4BVXEYXt2EShTzbcg"
            "TDC YouTube channel"
        )
    else(WIN32)
        set(CPACK_PACKAGE_ICON "${HELICS-BBX_SOURCE_DIR}/docs/img/HELICS.ico")
    endif(WIN32)

    if(APPLE)
        set(CPACK_BUNDLE_NAME "libhelics")
        configure_file(
            "${HELICS-BBX_SOURCE_DIR}/config/Info.plist.in"
            "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" @ONLY
        )
        set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist")
        set(CPACK_BUNDLE_ICON "${HELICS-BBX_SOURCE_DIR}/docs/img/HELICS.ico")
    endif(APPLE)

    set(CPACK_SOURCE_IGNORE_FILES "/Build*/;/build*/;/.git/")
    # THIS LINE MUST BE LAST
    include(CPack)
endif(ENABLE_PACKAGE_BUILD)
