cmake_minimum_required(VERSION 2.8)

project(imgui_glfw)

set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT EMSCRIPTEN)
    find_package(glfw3 3.2 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(Freetype REQUIRED)
endif(NOT EMSCRIPTEN)
aux_source_directory(src SRC_FILE)
aux_source_directory(imgui SRC_FILE)
file(GLOB HDR_IMGUI_FILE "imgui/*.h*")
file(GLOB HDR_FILE "include/*.h*")
include_directories(src imgui include imgui/examples/libs/gl3w ${FREETYPE_INCLUDE_DIRS})
add_definitions(-DIMGUI_USER_CONFIG=<imconfig-glfw.h>)
add_library(${PROJECT_NAME} ${SRC_FILE} ${HDR_FILE} ${HDR_IMGUI_FILE})
target_link_libraries(${PROJECT_NAME} debug glfw3d optimized glfw3 ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

install(FILES 
    include/imconfig-glfw.h
    include/imgui-glfw.h
    include/imgui-glfw_export.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imconfig.h
    imgui/misc/cpp/imgui_stdlib.h
    imgui/misc/freetype/imgui_freetype.h
    DESTINATION include
)

install(FILES
    imgui/backends/imgui_impl_opengl3_loader.h
    DESTINATION include/GL
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# if(BUILD_EXAMPLE)
    add_executable(example example/main.cpp)
    set(LIBRARY_DEPT)
    if (APPLE)
        set(LIBRARY_DEPT "-framework  CoreFoundation"
        "-framework  AppKit"
        "-framework  OpenGL"
        "-framework  IOKit"
        "-framework  Cocoa"
        "-framework  Carbon"
        "-framework  CoreVideo"
        "-ObjC")
    endif()
    target_link_libraries(example ${PROJECT_NAME})
    set_property(TARGET example PROPERTY CXX_STANDARD 11)
# endif(BUILD_EXAMPLE)
