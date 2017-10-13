# Project Information
########################################
cmake_minimum_required(VERSION 3.5.1)
project(TestingGround)

# Project Structure
########################################
set(CMAKE_BINARY_DIR            ${CMAKE_SOURCE_DIR}/bin )
set(EXECUTABLE_OUTPUT_PATH      ${CMAKE_BINARY_DIR}     )

# File Information
########################################
include_directories("${PROJECT_SOURCE_DIR}/source")
set(SRCS 
    source/main.cpp
    source/Application/Application.cpp
    source/Entity/Entity.cpp
    source/Entity/EntityManager.cpp
    source/Components/Component.cpp
    source/EventManagement/EventManager.cpp
    source/Systems/System.cpp
    source/Systems/RenderSystem.cpp
    source/Systems/MovementSystem.cpp
)

# Build Info
########################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_NAME "TestingGround")
add_executable(${EXECUTABLE_NAME} ${SRCS})

# Dependencies
########################################
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# SFML
find_package(SFML 2 REQUIRED graphics window system)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()