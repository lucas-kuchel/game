#pragma once

#include <spark/events/signal.hpp>

namespace spark {
    template <typename T, typename U = uint64>
    requires(is_unsigned<U>)
    class sink {
    public:
        using event_type = T;
        using size_type = U;

        sink(list<detail::signal_delegate<size_type>, size_type>& signals, size_type index)
            : signals_(&signals), index_(index) {
        }

        ~sink() = default;

        sink(const sink&) = default;
        sink(sink&&) noexcept = default;

        sink& operator=(const sink&) = default;
        sink& operator=(sink&&) noexcept = default;

        template <auto Fn>
        void connect() {
            auto& instance = acquire();

            instance.template connect<Fn>();
        }

        template <auto Fn, typename C>
        void connect(C& caller) {
            auto& instance = acquire();

            instance.template connect<Fn, C>(caller);
        }

        template <auto Fn>
        void disconnect() {
            auto& instance = acquire();

            instance.template disconnect<Fn>();
        }

        template <auto Fn, typename C>
        void disconnect(C& caller) {
            auto& instance = acquire();

            instance.template disconnect<Fn, C>(caller);
        }

        void clear() {
            auto& instance = acquire();

            instance.clear();
        }

    private:
        signal<event_type, size_type>& acquire() {
            auto& filler = (*signals_)[index_].filler;
            auto& instance = *reinterpret_cast<signal<event_type, size_type>*>(&filler);

            return instance;
        }

        list<detail::signal_delegate<size_type>, size_type>* signals_;
        size_type index_;
    };
}