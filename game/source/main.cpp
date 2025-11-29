#include <spark/events/dispatcher.hpp>

#include <spark/ecs/registry.hpp>

#include <print>

struct Mob {
    float health;
};

struct DamageEvent {
    float damage;
};

struct HealEvent {
    float health;
};

class Responder {
public:
    void respond(const DamageEvent& event) {
        std::println("damage: {} [from member function]", event.damage);
    }
};

void reactToDamage(const DamageEvent& damageEvent) {
    std::println("damage: {} [from free function]", damageEvent.damage);
}

void reactToHeal(const HealEvent& healEvent) {
    std::println("heal: {} [from free function]", healEvent.health);
}

int main() {
    std::println("testing dispatcher");

    spark::dispatcher dispatcher;

    spark::sink damageSink = dispatcher.sink<DamageEvent>();
    spark::sink healSink = dispatcher.sink<HealEvent>();

    Responder responder;

    // TEST CASE: multiple callbacks
    damageSink.connect<reactToDamage>();
    healSink.connect<reactToHeal>();
    damageSink.connect<&Responder::respond>(responder);

    dispatcher.trigger<DamageEvent>(70.0f);
    dispatcher.trigger<HealEvent>(30.0f);

    std::println("testing registry");

    spark::registry registry;

    spark::entity player = registry.create();

    registry.emplace<Mob>(player, 100.0f);
}