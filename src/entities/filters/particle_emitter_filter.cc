#include "particle_emitter_filter.h"

void ParticleEmitterFilter::Update(const SkyTime& time, Entity* entity) {

    auto emitter = entity->Get<ParticleEmitter>();
    auto body = entity->Get<Body>();

    if (emitter->timer <= 0) {
        auto number = rand_int(emitter->ParticleNumber.x, emitter->ParticleNumber.y);

        for (int i = 0; i < number; i++) {
            // Spawn the particle
        }
    }

    emitter->timer -= time.dt;
}

void ParticleEmitterFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {

}
