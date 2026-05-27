add_executable(vector_test tests.cpp)

target_link_libraries(vector_test PRIVATE HPRB GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vector_test)

