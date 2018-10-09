#include "render_filter.h"
#include "../../skyvault.h"
#include <iostream>

void RenderFilter::Update(const SkyTime& time, std::unique_ptr<Entity>& entity){

}

void RenderFilter::Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& self){
    var ren = self->Get<Renderable>();
    var body = self->Get<Body>();
    
    ren->sprite.setColor(ren->Color);
    ren->sprite.setPosition(body->Position);

    if (ren->sprite.getTexture() != nullptr) {
        let rect = ren->sprite.getTextureRect();

        //printf("%d %d %d %d\n", rect.left, rect.top, rect.width, rect.height);
        let imwidth = static_cast<float>(rect.width);
        let imheight = static_cast<float>(rect.height);

        let scale_x = body->Size.x / imwidth;
        let scale_y = body->Size.y / imheight;

        ////printf("sx: %f sy: %f\n", imwidth, imheight);

        ren->sprite.setScale(sf::Vector2f(scale_x, scale_y));
    } else {
        ren->sprite.setScale(sf::Vector2f(1, 1));
    }

    window->draw(ren->sprite);
}


void RenderAnimatedSpriteFilter::Update(const SkyTime& time, std::unique_ptr<Entity>& self){
    var ren = self->Get<AnimatedSprite>();

    //if (ren == nullptr) {
        //std::cout << "What" << std::endl;
    //}

    let frame = ren->GetCurrentAnimation().GetCurrentFrame();
    if ((1000*ren->timer) >= frame.duration) {
        ren->timer = 0;
        ren->GetCurrentAnimation().Next();
    } else {
        ren->timer += time.dt;
    }
}

void RenderAnimatedSpriteFilter::Render(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<Entity>& self){
    var ren = self->Get<AnimatedSprite>();
    var body = self->Get<Body>();
    
    ren->GetSprite().setColor(ren->Color);
    ren->GetSprite().setPosition(body->Position);

    ren->GetSprite().setTextureRect(ren->GetCurrentAnimation().GetCurrentFrame().GetRect());

    if (ren->GetSprite().getTexture() != nullptr) {
        let rect = ren->GetSprite().getTextureRect();

        //printf("%d %d %d %d\n", rect.left, rect.top, rect.width, rect.height);
        let imwidth = static_cast<float>(rect.width);
        let imheight = static_cast<float>(rect.height);

        let scale_x = body->Size.x / imwidth;
        let scale_y = body->Size.y / imheight;

        ////printf("sx: %f sy: %f\n", imwidth, imheight);

        ren->GetSprite().setScale(sf::Vector2f(scale_x, scale_y));
    } else {
        ren->GetSprite().setScale(sf::Vector2f(1, 1));
    }

    window->draw(ren->GetSprite());
}
