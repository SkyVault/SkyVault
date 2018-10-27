#include "gui.h"

void GUI::DoDialog(const sol::table& data) {
    dialog_stack.push_back(std::make_unique<Dialog>(data));
}

void GUI::Update(const SkyTime& time) {
    if (dialog_stack.size() > 0) {
        GameState::It()->CurrentState = GameState::States::DIALOG_STATE;     

        const auto& top = dialog_stack[0];
        if (top->CurrentIndex == -1) {
            dialog_stack.erase(dialog_stack.begin());

        } else {

            // Update state and stuff

        }
    }
}

void GUI::Render(const std::unique_ptr<sf::RenderWindow>& window) {
    const auto [ww, wh] = window->getSize();

    if (dialog_stack.size() > 0) {
        //const auto& top = dialog_stack[0];

        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(ww, wh/3.0f));
        shape.setPosition(sf::Vector2f(0, wh - wh/3.0f));
        window->draw(shape);
    }
}
