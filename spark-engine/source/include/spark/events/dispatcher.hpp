#pragma once

#include <spark/events/signal.hpp>
#include <spark/events/sink.hpp>

namespace spark {
    template <typename T = uint64>
    requires(is_unsigned<T>)
    class dispatcher {
    public:
        using size_type = T;

        dispatcher() = default;

        ~dispatcher() {
            for (auto& delegate : signals_) {
                if (delegate.destructor != nullptr) {
                    delegate.destructor(&delegate.filler);
                }
            }
        }

        dispatcher(const dispatcher&) = delete;
        dispatcher(dispatcher&&) noexcept = default;

        dispatcher& operator=(const dispatcher&) = delete;
        dispatcher& operator=(dispatcher&&) noexcept = default;

        template <typename U>
        sink<U, size_type> sink() {
            acquireSignal<U>();

            size_type index = indexer_.template index<U>();

            return ::spark::sink<U, size_type>(signals_, index);
        }

        template <typename U, typename... Args>
        void trigger(Args&&... args) {
            signal<U, size_type> instance = acquireSignal<U>();

            instance.dispatch(forward<Args>(args)...);
        }

    private:
        template <typename U>
        signal<U, size_type>& acquireSignal() {
            size_type index = indexer_.template index<U>();

            if (index + 1 > signals_.size()) {
                signals_.resize(index + 1);
            }

            auto& delegate = signals_[index];

            if (delegate.destructor == nullptr) {
                new (static_cast<void*>(&delegate.filler)) signal<U, size_type>();

                delegate.destructor = [](void* filler) {
                    auto& instance = *reinterpret_cast<signal<U, size_type>*>(filler);

                    instance.~signal<U, size_type>();
                };
            }

            auto& acquired = *reinterpret_cast<signal<U, size_type>*>(&delegate.filler);

            return acquired;
        }

        type_indexer<size_type> indexer_;
        list<detail::signal_delegate<size_type>, size_type> signals_;
    };
}