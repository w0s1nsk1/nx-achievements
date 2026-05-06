cmake_minimum_required(VERSION 3.16)
project(NXAchievements VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(nxachievements_core STATIC
    core/src/AchievementEngine.cpp
    core/src/ConditionEvaluator.cpp
    core/src/PackLoader.cpp
    core/src/ProfileStore.cpp
)

target_include_directories(nxachievements_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/core/vendor
)

add_executable(nxachievements-test
    tools/nxachievements-test/main.cpp
)
target_link_libraries(nxachievements-test PRIVATE nxachievements_core)

add_executable(nxachievements-tests
    tests/main.cpp
)
target_link_libraries(nxachievements-tests PRIVATE nxachievements_core)

add_executable(pack-validator
    tools/pack-validator/main.cpp
)
target_link_libraries(pack-validator PRIVATE nxachievements_core)
