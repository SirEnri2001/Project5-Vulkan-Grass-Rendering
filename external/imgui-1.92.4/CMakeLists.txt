# Example usage:
#  mkdir build
#  cd build
#  cmake -g "Visual Studio 14 2015" ..

cmake_minimum_required(VERSION 2.8)
project(imgui_example_glfw_vulkan C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# Dear ImGui
set(IMGUI_DIR .)

# Libraries
find_package(Vulkan REQUIRED)
#find_library(VULKAN_LIBRARY
  #NAMES vulkan vulkan-1)
#set(LIBRARIES "${VULKAN_LIBRARY}")
set(LIBRARIES "Vulkan::Vulkan")

add_library(imgui_lib STATIC 
${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp 
${IMGUI_DIR}/imgui.cpp 
${IMGUI_DIR}/imgui_draw.cpp 
${IMGUI_DIR}/imgui_demo.cpp 
${IMGUI_DIR}/imgui_tables.cpp 
${IMGUI_DIR}/imgui_widgets.cpp)
target_link_libraries(imgui_lib ${LIBRARIES} glfw)
target_include_directories(imgui_lib PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)