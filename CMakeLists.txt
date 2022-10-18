cmake_minimum_required(VERSION 3.2)

include(ExternalProject)

project(PV112_TermProject_Game)

if(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
endif()

# GLFW library
set(GLFW_INSTALL_DIR "${CMAKE_BINARY_DIR}/glfw")
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include")
set(GLFW_LIBRARIES   "${GLFW_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}")
ExternalProject_Add(glfw
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw"

    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""

    CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/glfw"
        "-DGLFW_BUILD_EXAMPLES=OFF"
        "-DGLFW_BUILD_TESTS=OFF"
        "-DGLFW_BUILD_DOCS=OFF"
        "-DBUILD_SHARED_LIBS=OFF"

    BUILD_BYPRODUCTS ${GLFW_LIBRARIES}
)

if(UNIX)
    set(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${X11_LIBRARIES} ${X11_Xrandr_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB} ${X11_Xxf86vm_LIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

# GLAD library
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include")
add_library(glad "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_INCLUDE_DIR}")
set(GLAD_LIBARIES)
if(UNIX)
    set(GLAD_LIBRARIES "glad" ${CMAKE_DL_LIBS})
else()
    set(GLAD_LIBRARIES glad)
endif()

# Single header libraries
set(SINGLE_HEADER_LIBS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/single_header")

# Configure compiler flags for our project
if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  # using regular Clang or AppleClang or GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "/std:c++17 /permissive- /W3 /EHsc")
endif()

# PV112 Framework
set(FRAMEWORK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/framework/include")
set(FRAMEWORK_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/framework/src")
add_library(framework
	"${FRAMEWORK_INCLUDE_DIR}/utilities.hpp"
	"${FRAMEWORK_SRC_DIR}/utilities.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/program.hpp"
	"${FRAMEWORK_SRC_DIR}/program.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/mesh.hpp"
	"${FRAMEWORK_SRC_DIR}/mesh.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/framebuffer.hpp"
	"${FRAMEWORK_SRC_DIR}/framebuffer.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/texture.hpp"
	"${FRAMEWORK_SRC_DIR}/texture.cpp"
        framework/src/texture_group.cpp)
add_dependencies(framework glfw glad)
target_link_libraries(framework 
	${GLFW_LIBRARIES}
	${GLAD_LIBRARIES}
)
target_include_directories(framework 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/objects"
)

# YOUR PROJECT
add_executable(final
	src/main.cpp
	src/application.cpp
		src/Game/camera.hpp src/Game/camera.cpp src/Game/game.hpp src/Game/game.cpp src/Game/player.hpp src/Game/player.cpp src/Game/object.hpp src/Game/object.cpp src/Game/entity.hpp src/Game/entity.cpp src/Game/map.hpp src/Game/map.cpp src/Game/area.hpp src/Game/area.cpp src/Game/light.hpp src/Game/light.cpp
        framework/include/texture_group.hpp framework/include/debug.hpp src/Game/ui.hpp src/Game/ui.cpp)

add_dependencies(final 
	glfw 
	glad 
	framework
)

target_link_libraries(final 
	${GLFW_LIBRARIES} 
	${GLAD_LIBRARIES} 
	framework
)

target_include_directories(final 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

# You can use configure_file to copy all the files needed to run the application into the build folder.
# That way you don't have to configure the IDE to run the program from inside the source folder.
# configure_file("${CMAKE_CURRENT_SOURCE_DIR}/FILL_IN" "${CMAKE_CURRENT_BINARY_DIR}/FILL_IN" COPYONLY)
#

set(SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
#configure_file(${SHADERS}/draw_lights.vert ${SHADERS_BINARY}/draw_lights.vert COPYONLY)
#configure_file(${SHADERS}/draw_lights.frag ${SHADERS_BINARY}/draw_lights.frag COPYONLY)
#configure_file(${SHADERS}/draw_object.vert ${SHADERS_BINARY}/draw_object.vert COPYONLY)
#configure_file(${SHADERS}/draw_object.frag ${SHADERS_BINARY}/draw_object.frag COPYONLY)
configure_file(${SHADERS}/draw_object_textured.vert ${SHADERS_BINARY}/draw_object_textured.vert COPYONLY)
configure_file(${SHADERS}/draw_object_textured.frag ${SHADERS_BINARY}/draw_object_textured.frag COPYONLY)
configure_file(${SHADERS}/postprocess.vert ${SHADERS_BINARY}/postprocess.vert COPYONLY)
configure_file(${SHADERS}/postprocess.frag ${SHADERS_BINARY}/postprocess.frag COPYONLY)
configure_file(${SHADERS}/ui.vert ${SHADERS_BINARY}/ui.vert COPYONLY)
configure_file(${SHADERS}/ui.frag ${SHADERS_BINARY}/ui.frag COPYONLY)

set(TEXTURES ${CMAKE_CURRENT_SOURCE_DIR}/textures)
set(TEXTURES_BINARY ${CMAKE_CURRENT_BINARY_DIR}/textures)
configure_file(${TEXTURES}/default.png ${TEXTURES_BINARY}/default.png COPYONLY)
configure_file(${TEXTURES}/font.png ${TEXTURES_BINARY}/font.png COPYONLY)

set(OBJECTS ${CMAKE_CURRENT_SOURCE_DIR}/objects)
set(OBJECTS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/objects)
configure_file(${OBJECTS}/test_scene.mtl ${OBJECTS_BINARY}/test_scene.mtl COPYONLY)
configure_file(${OBJECTS}/test_scene.obj ${OBJECTS_BINARY}/test_scene.obj COPYONLY)
configure_file(${OBJECTS}/mansion_wall.obj ${OBJECTS_BINARY}/mansion_wall.obj COPYONLY)


set(MAPS ${CMAKE_CURRENT_SOURCE_DIR}/maps)
set(MAPS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/maps)
configure_file(${MAPS}/map01.map ${MAPS_BINARY}/map01.map COPYONLY)