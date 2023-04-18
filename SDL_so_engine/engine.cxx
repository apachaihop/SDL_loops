#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include <SDL3/SDL_keycode.h>
#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>

#include "engine.hxx"
namespace eng
{
static bool Isengine = false;
class CKeys
{
    SDL_KeyCode key;
    std::string name;
    event       ev;

public:
    CKeys() {}
    CKeys(SDL_KeyCode k, std::string n, enum event e)
        : key(k)
        , name(n)
        , ev(e)
    {
    }
    SDL_KeyCode get_code() const { return this->key; }
    std::string get_name() { return this->name; }
    enum event  get_event() { return ev; }
};

class engine_impl final : public engine
{
    std::vector<CKeys> binded_keys;

public:
    bool get_input(event& e) final;
    bool create_window() final;
    bool initialize_engine() final;
    bool rebind_key() final;
};

bool engine_impl::rebind_key()
{
    std::cout << "Choose key to rebind" << std::endl;
    std::string key_name;
    std::cin >> key_name;
    auto it = std::find_if(binded_keys.begin(),
                           binded_keys.end(),
                           [&](CKeys& k) { return k.get_name() == key_name; });
    if (it == binded_keys.end())
    {
        std::cout << "No such name" << std::endl;
    }
    SDL_KeyCode kc = (SDL_KeyCode)SDL_GetKeyFromName(it->get_name().c_str());
    CKeys       new_key{ kc, key_name, it->get_event() };
    binded_keys.erase(it);
    binded_keys.push_back(new_key);
}
engine* create_engine()
{
    if (Isengine)
    {
        std::cerr << "Engine was defined arledy";
    }
    engine* res = new engine_impl();
    Isengine    = true;
    return res;
}
void destroy_engine(engine* eng)
{
    if (!Isengine)
    {
        std::cerr << "Engine not created";
        return;
    }
    delete eng;
    return;
}
bool engine_impl::create_window()
{
    const SDL_Window* window;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL video failed to initialise: %s\n" << SDL_GetError();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("SDL_version", width, height, 0);
    return true;
}
bool engine_impl::initialize_engine()
{
    binded_keys = { { SDLK_w, "up", event::up },
                    { SDLK_a, "left", event::left },
                    { SDLK_s, "down", event::down },
                    { SDLK_d, "right", event::right },
                    { SDLK_LCTRL, "button_one", event::button_one },
                    { SDLK_SPACE, "button_two", event::button_two },
                    { SDLK_ESCAPE, "select", event::select },
                    { SDLK_RETURN, "start", event::start } };
    if (!create_window())
    {
        return false;
    }
    return true;
}

bool engine_impl::get_input(eng::event& e)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            std::cout << "Goodbye :) " << std::endl;
            e = event::exit;
            return true;
        }
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            auto it =
                std::find_if(binded_keys.begin(),
                             binded_keys.end(),
                             [&](const CKeys& k)
                             { return k.get_code() == event.key.keysym.sym; });
            if (it == binded_keys.end())
            {
                return true;
            }
            e = it->get_event();
            std::cout << it->get_name() << " Key Down" << std::endl;
            return true;
        }
        if (event.type == SDL_EVENT_KEY_UP)
        {
            auto it =
                std::find_if(binded_keys.begin(),
                             binded_keys.end(),
                             [&](const CKeys& k)
                             { return k.get_code() == event.key.keysym.sym; });
            if (it == binded_keys.end())
            {
                return true;
            }
            e = it->get_event();
            std::cout << it->get_name() << " Key Released" << std::endl;
            return true;
        }
    }
    return false;
}

} // namespace eng
