#pragma once

#include <spark/events/queue.hpp>
#include <spark/events/signal.hpp>

namespace spark {
    template <typename T = uint64>
    requires(is_unsigned<T>)
    struct family {
        using size_type = T;

        using signal_dummy = signal<size_type, size_type>;
        using signal_filler = filler_of<signal_dummy>;
        using signal_destructor = void (*)(void*);

        using queue_dummy = queue<size_type, size_type>;
        using queue_filler = filler_of<queue_dummy>;
        using queue_destructor = void (*)(void*);

        signal_filler signalFiller;
        signal_destructor signalDestructor = nullptr;

        queue_filler queueFiller;
        queue_destructor queueDestructor = nullptr;
    };
}