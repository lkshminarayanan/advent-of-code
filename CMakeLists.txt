#
# Run the following from root of the repo to configure :
#   cmake -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .
#
# The executable names are of format <year>-<day>
#
# To update AOC_SESSION_TOKEN, rerun cmake
#

cmake_minimum_required(VERSION 3.30)

project(AdventOfCode CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(cpputils)

macro(init_solutions year)
  file(GLOB days LIST_DIRECTORIES true ${year}/day*)  # Find all directories starting with "day"
  foreach(daydir ${days})
    if(IS_DIRECTORY ${daydir})  # Check if it's a directory
      file(GLOB DaySources ${daydir}/*.cc)  # Get all .cc files in the directory
      get_filename_component(DayExeName ${daydir} NAME)  # Extract the directory name for the executable
      add_executable(${year}-${DayExeName} ${DaySources})  # Create an executable from the .cc files
      target_link_libraries(${year}-${DayExeName} aocutils)  # Link the "aocutils" library
    endif()
  endforeach()
endmacro()

init_solutions(2023)

# Create symbolic link to compile_commands.json after CMake configuration
execute_process(
  COMMAND ${CMAKE_COMMAND} -E create_symlink
          ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json
)
