cmake_minimum_required(VERSION 2.8)

project(imgui_glfw)

set(CMAKE_CXX_EXTENSIONS OFF)

# Game section
find_package(glfw3 3.3 REQUIRED)
aux_source_directory(src SRC_FILE)
aux_source_directory(imgui SRC_FILE)
file(GLOB HDR_FILE "imgui/*.h*")
include_directories(src imgui)
add_definitions(-DIMGUI_USER_CONFIG=<imconfig-glfw.h>)
add_library(${PROJECT_NAME} ${SRC_FILE} ${HDR_FILE} glfw)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

INSTALL(FILES 
    include/imconfig-glfw.h
    include/imgui-glfw.h
    include/imgui-glfw_export.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imconfig.h
    DESTINATION include
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
