cmake_minimum_required(VERSION 3.12)
project("Holor" VERSION 1.1
             DESCRIPTION "A C++ header-only template library for multi-dimensional containers."
             HOMEPAGE_URL "https://github.com/cmas1/HolorLib")



#Simple check to verify that the cmake command is not called from the location where the topmost CMakeLists is.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()
           
           

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)


set(HOLOR_CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#====================================================
#               BUILD TYPE
#====================================================
# Set the build type. By default, this value is set to Release and it should not be changed unless required.
# Possible values are None, Debug, Release, RelWithDebInfo, MinSizeRel
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()



#====================================================
#               Various Options
#====================================================
# Set the assertion level, which determines the level of dynamic checks performed when calling the functions from the library, e.g. to verify that the arguments passed to the functions are meaningful
# The possible values are:
#     * no_checks  -> removes all checks
#     * release -> is the recommended setting, which enables only meanningful runtime checks
#     * debug -> this is mainly used for development, to add runtime checks to perform some tests
set(HolorDynamicAssertionLevel "release" CACHE STRING "Dynamic assertions level chosen by the user at CMake configure time")
set_property(CACHE HolorDynamicAssertionLevel PROPERTY STRINGS "release" "no_checks" "debug")
message(STATUS "HolorDynamicAssertionLevel='${HolorDynamicAssertionLevel}'")
add_definitions(-DDEFINE_HOLOR_ASSERT_LEVEL=AssertionLevel::${HolorDynamicAssertionLevel})

#option to install the library
option(HOLOR_INSTALL_LIBRARY "Install the Holor library" ON)
message(STATUS "HOLOR_INSTALL_LIBRARY='${HOLOR_INSTALL_LIBRARY}'")

#option to build the tests (requires gtest)
option(HOLOR_BUILD_TESTS "Build tests " OFF)
message(STATUS "HOLOR_BUILD_TESTS='${HOLOR_BUILD_TESTS}'")

#option to build the benchmarks (requires google benchmarks)
option(HOLOR_BUILD_BENCHMARKS "Build benchmarks " OFF)
message(STATUS "HOLOR_BUILD_BENCHMARKS='${HOLOR_BUILD_BENCHMARKS}'")

#option to build the examples
option(HOLOR_BUILD_EXAMPLES "Build examples " ON)
message(STATUS "HOLOR_BUILD_EXAMPLES='${HOLOR_BUILD_EXAMPLES}'")

#====================================================
#               Installation Paths
#====================================================
#path to install the header files
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}" CACHE PATH "The directory relative to CMAKE_INSTALL_PREFIX where Holor header files are installed")




#path to install the exported cmake package
set(CMAKEPACKAGE_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake" CACHE PATH "The directory relative to CMAKE_INSTALL_PREFIX where HolorConfig.cmake is installed")




#====================================================
#               Library
#====================================================
add_library(${PROJECT_NAME} INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)



#====================================================
#               Build Examples
#====================================================
if(HOLOR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



#====================================================
#               Build Tests Unit
#====================================================
if(HOLOR_BUILD_TESTS)
    add_subdirectory(tests)
endif()



#====================================================
#               Build Benchmarks
#====================================================
if(HOLOR_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()



#====================================================
#               Install Library
#====================================================
if(HOLOR_INSTALL_LIBRARY)
    # locations are provided by GNUInstallDirs
    
    #install library
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    #install headers
    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    #install the export details. The HolorTargets will be located in the Holor namespace. Other CMake
    # targets can refer to it using Holor::HolorTargets.
    install(
        EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
    )


    #create the cmake package to be exported so that other cmake projects can find it easily
    write_basic_package_version_file(
        "${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
  
    configure_package_config_file(
        "${HOLOR_CMAKE_MODULE_PATH}/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKEPACKAGE_INSTALL_DIR}
    )
  

    #install the Config and ConfigVersion of the package
    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION ${CMAKEPACKAGE_INSTALL_DIR})
    
    # Export the Holor package to CMake registry such that it can be easily found by
    # CMake even if it has not been installed to a standard directory.
    export (PACKAGE ${PROJECT_NAME})

    
endif()



#====================================================
#               Uninstall Command
#====================================================
#This adds an uninstall command that looks at all the files from the install_manifest.txt and removes them. To use this command
# go into the build directory and execute $make uninstall
add_custom_target(uninstall 
    COMMAND ${CMAKE_COMMAND} -P ${HOLOR_CMAKE_MODULE_PATH}/HolorUninstall.cmake)

