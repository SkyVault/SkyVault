#include "particle_emitter_filter.h"

void ParticleEmitterFilter::Update(const SkyTime& time, Entity* entity) {

    auto emitter = entity->Get<ParticleEmitter>();
    auto body = entity->Get<Body>();

    if (emitter->timer <= 0) {
        auto number = rand_int(emitter->ParticleNumber.x, emitter->ParticleNumber.y);

        for (int i = 0; i < number; i++) {
            // Spawn the particle

            auto dir = rand_float() * 2 * 3.1415926;
            auto xpos = cos(dir) * (emitter->Radius * rand_float());
            auto ypos = sin(dir) * (emitter->Radius * rand_float());

            auto particle = std::unique_ptr<Particle>(new Particle);

            particle->Size = sf::Vector2f(10, 10);
            particle->Position = body->Center() + sf::Vector2f(xpos, ypos);
            particle->Life =
                emitter->LifeRange.x +
                (rand_float() * (emitter->LifeRange.x + emitter->LifeRange.y));

            particles.push_back(std::move(particle));
        }

        emitter->timer = emitter->MaxTime;
    }

    emitter->timer -= time.dt;

    // Update the particles
}

void ParticleEmitterFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {
    sf::CircleShape shape;

    for (const auto& particle : particles) {
        shape.setPosition(particle->Position);
        shape.setRadius(particle->Size.x);
        shape.setFillColor(particle->Color);

        window->draw(shape);
    }
}
