cmake_minimum_required(VERSION 3.6)
project(OpenGLLearning)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/jack/Dev/OpenGLLearning/Debug")

#Add our findGLM.cmake file to the CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/custom_cmake_files/")

find_package(glfw3 3.1 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    #Make sure to have the proper dependencies installed to build this program.
        # They are listed in README.Linux
    include_directories("/usr/include")
    link_libraries(GLEW ${OPENGL_gl_LIBRARY} glfw)
elseif(APPLE)
    #Make sure you have the correct dependencies installed to build this program
        # They are listed in README.Mac
    set(BREW_DIR [YOUR_BREW_CELLAR_DIRECTORY])
    set(FRAMEWORKS "-framwork Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
    include_directories(${BREW_DIR}/glfw/3.2.1/include/GLFW ${BREW_DIR}/glew/2.0.0/include ${BREW_DIR}/glm/0.9.8.3/include/glm)
    link_libraries(${GLEW} ${glfw3} ${OPEN_gl_LIBRARY} ${FRAMEWORKS})
endif()

set(CMAKE_CXX_STANDARD 11)

#The Debug build is to be built, with error logging.
add_definitions(-DDEBUG)

set(SOURCE_FILES main.cpp Rendering/shaders.cpp Rendering/shaders.h Rendering/ObjModel.cpp Rendering/ObjModel.h Movement/Camera.h Movement/MovementController.h Movement/FixedPointOriginMovement.cpp Movement/FixedPointOriginMovement.h Movement/FreeMovement.cpp Movement/FreeMovement.h Movement/InputController.cpp Movement/InputController.h)
add_executable(OpenGLLearning ${SOURCE_FILES})