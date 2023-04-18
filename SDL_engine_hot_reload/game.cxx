#include <cmath>
#include <cstring>
#include <iostream>

#include "engine.hxx"

class my_game : public eng::game
{
    float A = 0, B = 0, i, j, z[1760];
    char  b[1760];

public:
    explicit my_game(eng::engine&) {}
    virtual void update() override { rotate_donut_3D(); }
    virtual void rotate_donut_3D() override
    {
        int k;
        system("clear");

        printf("\x1b[2J");
        memset(b, 32, 1760);
        memset(z, 0, 7040);
        for (j = 0; 6.28 > j; j += 0.07)
            for (i = 0; 6.28 > i; i += 0.02)
            {
                float c = sin(i), d = cos(j), e = sin(A), f = sin(j),
                      g = cos(A), h = d + 2, D = 1 / (c * h * e + f * g + 5),
                      l = cos(i), m = cos(B), n = s\
in(B),
                      t = c * h * g - f * e;
                int x   = 40 + 30 * D * (l * h * m - t * n),
                    y = 12 + 15 * D * (l * h * n + t * m), o = x + 80 * y,
                    N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g -
                             l * d * n);
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                {
                    z[o] = D;
                    ;
                    ;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        printf("\x1b[H");
        for (k = 0; 1761 > k; k++)
            putchar(k % 80 ? b[k] : 10);
        A += 0.01;
        B += 0.01;
    }
};

eng::game* create_game(eng::engine* engine)
{
    if (engine != nullptr)
    {
        return new my_game(*engine);
    }
    return nullptr;
}

void destroy_game(eng::game* game)
{
    delete game;
}
