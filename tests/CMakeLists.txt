find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

SET(TESTS_SOURCES
    listnodedata_tests.cpp
    listnode_tests.cpp
    list_tests.cpp
    main.cpp
)

add_executable(listlibtests ${TESTS_SOURCES})

target_link_libraries(listlibtests PRIVATE GTest::GTest GTest::Main pthread listlib)

add_test(NAME listlibtests COMMAND listlibtests)