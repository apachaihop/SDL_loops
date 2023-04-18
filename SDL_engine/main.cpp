#include "engine.hxx"
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<eng::engine, void (*)(eng::engine*)> engine(
        eng::create_engine(), eng::destroy_engine);
    if (!engine->initialize_engine())
    {
        std::cout << "Error";
        return EXIT_FAILURE;
    };
    bool quit = 0;
    while (!quit)
    {
        eng::event ev;
        while (engine->get_input(ev))
        {
            if (ev == eng::event::exit)
            {
                quit = true;
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
