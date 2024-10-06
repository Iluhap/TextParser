cmake_minimum_required(VERSION 3.20)
project(TextParser)

set(CMAKE_CXX_STANDARD 20)

# _________Boost_________
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS
        program_options
        REQUIRED
)

find_package(Boost COMPONENTS log log_setup REQUIRED)

include_directories(src/)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
        "src/main.cpp"

        "src/*.h"
        "src/*.cpp"
)

add_executable(text_parser
        ${SRC}
)

target_include_directories(text_parser
        PUBLIC ${Boost_INCLUDE_DIRS}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(text_parser
        Boost::program_options
)
