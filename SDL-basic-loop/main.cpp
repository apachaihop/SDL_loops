#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

using namespace std;
constexpr int width  = 800;
constexpr int height = 600;

struct CKeys
{
    SDL_KeyCode key;
    string_view name;

    CKeys(SDL_KeyCode k, string_view n)
        : key(k)
        , name(n)
    {
    }
    SDL_KeyCode get_code() const { return this->key; }
    string_view get_name() { return this->name; }
};

vector<CKeys> keys = { { SDLK_w, "up" },
                       { SDLK_a, "left" },
                       { SDLK_s, "down" },
                       { SDLK_d, "right" },
                       { SDLK_LCTRL, "button_one" },
                       { SDLK_SPACE, "button_two" },
                       { SDLK_ESCAPE, "select" },
                       { SDLK_RETURN, "start" } };

string_view button_pressed(SDL_Event event)
{

    auto it = find_if(keys.begin(),
                      keys.end(),
                      [&](const ::CKeys& k)
                      { return k.get_code() == event.key.keysym.sym; });

    if (it == keys.end())
    {
        return "";
    }
    else if (it != keys.end())
    {
        return it->name;
    }
}

int main()
{

    SDL_Window* window = NULL;
    int         quit   = 0;
    int         a      = 0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "SDL video failed to initialise: %s\n" << SDL_GetError();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("SDL_version", width, height, 0);

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                std::cout << "Exit" << std::endl;
                quit = !quit;
            }

            switch (event.type)
            {
                case SDL_EVENT_KEY_DOWN:
                    std::cout << button_pressed(event) << "Key down" << endl;
                    break;

                case SDL_EVENT_KEY_UP:
                    std::cout << button_pressed(event) << "Key released"
                              << endl;
                    break;
            }
        }
    }
    return EXIT_SUCCESS;
}
