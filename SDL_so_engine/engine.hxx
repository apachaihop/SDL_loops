#ifndef SDL_ENGINE_ENGINE_HXX
#define SDL_ENGINE_ENGINE_HXX
namespace eng
{

constexpr int width  = 800;
constexpr int height = 600;
enum class event
{
    up,
    left,
    down,
    right,
    button_one,
    button_two,
    select,
    start,
    exit
};
class engine;

engine* create_engine();
void    destroy_engine(engine* eng);

class engine
{
public:
    virtual bool get_input(event& e) = 0;
    virtual bool create_window()     = 0;
    virtual bool initialize_engine() = 0;
    virtual bool rebind_key()        = 0;
};
} // namespace eng
#endif // SDL_ENGINE_ENGINE_HXX
