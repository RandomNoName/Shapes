
cmake_minimum_required(VERSION 3.19)

project("Engine") 

set(imgui_name "imgui-1.91.0")
set(imgui_zip_path "${CMAKE_SOURCE_DIR}/bin/${imgui_name}.zip")
set(imgui_out_path "${CMAKE_BINARY_DIR}/${imgui_name}")

set(glfw_name "glfw-3.3.3")
set(glfw_zip_path "${CMAKE_SOURCE_DIR}/bin/${glfw_name}.zip")
set(glfw_out_path "${CMAKE_BINARY_DIR}/${glfw_name}")


file(ARCHIVE_EXTRACT INPUT ${imgui_zip_path})
file(ARCHIVE_EXTRACT INPUT ${glfw_zip_path})

# glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target") 
add_subdirectory(${glfw_out_path} glfw_binary)

# glad
add_library(glad STATIC ${glfw_out_path}/deps/glad_gl.c )
target_include_directories(glad PUBLIC ${glfw_out_path}/deps/)

# imgui with glfw + opengl3 backend
add_library(imgui STATIC
   ${imgui_out_path}/imgui.cpp
   ${imgui_out_path}/imgui_demo.cpp
   ${imgui_out_path}/imgui_draw.cpp
   ${imgui_out_path}/imgui_tables.cpp
   ${imgui_out_path}/imgui_widgets.cpp
   ${imgui_out_path}/backends/imgui_impl_glfw.cpp
   ${imgui_out_path}/backends/imgui_impl_opengl3.cpp
 )

target_include_directories(imgui PUBLIC 
   ${imgui_out_path}
   ${imgui_out_path}/backends
)

target_link_libraries(imgui glad glfw)

# engine
file(GLOB_RECURSE srcs
   "src/*.h"
   "src/*.cpp"
)

find_package(OpenGL REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${srcs} )

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

target_link_libraries(${CMAKE_PROJECT_NAME} glad imgui ${OPENGL_LIBRARIES})
