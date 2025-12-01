file(GLOB_RECURSE GAME_SOURCES "source/*.cpp")

include(FetchContent)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.16.0
)

FetchContent_MakeAvailable(entt)

add_executable(game)

target_sources(game PRIVATE ${GAME_SOURCES})

target_include_directories(game PRIVATE
    "source/include"
)

target_link_libraries(game PRIVATE
    spark
    EnTT::EnTT
)

enable_project_warnings(game)
