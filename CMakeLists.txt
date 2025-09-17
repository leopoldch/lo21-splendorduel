cmake_minimum_required(VERSION 3.5)

project(lo21-splendor-duel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT}
        -i
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )
endif()

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(lo21-splendor-duel
    main.cpp
    main.h
    ${PROJECT_SOURCES}
)

target_include_directories(lo21-splendor-duel PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/Qt/headers"
    "${CMAKE_SOURCE_DIR}/src/core/headers"
)

target_include_directories(lo21-splendor-duel PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Qt/headers
    ${CMAKE_SOURCE_DIR}/src/core/headers
)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(lo21-splendor-duel PRIVATE nlohmann_json::nlohmann_json Qt6::Widgets Qt6::Core)

