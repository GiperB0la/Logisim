cmake_minimum_required(VERSION 3.16)
project(Logisim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    Logisim/Main.cpp
    Logisim/src/CircuitManager.cpp
    Logisim/src/Logisim.cpp
    Logisim/Widgets/src/Button.cpp
)

include_directories(
    Logisim/include
    Logisim/Gates/include
    Logisim/Widgets/include
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SFML 2.6 COMPONENTS system window graphics audio network REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Logisim/Graphics
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Graphics
)