#include <engine/subsystem.hpp>

#include <GLFW/glfw3.h>

engine::Subsystem::~Subsystem() {
    if (initialised_) {
        initialised_ = false;

        glfwTerminate();
    }
}

auto engine::Subsystem::create() -> SubsystemResult {
    if (initialised_) {
        return SubsystemResult::ERROR_ALREADY_INIT;
    }

    if (glfwInit() != GLFW_TRUE) {
        return SubsystemResult::ERROR_INIT_FAILED;
    }

    initialised_ = true;

    return SubsystemResult::SUCCESS;
}

auto engine::Subsystem::pollEvents() const -> SubsystemResult {
    if (!initialised_) {
        return SubsystemResult::ERROR_LOST_SUBSYSTEM;
    }

    glfwPollEvents();

    return SubsystemResult::SUCCESS;
}

auto engine::Subsystem::awaitEvents() const -> SubsystemResult {
    if (!initialised_) {
        return SubsystemResult::ERROR_LOST_SUBSYSTEM;
    }

    glfwWaitEvents();

    return SubsystemResult::SUCCESS;
}

auto engine::Subsystem::awaitEventsTimeout(double timeout) const -> SubsystemResult {
    if (!initialised_) {
        return SubsystemResult::ERROR_LOST_SUBSYSTEM;
    }

    double start = glfwGetTime();

    glfwWaitEventsTimeout(timeout);

    double end = glfwGetTime();
    double elapsed = end - start;

    constexpr double EPS = 0.0001;

    if (elapsed + EPS > timeout) {
        return SubsystemResult::TIMEOUT;
    }

    return SubsystemResult::SUCCESS;
}