# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RocketRecallProject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RocketRecallProject_autogen.dir\\ParseCache.txt"
  "RocketRecallProject_autogen"
  )
endif()
