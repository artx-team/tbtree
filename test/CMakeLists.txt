add_executable(test_tbtree test_tbtree.c)
target_link_libraries(test_tbtree ${CUNIT_LIBRARIES})
target_include_directories(test_tbtree BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CUNIT_INCLUDE_DIRS}
)
target_compile_options(test_tbtree PRIVATE ${TBTREE_C_FLAGS})

if(VALGRIND)
    add_test(NAME test_tbtree
             COMMAND ${VALGRIND} ${VALGRIND_OPTIONS} ${CMAKE_CURRENT_BINARY_DIR}/test_tbtree)
else()
    add_test(NAME test_tbtree
             COMMAND test_tbtree)
endif()
