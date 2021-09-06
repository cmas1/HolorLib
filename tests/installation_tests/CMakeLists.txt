cmake_minimum_required(VERSION 3.12)

project("Holor_Install_Test")

find_package(Holor CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ./installation_tests/src/main.cpp)
target_link_libraries(${PROJECT_NAME} Holor::Holor)