# ----------------------------------------------------------------------------
# минимальная версия CMake, способная переварить этот файл
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(NAME Fractals)
project(${NAME} LANGUAGES CXX)

include_directories("src")
include_directories("SDL2-2.0.12/include")
include_directories("vulkan/include")

# Set preprocessor defines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-std=c++17)
	
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
set(SOURCES src/main.cpp )
add_executable(${NAME} ${SOURCES} ${SHADERS})
target_link_libraries(${NAME} 
					  "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/lib/vulkan-1.lib"
					  "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.12/lib/x64/SDL2.lib"
					  "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.12/lib/x64/SDL2main.lib"
					  "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.12/lib/x64/SDL2_image.lib"
					   )

set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR})	
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${NAME})
