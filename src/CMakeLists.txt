
set(target game)

set(BIRD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bird.png)
# Using configure_file command to generate header file that 
# needs some environment variables hold by CMake
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bird.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/bird.hpp)

add_executable(${target}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${target} 
${CONAN_LIBS} 
)

