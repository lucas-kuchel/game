#pragma once

#include <spark/events/signal.hpp>

namespace spark {
    template <typename T = uint64>
    requires(is_unsigned<T>)
    struct family {
        using size_type = T;

        using signal_dummy = signal<size_type, size_type>;
        using signal_filler = filler_of<signal_dummy>;
        using signal_destructor = void (*)(void*);

        using list_dummy = list<size_type, size_type>;
        using list_filler = filler_of<list_dummy>;
        using list_destructor = void (*)(void*);
        using list_clearer = void (*)(void*);

        using dispatch_call = void (*)(void*, void*);

        signal_filler signalFiller;
        signal_destructor destructSignal = nullptr;

        list_filler listFiller;
        list_clearer clearList = nullptr;
        list_destructor destructList = nullptr;

        dispatch_call dispatch = nullptr;
    };
}