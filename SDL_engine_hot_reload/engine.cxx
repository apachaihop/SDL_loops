#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <thread>
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

    enum event get_event() { return ev; }
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
    // if (!create_window()) {
    //   return false;
    //}
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

eng::game* reload_game(eng::game*   old,
                       const char*  library_name,
                       const char*  tmp_library_name,
                       eng::engine& engine,
                       void*&       old_handle)
{
    using namespace std::filesystem;

    if (old)
    {
        SDL_UnloadObject(old_handle);
    }

    if (std::filesystem::exists(tmp_library_name))
    {
        if (0 != remove(tmp_library_name))
        {
            std::cerr << "error: can't remove: " << tmp_library_name
                      << std::endl;
            return nullptr;
        }
    }

    try
    {
        copy(library_name, tmp_library_name); // throw on error
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: can't copy [" << library_name << "] to ["
                  << tmp_library_name << "]" << std::endl;
        return nullptr;
    }

    void* game_handle = SDL_LoadObject(tmp_library_name);

    if (game_handle == nullptr)
    {
        std::cerr << "error: failed to load: [" << tmp_library_name << "] "
                  << SDL_GetError() << std::endl;
        return nullptr;
    }

    old_handle = game_handle;

    SDL_FunctionPointer create_game_func_ptr =
        SDL_LoadFunction(game_handle, "create_game");

    if (create_game_func_ptr == nullptr)
    {
        std::cerr << "error: no function [create_game] in " << tmp_library_name
                  << " " << SDL_GetError() << std::endl;
        return nullptr;
    }
    void* destroy_game_func_ptr =
     (void*)SDL_LoadFunction(game_handle, "destroy_game");

    using create_game_ptr = decltype(&create_game);
     using destroy_game_ptr = decltype(&destroy_game);

     auto destroy_game_func =
        reinterpret_cast<destroy_game_ptr>(destroy_game_func_ptr);
    auto create_game_func =
        reinterpret_cast<create_game_ptr>(create_game_func_ptr);

    destroy_game_func(old);
    eng::game* game = create_game_func(&engine);

    if (game == nullptr)
    {
        std::cerr << " returned: nullptr" << std::endl;
        return nullptr;
    }
    return game;
}

#ifdef __cplusplus
extern "C"
#endif
    int
    main()
{
    std::unique_ptr<eng::engine, void (*)(eng::engine*)> engine(
        eng::create_engine(), eng::destroy_engine);

    if (!engine->initialize_engine())
    {
        std::cout << "Error";
        return EXIT_FAILURE;
    };

    using namespace std::string_literals;

    const char* library_name = "./libgame.so";

    using namespace std::filesystem;

    const char* tmp_library_file = "./temp.so";

    void*      game_library_handle{};
    eng::game* game = reload_game(
        nullptr, library_name, tmp_library_file, *engine, game_library_handle);

    auto time_during_loading = last_write_time(library_name);

    bool continue_loop = true;
    while (continue_loop)
    {
        auto current_write_time = last_write_time(library_name);

        if (current_write_time != time_during_loading)
        {
            file_time_type next_write_time;
            for (;;)
            {
                using namespace std::chrono;
                std::this_thread::sleep_for(milliseconds(100));
                next_write_time = last_write_time(library_name);
                if (next_write_time != current_write_time)
                {
                    current_write_time = next_write_time;
                }
                else
                {
                    break;
                }
            }

            game = reload_game(game,
                               library_name,
                               tmp_library_file,
                               *engine,
                               game_library_handle);

            if (game == nullptr)
            {
                std::cerr << "next attempt to reload game..." << std::endl;
                continue;
            }

            time_during_loading = next_write_time;
        }
        game->update();
    }
    return EXIT_SUCCESS;
}
