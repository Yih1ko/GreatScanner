# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GreatScanner_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GreatScanner_autogen.dir\\ParseCache.txt"
  "GreatScanner_autogen"
  )
endif()
