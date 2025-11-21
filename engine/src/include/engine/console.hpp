#pragma once

#include <format>
#include <iostream>

namespace engine {
    class Console {
    public:
        Console() = delete;
        ~Console() = delete;

        Console(const Console&) = delete;
        Console(Console&&) noexcept = delete;

        auto operator=(const Console&) -> Console& = delete;
        auto operator=(Console&&) noexcept -> Console& = delete;

        template <typename T>
        static auto get() -> T {
            T value;

            std::getline(std::cin, value);

            return value;
        }

        template <typename... Args>
        static void print(std::format_string<Args...> message, Args&&... args) noexcept {
            std::cout << std::format(message, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void printLine(std::format_string<Args...> message, Args&&... args) noexcept {
            std::cout << std::format(message, std::forward<Args>(args)...) << '\n';
        }

        static constexpr std::string_view NEWLINE = "\n";
        static constexpr std::string_view TAB = "\t";

        static constexpr std::string_view FG_BLACK = "\033[30m";
        static constexpr std::string_view FG_RED = "\033[31m";
        static constexpr std::string_view FG_GREEN = "\033[32m";
        static constexpr std::string_view FG_YELLOW = "\033[33m";
        static constexpr std::string_view FG_BLUE = "\033[34m";
        static constexpr std::string_view FG_MAGENTA = "\033[35m";
        static constexpr std::string_view FG_CYAN = "\033[36m";
        static constexpr std::string_view FG_WHITE = "\033[37m";

        static constexpr std::string_view FG_BRIGHT_BLACK = "\033[90m";
        static constexpr std::string_view FG_BRIGHT_RED = "\033[91m";
        static constexpr std::string_view FG_BRIGHT_GREEN = "\033[92m";
        static constexpr std::string_view FG_BRIGHT_YELLOW = "\033[93m";
        static constexpr std::string_view FG_BRIGHT_BLUE = "\033[94m";
        static constexpr std::string_view FG_BRIGHT_MAGENTA = "\033[95m";
        static constexpr std::string_view FG_BRIGHT_CYAN = "\033[96m";
        static constexpr std::string_view FG_BRIGHT_WHITE = "\033[97m";

        static constexpr std::string_view BG_BLACK = "\033[40m";
        static constexpr std::string_view BG_RED = "\033[41m";
        static constexpr std::string_view BG_GREEN = "\033[42m";
        static constexpr std::string_view BG_YELLOW = "\033[43m";
        static constexpr std::string_view BG_BLUE = "\033[44m";
        static constexpr std::string_view BG_MAGENTA = "\033[45m";
        static constexpr std::string_view BG_CYAN = "\033[46m";
        static constexpr std::string_view BG_WHITE = "\033[47m";

        static constexpr std::string_view RESET = "\033[0m";
        static constexpr std::string_view BOLD = "\033[1m";
        static constexpr std::string_view DIM = "\033[2m";
        static constexpr std::string_view UNDERLINE = "\033[4m";
        static constexpr std::string_view BLINK = "\033[5m";
        static constexpr std::string_view INVERT = "\033[7m";
        static constexpr std::string_view HIDDEN = "\033[8m";
    };
}