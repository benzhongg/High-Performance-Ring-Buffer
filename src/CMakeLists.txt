set(SOURCES 
    hpringbuffer.cpp
    )

add_library(HPRB ${SOURCES})

target_include_directories(HPRB PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    )

add_executable(executable main.cpp)
target_link_libraries(executable PRIVATE HPRB)