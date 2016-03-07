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

SET(ROBOPTIM_CORE_SRC
  loader.cc
  )

ADD_LIBRARY(roboptim-core-config SHARED ${HEADERS} ${ROBOPTIM_CORE_SRC})
SET_TARGET_PROPERTIES(roboptim-core-config PROPERTIES
  COMPILE_FLAGS "-Droboptim_core_config_EXPORTS")
PKG_CONFIG_USE_COMPILE_DEPENDENCY(roboptim-core-config roboptim-core)
PKG_CONFIG_USE_DEPENDENCY(roboptim-core-config yaml-cpp)
