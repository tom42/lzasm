# MIT License
#
# lzasm: a runtime assembler
# Copyright 2021 Thomas Mathys
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.17.3)
project(lzasm VERSION 0.1.0 LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_MAIN_PROJECT TRUE)
else()
  set(IS_MAIN_PROJECT FALSE)
endif()

if(IS_MAIN_PROJECT)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  include(CTest)
endif()


################################################################################
# Global compiler options
################################################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
  # Passing zero arguments to a variadic macro parameter is allowed in C++20.
  # Clang 12 doesn't appear to know yet, so we manually switch off this warning.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-gnu-zero-variadic-macro-arguments)
  endif()
endif()


################################################################################
# lzasm header only library target
################################################################################

add_library(lzasm INTERFACE)

target_include_directories(
  lzasm INTERFACE 
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(IS_MAIN_PROJECT)
  # Target sources are added to consumers of lzasm.
  # This may not be desired, so only do this when this is the main project.
  set(
    LZASM_SOURCES
    include/lzasm/arm/arm32/divided_thumb_assembler.hpp
    include/lzasm/arm/arm32/detail/basic_types.hpp
    include/lzasm/arm/arm32/detail/immediate.hpp
    include/lzasm/arm/arm32/detail/literal.hpp
    include/lzasm/arm/arm32/detail/object.hpp
    include/lzasm/arm/arm32/detail/operations.hpp
    include/lzasm/arm/arm32/detail/reference.hpp
    include/lzasm/arm/arm32/detail/register_lists.hpp
    include/lzasm/arm/arm32/detail/registers.hpp
    include/lzasm/arm/arm32/detail/symbol.hpp
    include/lzasm/arm/arm32/detail/utilities.hpp)
  target_sources(
    lzasm
    INTERFACE
    $<BUILD_INTERFACE:${LZASM_SOURCES}>)
endif()


################################################################################
# lzasm install and uninstall targets
################################################################################

if(IS_MAIN_PROJECT)

  # Install
  # From https://stackoverflow.com/questions/47718485/install-and-export-interface-only-library-cmake
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/lzasmConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)

  install(
    TARGETS lzasm
    EXPORT lzasmTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime)

  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/lzasmConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/lzasmConfig.cmake"
    INSTALL_DESTINATION lib/cmake/lzasm)

  install(EXPORT lzasmTargets DESTINATION lib/cmake/lzasm)
  install(
    FILES "${PROJECT_BINARY_DIR}/lzasmConfigVersion.cmake"
          "${PROJECT_BINARY_DIR}/lzasmConfig.cmake"
    DESTINATION lib/cmake/lzasm)
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

  # Uninstall
  # From https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#can-i-do-make-uninstall-with-cmake,
  # but modified so that cmake_uninstall.cmake.in is not in the top level source
  # directory but in a cmake subdirectory.
  if(NOT TARGET uninstall)
    configure_file(
      "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
      "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
      IMMEDIATE @ONLY)

    add_custom_target(uninstall
      COMMAND ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/cmake_uninstall.cmake)
  endif()

endif()


################################################################################
# Source package
################################################################################

# See https://cliutils.gitlab.io/modern-cmake/chapters/install/packaging.html
#
# This will use line endings as found in the source files, regardless of the
# build machine and CPack source generator. CPack does not convert line endings
# in any way.
#
# To get a working 'package_source' target on Windows, use CMake's
# 'NMake Makefiles' generator rather than one of the Visual Studio generators.
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES
  /.*build.*
  /.git)
include(CPack)

################################################################################
# Unit tests
################################################################################

if((IS_MAIN_PROJECT OR lzasm_BUILD_TESTING) AND BUILD_TESTING)

  # Search for boost headers only, but not the libraries.
  # We'll compile the unit test framework ourselves as part of the unit tests.
  # This simplifies cross development, since we only
  # need boost headers on build machines.
  find_package(Boost 1.74)

  if(Boost_FOUND)
    message(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost library directories: ${Boost_LIBRARY_DIRS}")
    add_subdirectory(unittest)
  else()
    message(STATUS "Boost not found. Unit tests will not be built")
  endif()
endif()
