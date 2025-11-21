file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")

add_executable(game)

target_sources(game PRIVATE ${GAME_SOURCES})

target_include_directories(game PRIVATE
    "src/include"
)

target_link_libraries(game PRIVATE
    engine
)

enable_project_warnings(game)