#include "once_off_filter.h"

void OnceOffFilter::Update(const SkyTime& time, Entity* entity) { 
    if (entity->Has<Laser>()) {
        auto laser = entity->Get<Laser>();
        
        // Handling Laser types 


    }
}

void OnceOffFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* entity) {
    if (entity->Has<Laser>()) {
        auto laser = entity->Get<Laser>();
        auto body = entity->Get<Body>();

        // Get list of blocks
        
        // Handling Laser types 
        sf::RectangleShape shape;
           
        shape.setSize(sf::Vector2f(8, 8));

        auto blocks = world->GetAllWithTag("Block");

        switch(laser->Color) {
            case LaserColor::Red: shape.setFillColor(sf::Color::Red); break;
            case LaserColor::Green: shape.setFillColor(sf::Color::Green); break;
            case LaserColor::Blue: shape.setFillColor(sf::Color::Blue); break;
        }

        auto center = body->Center();
        auto dirx = 0;
        auto diry = 0;

        std::cout << laser->Facing << std::endl;
        switch(laser->Facing) {
            case North: { diry = -1; break; }
            case South: { diry = 1; break; } 
            case East: { dirx = 1; break; }
            case West: { dirx = -1; break; }
        }

        shape.setPosition(center);

        bool done = false;
        for (int i = 0; i < 100; i++) {

            if (done) break;
            for (Entity* block : blocks) {
                auto body = block->Get<Body>();

                if (body->Contains(
                    Body(shape.getPosition(),
                    shape.getSize())
                            )) {

                    switch(laser->Color) {
                        case LaserColor::Red: { if (!block->HasTag("Red")) block->Kill(); break; }
                        case LaserColor::Green: { if (!block->HasTag("Green")) block->Kill(); break; }
                        case LaserColor::Blue: { if (!block->HasTag("Blue")) block->Kill(); break; }
                    }

                    done = true;
                }
            } 
            if (done) break;

            window->draw(shape); 

            shape.setPosition(shape.getPosition() + sf::Vector2f(dirx * shape.getSize().x, diry * shape.getSize().y));
        } 
    }
}
