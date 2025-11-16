#---------------------------------------------------

# CMAKE VERSION
# --------------------------------------------------
cmake_minimum_required(VERSION 3.12)

# --------------------------------------------------
# PROJECT NAME AND VERSION
# --------------------------------------------------
project(SoftwareRenderer VERSION 1.0)

# --------------------------------------------------
# GENERATE COMPILATION DATABASE (FOR YCM)
# --------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# BUILD SHARED LIBRARIES
# --------------------------------------------------
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# --------------------------------------------------
# CONFIGURATION HEADER
# --------------------------------------------------
configure_file(include/Config.h.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Config.h)

# --------------------------------------------------
# EXECUTABLE
# --------------------------------------------------
add_executable(SoftwareRenderer
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/canvas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/canvas.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vector.h"
    )

# --------------------------------------------------
# TARGET INCLUDE DIRECTORIES
# --------------------------------------------------
find_package(SDL3 REQUIRED)

if(${SDL3_FOUND})
    message("SDL VERSION : ${SDL3_VERSION}")

    target_include_directories(SoftwareRenderer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${SDL3_INCLUDE_DIRS}")

    target_link_libraries(SoftwareRenderer ${SDL3_LIBRARIES})

    message("SDL Libraries Path: ${SDL3_LIBRARIES}")
    
endif()
# --------------------------------------------------
# SET OUTPUT DIRECTORIES
# --------------------------------------------------
set_target_properties(SoftwareRenderer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")
     
# --------------------------------------------------
# COMPILER FLAGS
# --------------------------------------------------
target_compile_features(SoftwareRenderer PUBLIC cxx_std_20)

# add compiler warning flags just when building this project
# via the BUILD_INTERFACE generator expression
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(gcc_flags "-Wall;-Wextra;-Wshadow;-Wshadow;-Wformat=2")
set(msvc_flags "-W3")

target_compile_options(SoftwareRenderer PRIVATE
   "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>"
   "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>")
