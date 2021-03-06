#include "render_filter.h"
#include "../../skyvault.h"
#include <iostream>

void RenderFilter::Update(const SkyTime& time, Entity* entity){

}

void RenderFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* self){
    var ren = self->Get<Renderable>();
    var body = self->Get<Body>();
    
    ren->sprite.setColor(ren->Color);
    ren->sprite.setPosition(body->Position + ren->Offset);

    //if (ren->sprite.getTexture() != nullptr) {
        //let rect = ren->sprite.getTextureRect();

        ////printf("%d %d %d %d\n", rect.left, rect.top, rect.width, rect.height);
        ////let imwidth = static_cast<float>(rect.width);
        ////let imheight = static_cast<float>(rect.height);

        ////let scale_x = body->Size.x / imwidth;
        ////let scale_y = body->Size.y / imheight;

        //////printf("sx: %f sy: %f\n", imwidth, imheight);

        ////ren->sprite.setScale(sf::Vector2f(scale_x, scale_y));
    //} else {
        //ren->sprite.setScale(sf::Vector2f(1, 1));
    //}

    Art::It()->Draw(ren->GetSprite(), body->Position.y);
}


void RenderAnimatedSpriteFilter::Update(const SkyTime& time, Entity* self){
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

void RenderAnimatedSpriteFilter::Render(std::unique_ptr<sf::RenderWindow>& window, Entity* self){
    var ren = self->Get<AnimatedSprite>();
    var body = self->Get<Body>();
    
    ren->GetSprite().setColor(ren->Color);
    ren->GetSprite().setPosition(body->Position + ren->Offset);

    const auto frame = ren->GetCurrentAnimation().GetCurrentFrame().GetRect();
    ren->GetSprite().setTextureRect(frame);

    if (ren->GetSprite().getTexture() != nullptr) {
        let rect = ren->GetSprite().getTextureRect();

        //printf("%d %d %d %d\n", rect.left, rect.top, rect.width, rect.height);
        let imwidth = static_cast<float>(rect.width);
        let imheight = static_cast<float>(rect.height);

        let scale_x = body->Size.x / imwidth;
        let scale_y = body->Size.y / imheight;

        ren->GetSprite().setScale(sf::Vector2f(scale_x, scale_y));
    } else {
        ren->GetSprite().setScale(sf::Vector2f(1, 1));
    }

    Art::It()->Draw(ren->GetSprite(), body->Position.y);
    //window->draw(ren->GetSprite());
}
