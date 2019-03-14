cmake_minimum_required(VERSION 3.10)
project(simulator)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
        main.cpp
        windowLogic.cpp windowLogic.h)

if (WIN32) #kompilacja dla windowsa
    #lokalizacja plików SFML na dysku
    set(SFML_LOC "E:/sfml/")

    set(SFML_INCLUDE_DIR "${SFML_LOC}include")
    set(SFML_LIB_DIR "${SFML_LOC}lib")
    include_directories(${SFML_INCLUDE_DIR})
    link_directories(${SFML_LIB_DIR})
    link_libraries("-static")
    add_executable(simulator ${SOURCE_FILES})
    set(LIBRARY_FILES sfml-main-d.a;sfml-system-d.a;sfml-window-d.a;sfml-graphics-d.a;sfml-audio-d.a)
    target_link_libraries(simulator ${LIBRARY_FILES})
endif()
if (UNIX AND NOT APPLE) #kompilacja dla linuxa
    add_executable(simulator ${SOURCE_FILES})
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
    find_package(SFML REQUIRED system window graphics network audio)
    if (SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(simulator ${SFML_LIBRARIES})
    endif()
endif()
