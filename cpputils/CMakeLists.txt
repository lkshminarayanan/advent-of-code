# build the aoc lib
file(GLOB LibSources src/*.cc)
add_library(aocutils SHARED ${LibSources})
target_include_directories(aocutils PUBLIC include)

# Set and create the input local cache directory
set(INPUT_LOCAL_CACHE "${CMAKE_SOURCE_DIR}/input-files" CACHE STRING "Local cache directory for the input files")
file(MAKE_DIRECTORY ${INPUT_LOCAL_CACHE})

# Ensure the AOC_SESSION_TOKEN environment variable is defined
if(DEFINED ENV{AOC_SESSION_TOKEN})
else()
  message(FATAL_ERROR "Please export AOC session value to AOC_SESSION_TOKEN env variable before calling cmake")
endif()

# Generate the configuration file
configure_file(
  generated/config.hh.in
  ${CMAKE_CURRENT_BINARY_DIR}/generated/config.hh
)

# Include generated directory
target_include_directories(aocutils PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Cleanup the cache variable
unset(INPUT_LOCAL_CACHE CACHE)

# Libcurl required for downloading input
find_package(CURL REQUIRED)
target_link_libraries(aocutils PRIVATE ${CURL_LIBRARIES})
target_include_directories(aocutils PRIVATE ${CURL_INCLUDE_DIRS})
