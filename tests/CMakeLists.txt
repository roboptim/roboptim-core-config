# Copyright 2016, Benjamin Chrétien, CNRS-AIST JRL
#
# This file is part of roboptim-core.
# roboptim-core is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# roboptim-core is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Lesser Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with roboptim-core.  If not, see <http://www.gnu.org/licenses/>.

# Make sure local headers are found
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/tests)

# Add Boost path to include directories.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Make Boost.Test generates the main function in test cases.
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

# To locate built plug-in.
ADD_DEFINITIONS(-DPLUGIN_PATH="${CMAKE_BINARY_DIR}/src")
ADD_DEFINITIONS(-DTESTS_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# ROBOPTIM_CORE_CONFIG_TEST(NAME)
# -------------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against
# Boost, roboptim-core, and add it to the test suite.
# Complementary source files can be added as extra arguments.
#
MACRO(ROBOPTIM_CORE_CONFIG_TEST NAME)
  ADD_EXECUTABLE(${NAME} ${NAME}.cc ${ARGN})
  ADD_TEST(NAME ${NAME}
    COMMAND ${CHECK_PREFIX} ${CMAKE_CURRENT_BINARY_DIR}/${NAME})

  PKG_CONFIG_USE_DEPENDENCY(${NAME} roboptim-core)
  PKG_CONFIG_USE_DEPENDENCY(${NAME} eigen3)
  PKG_CONFIG_USE_DEPENDENCY(${NAME} liblog4cxx)
  TARGET_LINK_LIBRARIES(${NAME} roboptim-core-config)

  # Link against Boost.
  TARGET_LINK_LIBRARIES(${NAME} ${Boost_LIBRARIES})

  # Make sure the plugins will be found.
  SET_PROPERTY(
    TEST ${NAME} PROPERTY ENVIRONMENT
    "LTDL_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src:$ENV{LTDL_LIBRARY_PATH}")
  SET_PROPERTY(
    TEST ${NAME} PROPERTY ENVIRONMENT
    "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src:$ENV{LD_LIBRARY_PATH}")
ENDMACRO(ROBOPTIM_CORE_CONFIG_TEST)

ROBOPTIM_CORE_CONFIG_TEST(loader)
