#include <engine/console.hpp>
#include <engine/matrix.hpp>
#include <engine/subsystem.hpp>
#include <engine/vector.hpp>

auto main() -> int {
    // engine::Subsystem subsystem;
    // engine::SubsystemResult result = subsystem.create();

    // if (result == engine::SubsystemResult::ERROR_ALREADY_INIT) {
    //     engine::Console::printLine("Failed to initialise subsystem: already initialised");
    // }
    // else if (result == engine::SubsystemResult::ERROR_INIT_FAILED) {
    //     engine::Console::printLine("Failed to initialise subsystem: initialisation failed");
    // }

    engine::Matrix mat1 = engine::Matrix4x4f::identity();
    engine::Matrix mat2 = engine::Matrix4x4f::translation({2.0, 1.0, 1.0});
    engine::Matrix mat3 = mat1 * mat2;

    engine::Vector4f mesh[4] = {
        {0.5, 0.5, 0.0, 1.0},
        {-0.5, 0.5, 0.0, 1.0},
        {0.5, -0.5, 0.0, 1.0},
        {-0.5, -0.5, 0.0, 1.0},
    };

    engine::Vector4f transformedMesh[4] = {
        mat3 * mesh[0],
        mat3 * mesh[1],
        mat3 * mesh[2],
        mat3 * mesh[3],
    };

    engine::Console::printLine("Matrix:");

    mat3 = engine::Matrix4x4f::transpose(mat3);

    for (std::size_t c = 0; c < mat3.columns(); c++) {
        engine::Console::printLine("{} {} {} {}", mat3[c][0], mat3[c][1], mat3[c][2], mat3[c][3]);
    }

    mat3 = engine::Matrix4x4f::transpose(mat3);

    engine::Console::printLine("\nOriginal mesh:");

    for (std::size_t i = 0; i < sizeof(mesh) / sizeof(engine::Vector4f); i++) {
        engine::Console::printLine("{}: [{} {} {} {}]", i, mesh[i].x, mesh[i].y, mesh[i].z, mesh[i].w);
    }

    engine::Console::printLine("\nTransformed mesh:");

    for (std::size_t i = 0; i < sizeof(transformedMesh) / sizeof(engine::Vector4f); i++) {
        engine::Console::printLine("{}: [{} {} {} {}]", i, transformedMesh[i].x, transformedMesh[i].y, transformedMesh[i].z, transformedMesh[i].w);
    }

    return 0;
}