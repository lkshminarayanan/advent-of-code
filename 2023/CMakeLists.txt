cmake_minimum_required(VERSION 3.20)

project(AdventOfCode2023 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(utils)

file(GLOB days LIST_DIRECTORIES true day*)
foreach(daydir ${days})
  if(IS_DIRECTORY ${daydir})
    file(GLOB DaySources ${daydir}/*.cc)
    get_filename_component(DayExeName ${daydir} NAME)
    add_executable(${DayExeName} ${DaySources})
    target_link_libraries(${DayExeName} aocutils)
  endif()
endforeach()
