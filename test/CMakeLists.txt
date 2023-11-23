# Google Test
# Add Google Test and Google Mock to your project
find_package(GTest REQUIRED)

# Add the test executable and link against gtest
add_executable(test_${PROJECT_NAME} 
    test_ip_retrieval.cpp
    ../src/ip_retrieval.cpp
    ../src/mqtt_client.cpp
)

# Include directories for Google Test and Google Mock
target_include_directories(test_${PROJECT_NAME} PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ../inc
)

# Link against Google Test and Google Mock
target_link_libraries(test_${PROJECT_NAME}
    gtest 
    gmock 
    pthread
    mosquittopp
)

add_dependencies(test_ispipd ispipd)

# Register the test with CTest
include(GoogleTest)
gtest_discover_tests(test_${PROJECT_NAME})
