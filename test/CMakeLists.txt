include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_BINARY_DIR}/lib)

set(TEST_SRCS main.cpp test_mysql.cpp)
add_executable(test ${TEST_SRCS})
target_link_libraries(test libzdb.a)
target_link_libraries(test mysqlclient pthread z m rt dl)
target_link_libraries(test /usr/lib/libgtest.a)

