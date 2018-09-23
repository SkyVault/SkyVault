#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include <type_traits>

#if 0
#include <mutex>

class Singleton
{
private:
	Singleton(const Singleton&) = delete;
	Singleton & operator=(const Singleton&) = delete;
	

	static std::unique_ptr<Singleton> instance;
	static std::once_flag onceFlag;
public:
	Singleton() = default;

	static void NofityInit()
	{
		std::cout << "Initializing Singleton" << '\n';
	}
	static Singleton& Singleton::Instance()
	{
		std::call_once(Singleton::onceFlag,[] (){
			NofityInit();
			instance.reset(new Singleton); 
		});

		std::cout << "Getting  Singleton instance" << '\n';
		return *(instance.get());
	}
};

std::unique_ptr<Singleton> Singleton::instance;
std::once_flag Singleton::onceFlag;
#endif 

#include <mutex>

struct Assets {
private:
	Assets(const Assets&) = delete;
	Assets& operator=(const Assets&) = delete;

	static std::unique_ptr<Assets> instance;
	static std::once_flag onceFlag;

    Assets() {

    }

public:
    static auto* It() {
        std::call_once(Assets::onceFlag, [] () {
            instance.reset(new Assets());     
        });

        return instance.get();
    }

    template <typename T>
    void Add(const std::string& id, T& t) {
        if constexpr (std::is_same<T, sf::Texture>()) {
            images[id] = t;
        } else if constexpr (std::is_same<T, sf::Font>()) {
            fonts[id] = t;
        }
    }

    template <typename T>
    T Get(const std::string& id) {
        if constexpr (std::is_same<T, sf::Texture>()) {
            return images[id];
        } else if constexpr (std::is_same<T, sf::Font>()) {
            return fonts[id];
        }
    }

private:
    std::map<std::string, sf::Texture*> images;
    std::map<std::string, sf::Font*> fonts;
};
