cmake_minimum_required(VERSION 3.21)

project(game LANGUAGES CXX)

function(enable_project_warnings target)
    if(MSVC)
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

function(enable_project_optimisations target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:/O2 /Ot /Ob2 /DNDEBUG>
        )
    else()
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:-O3 -flto -DNDEBUG>
        )
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Release>:-flto>
        )
    endif()
endfunction()

add_subdirectory(game)
add_subdirectory(spark-engine)
