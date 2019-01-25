#ifndef SKYVAULT_PARTICLE_FILTER
#define SKYVAULT_PARTICLE_FILTER
#endif//SKYVAULT_PARTICLE_FILTER

#include <SFML/Graphics.hpp>

#include "../components/particle_emitter.h"
#include "../components/body.h"
#include "../../skyvault.h"
#include "../filter.h"

struct ParticleEmitterFilter: public Filter {
    ParticleEmitterFilter() : Filter({
            typeid(ParticleEmitter),
            typeid(Body)
            }){}

    void Update(const SkyTime& time, Entity* entity) override;
    void Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) override;
};
