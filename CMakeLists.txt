include(FetchContent)

cmake_minimum_required(VERSION 3.15)
project(OCTEngineTestGame C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

FetchContent_Declare(OCTAVIANEngine
        GIT_REPOSITORY https://github.com/elfyndor203/OCTAVIANEngine.git
        GIT_TAG main
)
FetchContent_MakeAvailable(OCTAVIANEngine)


add_executable(OCTEngineTestGame main.c)
target_link_libraries(OCTEngineTestGame PRIVATE OCTAVIANEngine)
target_compile_definitions(OCTEngineTestGame PRIVATE GAME_CWD="${CMAKE_CURRENT_SOURCE_DIR}")

