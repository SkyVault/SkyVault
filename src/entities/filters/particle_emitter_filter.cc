#include "particle_emitter_filter.h"

using namespace Interpolation;

void ParticleEmitterFilter::Update(const SkyTime& time, Entity* entity) {

    auto emitter = entity->Get<ParticleEmitter>();
    auto body = entity->Get<Body>();

    emitter->VelocityBounds = sf::FloatRect(-10, 10, -5, -30);
    emitter->Radius = 4.0f;
    emitter->ScalerRange = sf::Vector2f(0.02, 0.2);
    emitter->MaxTime = 0.02f;
    emitter->ParticleNumber = sf::Vector2i(2, 6);
    emitter->Color = sf::Color(255, 150, 0);

    if (emitter->timer <= 0) {
        auto number = rand_int(emitter->ParticleNumber.x, emitter->ParticleNumber.y);


        for (int i = 0; i < number; i++) {
            // Spawn the particle

            auto dir = rand_float() * 2 * 3.1415926;
            auto xpos = cos(dir) * (emitter->Radius * rand_float());
            auto ypos = sin(dir) * (emitter->Radius * rand_float());

            auto particle = std::unique_ptr<Particle>(new Particle);

            auto size = emitter->SizeBounds.x +
                (rand_float() * (emitter->SizeBounds.x + emitter->SizeBounds.y));

            particle->Size = sf::Vector2f(size, size);

            particle->Position = body->Center() + sf::Vector2f(xpos, ypos);

            particle->StartingLife = particle->Life =
                emitter->LifeRange.x +
                (rand_float() * (emitter->LifeRange.x + emitter->LifeRange.y));

            auto velX = emitter->VelocityBounds.left +
                (rand_float() * (emitter->VelocityBounds.top * 2.0f));

            auto velY = emitter->VelocityBounds.width +
                (rand_float() * (emitter->VelocityBounds.height * 2.0f));

            particle->Velocity = sf::Vector2f(velX, velY);
            particle->Color = emitter->Color;
            particle->TargetColor = sf::Color::White;

            particle->Scaler = emitter->ScalerRange.x +
                (rand_float() * (emitter->ScalerRange.y * 2.0f));

            particles.push_back(std::move(particle));
        }

        emitter->timer = emitter->MaxTime;
    }

    emitter->timer -= time.dt;

    // Update the particles

    auto it = particles.begin();
    while (it != particles.end()) {
        (*it)->Position += (*it)->Velocity * time.dt;
        (*it)->Size *= powf((*it)->Scaler, time.dt);

        float life_percent = 1 - ((*it)->Life / (*it)->StartingLife);

        float r = (float)(*it)->Color.r / 255.0f;
        float g = (float)(*it)->Color.g / 255.0f;
        float b = (float)(*it)->Color.b / 255.0f;

        float tr = (float)(*it)->TargetColor.r / 255.0f;
        float tg = (float)(*it)->TargetColor.g / 255.0f;
        float tb = (float)(*it)->TargetColor.b / 255.0f;

        r = LerpPercent(r, tr, life_percent);
        g = LerpPercent(g, tg, life_percent);
        b = LerpPercent(b, tb, life_percent);

        (*it)->Color = sf::Color((int)(r * 255), (int)(g * 255), (int)(b * 255));

        if ((*it)->Life <= 0.0f) {
            it = particles.erase(it);
            continue;
        }

        (*it)->Life -= time.dt;

        ++it;
    }
}

void ParticleEmitterFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {
    sf::CircleShape shape;

    for (const auto& particle : particles) {
        shape.setPosition(particle->Position);
        shape.setRadius(particle->Size.x / 2.0f);
        shape.setFillColor(particle->Color);

        window->draw(shape);
    }
}
