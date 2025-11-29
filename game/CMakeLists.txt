file(GLOB_RECURSE GAME_SOURCES "source/*.cpp")

add_executable(game)

target_sources(game PRIVATE ${GAME_SOURCES})

target_include_directories(game PRIVATE
    "source/include"
)

target_link_libraries(game PRIVATE
    spark
)

enable_project_warnings(game)
