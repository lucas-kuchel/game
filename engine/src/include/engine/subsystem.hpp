#pragma once

#include <cstdint>

namespace engine {
    enum class SubsystemResult : std::uint8_t {
        ERROR_LOST_SUBSYSTEM,
        ERROR_INIT_FAILED,
        ERROR_ALREADY_INIT,
        TIMEOUT,
        SUCCESS,
    };

    class Subsystem {
    public:
        Subsystem() = default;
        ~Subsystem();

        Subsystem(const Subsystem&) = delete;
        Subsystem(Subsystem&&) = default;

        auto operator=(const Subsystem&) -> Subsystem& = delete;
        auto operator=(Subsystem&&) noexcept -> Subsystem& = default;

        [[nodiscard]] auto pollEvents() const -> SubsystemResult;
        [[nodiscard]] auto awaitEvents() const -> SubsystemResult;
        [[nodiscard]] auto create() -> SubsystemResult;
        [[nodiscard]] auto awaitEventsTimeout(double timeout) const -> SubsystemResult;

    private:
        bool initialised_ = false;
    };
}