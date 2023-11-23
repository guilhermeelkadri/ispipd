cmake_minimum_required(VERSION 3.0.0)
project(ispipd VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/ip_retrieval.cpp
    src/mqtt_client.cpp
)

# Link against the Mosquitto library
target_link_libraries(${PROJECT_NAME} PRIVATE mosquittopp pthread)

# Include the 'inc' directory for header files
target_include_directories(${PROJECT_NAME} PRIVATE inc)

# # Add a test directory and specify test files
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
