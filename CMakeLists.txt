cmake_minimum_required(VERSION 3.10.2)
project(project1)

#############################
# Linkowanie OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message("ERROR: openGL not found")
endif(NOT OPENGL_FOUND)

# Linkowanie GLEW
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	link_libraries(${GLEW_LIBRARIES})
endif()

# Linkowanie GLM
find_package(glm REQUIRED)
if(NOT glm_FOUND)
	message("ERROR: glm not found")
endif(NOT glm_FOUND)


# dodanie includow (headery)
include_directories(include)

set(GL_LIBRARY GL)

# ustawiamy SOURCES
file(GLOB SOURCES "src/*")

# dodajemy je jako wykonywalne w projecie
add_executable(project1 ${SOURCES})

# linkujemy stuff GLFW, OpenGL, libdl (dla GLAD)
target_link_libraries(project1 glfw ${GL_LIBRARY} ${CMAKE_DL_LIBS} glm)
