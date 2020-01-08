aux_source_directory(. TEST_SRC)

add_executable(unittest ${TEST_SRC})

# target_compile_definitions(unittest PRIVATE )
target_link_libraries(unittest common jsoncpp gtest_main rt)