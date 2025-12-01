#include <entt/entt.hpp>
#include <spark/events/dispatcher.hpp>

#include <chrono>
#include <iostream>

std::size_t total_allocated = 0;

void* operator new(std::size_t sz) {
    total_allocated += sz;
    return std::malloc(sz);
}

void operator delete(void* ptr, std::size_t sz) noexcept {
    std::free(ptr);
}

struct DamageEvent {
    float damage;
};

struct HealEvent {
    float health;
};

class Responder {
public:
    void respondDamage(const DamageEvent& event) {
        // std::println("damage: {} [from member function]", event.damage);
    }

    void respondHeal(const HealEvent& event) {
        // std::println("heal: {} [from member function]", event.health);
    }
};

void reactToDamage(const DamageEvent& damageEvent) {
    // std::println("damage: {} [from free function]", damageEvent.damage);
}

void reactToHeal(const HealEvent& healEvent) {
    // std::println("heal: {} [from free function]", healEvent.health);
}

void test_spark_dispatcher() {
    using clock = std::chrono::high_resolution_clock;

    spark::dispatcher dispatcher;
    Responder responder;

    auto damageSink = dispatcher.sink<DamageEvent>();
    auto healSink = dispatcher.sink<HealEvent>();

    // Connect callbacks
    damageSink.connect<&Responder::respondDamage>(responder);
    damageSink.connect<reactToDamage>();
    healSink.connect<&Responder::respondHeal>(responder);
    healSink.connect<reactToHeal>();

    constexpr int N = 1'000'000;

    // --- Immediate dispatch (trigger) ---
    {
        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            dispatcher.trigger<DamageEvent>(float(i));
        }

        auto end = clock::now();
        std::cout << "[Spark Immediate trigger] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }

    // --- FIFO dispatch (enqueue + update) ---
    {
        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            dispatcher.enqueue<DamageEvent>(float(i));
            dispatcher.enqueue<HealEvent>(float(i / 10.f));
        }

        dispatcher.update();

        auto end = clock::now();
        std::cout << "[Spark FIFO enqueue+update] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }

    auto fn = [](const DamageEvent& e) { volatile auto x = e.damage; };

    // --- Multi-listener fan-out ---
    {
        for (int i = 0; i < 8; ++i) {
            damageSink.connect<fn>();
        }

        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            dispatcher.enqueue<DamageEvent>(float(i));
        }

        dispatcher.update();

        auto end = clock::now();
        std::cout << "[Spark Multi-listener fan-out] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }

    // --- Multi-type interleaved FIFO ---
    {
        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            if (i % 3 == 0)
                dispatcher.enqueue<DamageEvent>(float(i));
            else if (i % 3 == 1)
                dispatcher.enqueue<HealEvent>(float(i / 2.f));
            else
                dispatcher.enqueue<DamageEvent>(float(i * 0.5f));
        }

        dispatcher.update();

        auto end = clock::now();
        std::cout << "[Spark Multi-type interleaved FIFO] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }
}

void test_entt_dispatcher() {
    using clock = std::chrono::high_resolution_clock;

    entt::dispatcher dispatcher;
    Responder responder;

    auto damageSink = dispatcher.sink<DamageEvent>();
    auto healSink = dispatcher.sink<HealEvent>();

    // Connect callbacks
    damageSink.connect<&Responder::respondDamage>(responder);
    damageSink.connect<reactToDamage>();
    healSink.connect<&Responder::respondHeal>(responder);
    healSink.connect<reactToHeal>();

    constexpr int N = 1'000'000;

    // --- Immediate dispatch (trigger) ---
    {
        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            dispatcher.trigger<DamageEvent>(float(i));
        }

        auto end = clock::now();
        std::cout << "[EnTT Immediate trigger] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }

    // --- FIFO dispatch (enqueue + update) ---
    {
        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            dispatcher.enqueue<DamageEvent>(float(i));
            dispatcher.enqueue<HealEvent>(float(i / 10.f));
        }

        dispatcher.update();

        auto end = clock::now();
        std::cout << "[EnTT FIFO enqueue+update] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }

    auto fn = [](const DamageEvent& e) { volatile auto x = e.damage; };

    // --- Multi-listener fan-out ---
    {
        for (int i = 0; i < 8; ++i) {
            damageSink.connect<fn>();
        }

        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            dispatcher.enqueue<DamageEvent>(float(i));
        }

        dispatcher.update();

        auto end = clock::now();
        std::cout << "[EnTT Multi-listener fan-out] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }

    // --- Multi-type interleaved FIFO ---
    {
        auto start = clock::now();

        for (int i = 0; i < N; ++i) {
            if (i % 3 == 0)
                dispatcher.enqueue<DamageEvent>(float(i));
            else if (i % 3 == 1)
                dispatcher.enqueue<HealEvent>(float(i / 2.f));
            else
                dispatcher.enqueue<DamageEvent>(float(i * 0.5f));
        }

        dispatcher.update();

        auto end = clock::now();
        std::cout << "[EnTT Multi-type interleaved FIFO] "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }
}

int main() {
    using clock = std::chrono::high_resolution_clock;
    auto totalStart = clock::now();

    std::println("testing spark::dispatcher");
    auto sparkStart = clock::now();
    test_spark_dispatcher();
    auto sparkEnd = clock::now();
    std::println("[Spark total allocations]: {} MB", static_cast<float>(total_allocated) / 1048576.0f);
    std::cout << "[Spark total test time] "
              << std::chrono::duration<double, std::milli>(sparkEnd - sparkStart).count()
              << " ms\n";

    total_allocated = 0;

    std::println("testing entt::dispatcher");
    auto enttStart = clock::now();
    test_entt_dispatcher();
    auto enttEnd = clock::now();
    std::println("[EnTT total allocations]: {} MB", static_cast<float>(total_allocated) / 1048576.0f);
    std::cout << "[EnTT total test time] "
              << std::chrono::duration<double, std::milli>(enttEnd - enttStart).count()
              << " ms\n";

    auto totalEnd = clock::now();
    std::cout << "[Total execution time] "
              << std::chrono::duration<double, std::milli>(totalEnd - totalStart).count()
              << " ms\n";
}