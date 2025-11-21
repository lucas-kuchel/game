#include <engine/console.hpp>
#include <engine/matrix.hpp>
#include <engine/subsystem.hpp>
#include <engine/vector.hpp>

auto main() -> int {
    engine::Subsystem subsystem;
    engine::SubsystemResult result = subsystem.create();

    if (result == engine::SubsystemResult::ERROR_ALREADY_INIT) {
        engine::Console::printLine("Failed to initialise subsystem: already initialised");
    }
    else if (result == engine::SubsystemResult::ERROR_INIT_FAILED) {
        engine::Console::printLine("Failed to initialise subsystem: initialisation failed");
    }

    engine::Matrix2x2 mat1;
    engine::Matrix2x2 mat2;
    engine::Matrix2x2 mat3 = mat1 * mat2;

    for (std::size_t n = 0; n < 2; n++) {
        engine::Console::printLine("[{} {}]", mat3[n].x, mat3[n].y);
    }

    return 0;
}