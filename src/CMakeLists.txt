file(GLOB_RECURSE SOURCES "impl/*.c" "impl/*.cpp")
file(GLOB_RECURSE HEADERS "heads/*.h" "heads/*.hpp")

add_library(listlib SHARED ${SOURCES} ${HEADERS})