#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

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
    SDL_KeyCode      key;
    std::string_view name;
    event            ev;

public:
    CKeys(SDL_KeyCode k, std::string_view n, enum event e)
        : key(k)
        , name(n)
        , ev(e)
    {
    }
    SDL_KeyCode      get_code() const { return this->key; }
    std::string_view get_name() { return this->name; }
    enum event       get_event() { return ev; }
};

class engine_impl final : public engine
{
    std::vector<CKeys> binded_keys;

public:
    virtual bool get_input(event& e) final;
    virtual bool create_window() final;
    virtual bool initialize_engine() final;
};
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
