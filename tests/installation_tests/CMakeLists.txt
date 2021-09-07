cmake_minimum_required(VERSION 3.12)

project("Holor_Install_Test")

find_package(Holor REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} ./src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Holor::Holor)