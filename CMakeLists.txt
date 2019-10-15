# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2019, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(HELICS-BBX VERSION 0.0.2)

# -----------------------------------------------------------------------------
# HELICS-BBX Version number
# -----------------------------------------------------------------------------
set(HELICS-BBX_VERSION_BUILD)
set(HELICS-BBX_DATE "10-15-19")

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
include(CTest)

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


cmake_dependent_option(HELICS-BBX_BUILD_TESTS "Enable the test executables to be built" OFF "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME" OFF)


if(HELICS-BBX_BUILD_TESTS AND BUILD_TESTING)
    
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Prohibit in-source build
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
            "In-source build is not supported. Please, use an empty directory for building the project."
    )
endif()


	include(compiler_flags)
	message(STATUS "setting version build options to \"${CXX_STANDARD_FLAG}\"")
	



cmake_dependent_advanced_option(
    STATIC_STANDARD_LIB
    "Link against a static standard lib"
    OFF
    "NOT USE_LIBCXX;CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME"
    OFF
)
include(ucm)

if(STATIC_STANDARD_LIB)
    ucm_set_runtime(STATIC)
else()
    ucm_set_runtime(DYNAMIC)
endif()

    cmake_dependent_advanced_option(EMBEDDED_DEBUG_INFO "embed debug info into obj files" OFF "MSVC;CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME" OFF)
	if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(MSVC AND EMBEDDED_DEBUG_INFO )
        ucm_set_embedded_debug(EMBEDDED)
    else()
        ucm_set_embedded_debug(EXTERNAL)
    endif()
	endif()

# -------------------------------------------------------------
# add coverage target
# -------------------------------------------------------------
    cmake_dependent_advanced_option(
        HELICS-BBX_TEST_CODE_COVERAGE
        "Build a target for testing code coverage"
        OFF
        "HELICS-BBX_BUILD_TESTS"
        OFF
    )
    if(HELICS-BBX_TEST_CODE_COVERAGE)
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
    endif(HELICS-BBX_TEST_CODE_COVERAGE)


cmake_dependent_option(HELICS-BBX_GENERATE_DOXYGEN_DOC "Generate Doxygen doc target" OFF "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME" OFF)

if(HELICS-BBX_GENERATE_DOXYGEN_DOC)
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
endif(HELICS-BBX_GENERATE_DOXYGEN_DOC)

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
# CMAKE Subdirectories
# -----------------------------------------------------------------------------
set(HELICS_DISABLE_C_SHARED_LIB ON CACHE INTERNAL "")
add_subdirectory(${PROJECT_SOURCE_DIR}/extern/helics)
add_subdirectory(src)

# -----------------------------------------------------------------------------
# Build the tests
# -----------------------------------------------------------------------------

mark_as_advanced(BUILD_TESTING)
if(HELICS-BBX_BUILD_TESTS AND BUILD_TESTING)
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
cmake_dependent_option(
    HELICS-BBX_ENABLE_PACKAGE_BUILD
    "Add projects for making packages and installers for HELICS" OFF "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME" OFF
)

if(ENABLE_PACKAGE_BUILD)
    set(CPACK_PACKAGE_NAME "Helics-BBX")
    set(CPACK_PACKAGE_VENDOR "GMLC")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HELICS-BBX")
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
        "Executables and helper applications for HELICS-BBX"
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
