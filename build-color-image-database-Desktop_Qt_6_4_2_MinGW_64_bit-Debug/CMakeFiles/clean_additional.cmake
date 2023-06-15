# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\color-image-database_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\color-image-database_autogen.dir\\ParseCache.txt"
  "color-image-database_autogen"
  )
endif()
