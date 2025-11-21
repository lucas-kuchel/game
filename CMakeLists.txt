cmake_minimum_required(VERSION 3.21)

project(game LANGUAGES CXX)

function(enable_project_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wdouble-promotion
            -Wshadow
            -Wnonnull
            -Wimplicit-fallthrough
            -Wunused
        )
    endif()
endfunction()

add_subdirectory(game)
add_subdirectory(engine)