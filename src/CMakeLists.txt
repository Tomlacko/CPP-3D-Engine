add_executable(PV112_TermProject_Game main.cpp application.cpp)

add_dependencies(PV112_TermProject_Game glfw glad framework)

target_link_libraries(PV112_TermProject_Game ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(PV112_TermProject_Game
		PRIVATE ${GLFW_INCLUDE_DIR}
		PRIVATE ${GLAD_INCLUDE_DIR}
		PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
		PRIVATE ${FRAMEWORK_INCLUDE_DIR}
		)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/draw_light.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/draw_light.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/draw_light.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/draw_light.frag" COPYONLY)