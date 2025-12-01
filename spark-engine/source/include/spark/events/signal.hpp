#pragma once

#include <spark/types/filler.hpp>
#include <spark/types/index.hpp>
#include <spark/types/list.hpp>
#include <spark/types/traits.hpp>

namespace spark {
    // TODO: implement sinks, publish, trigger, and connect
    // GET READY FOR EnTT BENCHMARK
    template <typename T, typename U = uint64>
    requires(is_unsigned<U>)
    class signal {
    public:
        using event_type = T;
        using size_type = U;

        signal() = default;
        ~signal() = default;

        signal(const signal&) = default;
        signal(signal&&) noexcept = default;

        signal& operator=(const signal&) = default;
        signal& operator=(signal&&) noexcept = default;

        template <auto Fn>
        void connect() {
            delegate& instance = acquire();

            construct<Fn>(instance);
        }

        template <auto Fn, typename C>
        void connect(C& caller) {
            delegate& instance = acquire();

            construct<Fn, C>(caller, instance);
        }

        void dispatch(const event_type& event) {
            for (auto& instance : delegates_) {
                if (instance.invoke == nullptr) {
                    continue;
                }

                instance.invoke(instance.instance, &event);
            }
        }

        void clear() {
            delegates_.clear();
            delegateFreeList_.clear();
        }

        template <auto Fn>
        void disconnect() {
            delegate target;

            construct<Fn>(target);
            remove(target);
        }

        template <auto Fn, typename C>
        void disconnect(C& caller) {
            delegate target;

            construct<Fn, C>(caller, target);
            remove(target);
        }

    private:
        struct delegate {
            using invoke_function = void (*)(void*, const void*);

            void* instance = nullptr;

            invoke_function invoke = nullptr;
        };

        void remove(delegate& target) {
            for (size_type i = 0; i < delegates_.size(); i++) {
                delegate& instance = delegates_[i];

                bool sameCall = instance.invoke == target.invoke;
                bool sameInstance = instance.instance == target.instance;

                if (sameCall && sameInstance) {
                    invalidate(instance, i);

                    break;
                }
            }
        }

        delegate& acquire() {
            size_type index = delegates_.size();

            if (!delegateFreeList_.empty()) {
                index = delegateFreeList_.last();
                delegateFreeList_.pop();
            }
            else {
                delegates_.emplace();
            }

            return delegates_[index];
        }

        void invalidate(size_type index) {
            delegate& instance = delegates_[index];

            instance.instance = nullptr;
            instance.invoke = nullptr;

            delegateFreeList_.emplace(index);
        }

        template <auto Fn>
        void construct(delegate& instance) {
            instance.instance = nullptr;
            instance.invoke = invokeFree<Fn>;
        }

        template <auto Fn, typename C>
        void construct(C& caller, delegate& instance) {
            instance.instance = &caller;
            instance.invoke = invokeMember<C, Fn>;
        }

        template <auto Fn>
        static void invokeFree(void*, const void* event) {
            Fn(*static_cast<const event_type*>(event));
        }

        template <typename C, auto Fn>
        static void invokeMember(void* instance, const void* event) {
            auto* object = static_cast<C*>(instance);

            (object->*Fn)(*static_cast<const event_type*>(event));
        }

        list<size_type, size_type> delegateFreeList_;
        list<delegate, size_type> delegates_;
    };
}