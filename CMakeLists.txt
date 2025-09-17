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

add_executable(lo21-splendor-duel

        main.h
        main.cpp

        classes/card.cpp
        classes/card.h
        classes/Exception.h
        classes/tokens.cpp
        classes/tokens.h
        classes/game.cpp
        classes/game.h
        classes/player.cpp
        classes/player.h
        classes/deck.cpp
        classes/deck.h
        classes/board.cpp
        classes/board.h
        classes/privilege.cpp
        classes/privilege.h
        classes/bag.cpp
        classes/bag.h
        classes/draw.cpp
        classes/draw.h
        classes/json.cpp
        classes/json.h
        classes/history.cpp
        classes/history.h

        Qt_cmake/qt_vue_jeton.h
        Qt_cmake/qt_vue_carte.h
        Qt_cmake/qt_vue_carte.cpp
        Qt_cmake/qt_vue_jeton.cpp
        Qt_cmake/popup_text.h
        Qt_cmake/popup_text.cpp
        Qt_cmake/qt_choicepopup.h
        Qt_cmake/qt_popup_yesno.h
        Qt_cmake/qt_popup_info.h
        Qt_cmake/qt_popup_tirageoupioche.h
        Qt_cmake/qt_popup_joker.cpp
        Qt_cmake/playerinputdialog.cpp
        Qt_cmake/qt_popup_couleur.cpp
        Qt_cmake/mainwindow.cpp
        Qt_cmake/mainwindow.h
        Qt_cmake/qt_plateau.cpp
        Qt_cmake/qt_plateau.h
        Qt_cmake/qt_tirages.cpp
        Qt_cmake/qt_tirages.h
)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(lo21-splendor-duel PRIVATE nlohmann_json::nlohmann_json Qt6::Widgets Qt6::Core)

