#pragma once

#include <cassert>

#include <spark/events/family.hpp>
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
            reset();
        }

        dispatcher(const dispatcher&) = delete;
        dispatcher(dispatcher&&) noexcept = default;

        dispatcher& operator=(const dispatcher&) = delete;
        dispatcher& operator=(dispatcher&&) noexcept = default;

        template <typename U>
        sink<U, size_type> sink() {
            acquireFamily<U>();

            size_type index = indexer_.template index<U>();

            return ::spark::sink<U, size_type>(families_, index);
        }

        template <typename U, typename... Args>
        void trigger(Args&&... args) {
            signal<U, size_type>& instance = acquireSignal<U>();

            U event(forward<Args>(args)...);

            instance.dispatch(event);
        }

        template <typename U, typename... Args>
        void enqueue(Args&&... args) {
            list<U, size_type>& familyList = acquirelist<U>();

            familyList.emplace(forward<Args>(args)...);
        }

        void clear() {
            for (auto& family : families_) {
                void* list = static_cast<void*>(&family.listFiller);

                family.clearList(list);
            }
        }

        void reset() {
            for (auto& delegate : families_) {
                if (delegate.destructSignal != nullptr) {
                    delegate.destructSignal(&delegate.signalFiller);
                }

                if (delegate.destructList != nullptr) {
                    delegate.destructList(&delegate.listFiller);
                }
            }

            indexer_.reset();
            families_.clear();
        }

        void update() {
            for (auto& family : families_) {
                void* list = static_cast<void*>(&family.listFiller);
                void* signal = static_cast<void*>(&family.signalFiller);

                family.dispatch(signal, list);
            }
        }

    private:
        template <typename U>
        family<size_type>& acquireFamily() {
            size_type index = indexer_.template index<U>();

            if (index + 1 > families_.size()) {
                families_.resize(index + 1);
            }

            auto& delegate = families_[index];

            if (delegate.destructSignal == nullptr) {
                new (static_cast<void*>(&delegate.signalFiller)) signal<U, size_type>();

                delegate.destructSignal = [](void* filler) {
                    auto& instance = *reinterpret_cast<signal<U, size_type>*>(filler);

                    instance.~signal<U, size_type>();
                };
            }

            if (delegate.destructList == nullptr) {
                new (static_cast<void*>(&delegate.listFiller)) list<U, size_type>();

                delegate.destructList = [](void* filler) {
                    auto& instance = *reinterpret_cast<list<U, size_type>*>(filler);

                    instance.~list<U, size_type>();
                };

                delegate.clearList = [](void* filler) {
                    auto& instance = *reinterpret_cast<list<U, size_type>*>(filler);

                    instance.clear();
                };
            }

            if (delegate.dispatch == nullptr) {
                delegate.dispatch = [](void* signalFiller, void* listFiller) {
                    auto& signalInstance = *reinterpret_cast<signal<U, size_type>*>(signalFiller);
                    auto& listInstance = *reinterpret_cast<list<U, size_type>*>(listFiller);

                    for (auto& event : listInstance) {
                        signalInstance.dispatch(event);
                    }

                    listInstance.clear();
                };
            }

            return delegate;
        }

        template <typename U>
        signal<U, size_type>& acquireSignal() {
            auto& delegate = acquireFamily<U>();
            auto& acquired = *reinterpret_cast<signal<U, size_type>*>(&delegate.signalFiller);

            return acquired;
        }

        template <typename U>
        list<U, size_type>& acquirelist() {
            auto& delegate = acquireFamily<U>();
            auto& acquired = *reinterpret_cast<list<U, size_type>*>(&delegate.listFiller);

            return acquired;
        }

        type_indexer<size_type> indexer_;
        list<family<size_type>, size_type> families_;
    };
}