#include "gui.h"

void GUI::DoDialog(const sol::table& data) {
    dialog_stack.push_back(std::make_unique<Dialog>(data));
}

void GUI::Update(const SkyTime& time) {
    if (dialog_stack.size() > 0) {
        if (GameState::It()->CurrentState != GameState::States::DIALOG_STATE) {
            PreviousState = GameState::It()->CurrentState;
            GameState::It()->CurrentState = GameState::States::DIALOG_STATE;     
        }

        const auto& top = dialog_stack[dialog_stack.size()-1];
        if (top->CurrentIndex == -1) {
            dialog_stack.pop_back();
        } else {

            // Update state and stuff
            if (sol::object(top->Data[top->CurrentIndex][2]).is<sol::table>()) {
                if (Input::It()->IsKeyPressed(sf::Keyboard::Left)) {
                    top->CurrentOptionSelection--;
                }

                if (Input::It()->IsKeyPressed(sf::Keyboard::Right)) {
                    top->CurrentOptionSelection++;
                }

                const auto& table = top->Data[top->CurrentIndex][2].get<sol::table>();
                const auto size = table.size();
                top->CurrentOptionSelection %= size;

                if (Input::It()->IsKeyPressed(sf::Keyboard::Z, true)) {
                    const auto nextIndex = top->Data[top->CurrentIndex][2][top->CurrentOptionSelection+1][2].get<int>();
                    top->CurrentIndex = nextIndex;
                }

            } else {
                if (Input::It()->IsKeyPressed(sf::Keyboard::Z, true)) {
                    const auto& top = dialog_stack[dialog_stack.size()-1];
                    const auto nextIndex = top->Data[top->CurrentIndex][2].get<int>();
                    top->CurrentIndex = nextIndex;
                }
            }
        }
    } else {
        if (GameState::It()->CurrentState == GameState::States::DIALOG_STATE){
            GameState::It()->CurrentState = PreviousState;     
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
        shape.setFillColor(DialogBoxColor);
        window->draw(shape);

        // Draw the current dialog

        const auto& top = dialog_stack[dialog_stack.size()-1];

        if (top->CurrentIndex == -1) return;

        std::string str = top->Data[top->CurrentIndex][1];

        auto font = Assets::It()->Get<sf::Font>("dialog");
        sf::Text text;

        text.setFont(*font);
        text.setString(str);
        text.setPosition(sf::Vector2f(10, wh - (wh/3.0)));
        window->draw(text);

        // Draw dialog options
        if (sol::object(top->Data[top->CurrentIndex][2]).is<sol::table>()) {
            sf::Text option;
            sf::Vector2f cursor;

            auto option_table = top->Data[top->CurrentIndex][2].get<sol::table>();

            auto index = 0;
            option_table.for_each([&](auto& key, sol::object &value) {
                auto t = value.as<sol::table>();
                auto str = t.get<std::string>(1);

                if (top->CurrentOptionSelection == index) {
                    //sf::RectangleShape shape;
                    //const auto size = option.getLocalBounds();
                    //shape.setSize(sf::Vector2f(size.width, size.height));
                    //shape.setPosition(option.getPosition());
                    //shape.setFillColor(sf::Color(0, 0, 0, 0));
                    //shape.setOutlineColor(sf::Color(255, 255, 255));
                    //shape.setOutlineThickness(1);
                    //window->draw(shape);

                    str = "<< " + str + " >>";
                }

                option.setFont(*font);
                option.setCharacterSize(32);
                option.setString(str);
                const auto size = text.getLocalBounds();
                option.setPosition(cursor + sf::Vector2f(10, wh - size.height - font->getLineSpacing(32)));
                window->draw(option);

                cursor.x += option.getLocalBounds().width + font->getLineSpacing(32);

                index++;
            });
        }
    }
}
