
cmake_minimum_required(VERSION 3.30)
project(PBL2_PROJECT CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL2_INCLUDE_DIR ${CMAKE_BINARY_DIR}/SDL2/include)
set(SDL2_LIB_DIR ${CMAKE_BINARY_DIR}/SDL2/lib)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(${PROJECT_NAME}
        main.cpp
        CityMap.cpp
        CityMap.h
        Customer.cpp
        Customer.h
        Warehouse.cpp
        Warehouse.h
        Driver.cpp
        Driver.h
        SDLHelper.cpp
        SDLHelper.h
        SDLInterface.cpp
        SDLInterface.h
        MySelf.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf)

file(COPY ${SDL2_LIB_DIR}/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SDL2_LIB_DIR}/SDL2_ttf.dll DESTINATION ${CMAKE_BINARY_DIR})
