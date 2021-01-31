#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include  <cstdlib>
#include <ctime>
#include    <cstdlib>

using namespace std;

unsigned int mod(int a) {return (a < 0) ? (-a) : (a);}

const unsigned int MAX_N = 1e6, MAX_M = 720;

struct point {float x ,y;};
struct rectangle {struct point a; int w, h;};
struct line {struct point a, b;};

unsigned int WIDTH = 800, HIGTH = 600;

struct line obs[MAX_N];
struct rectangle player;

unsigned int speed = 4;


long long lengths[MAX_M];

float dist(struct line p)
{
    return sqrt((p.a.x - p.b.x)*(p.a.x - p.b.x) + (p.a.y - p.b.y)*(p.a.y - p.b.y));
}


bool findIntersection(struct line a, struct line b, struct point* result)
{
    float x1 = a.a.x, y1 = a.a.y, x2 = a.b.x, y2 = a.b.y, x3 = b.a.x, y3 = b.a.y, x4 = b.b.x, y4 = b.b.y;

    float a1 = y2 - y1;
    float b1 = x1 - x2;
    float c1 = a1 * x1 + b1 * y1;

    float a2 = y4 - y3;
    float b2 = x3 - x4;
    float c2 = a2 * x3 + b2 * y3;

    float d = a1 * b2 - a2 * b1;

    if (d == 0) return false;

    float x = (b2 * c1 - b1 * c2) / d;
    float y = (a1 * c2 - a2 * c1) / d;
    bool online1 =
            ((fmin(x1, x2) < x || fmin(x1, x2) == x) &&
             (fmax(x1, x2) > x || fmax(x1, x2) == x) &&
             (fmin(y1, y2) < y || fmin(y1, y2) == y) &&
             (fmax(y1, y2) > y || fmax(y1, y2) == y));
    bool online2 =
            ((fmin(x3, x4) < x || fmin(x3, x4) == x) &&
             (fmax(x3, x4) > x || fmax(x3, x4) == x) &&
             (fmin(y3, y4) < y || fmin(y3, y4) == y) &&
             (fmax(y3, y4) > y || fmax(y3, y4) == y));
    if (online1 && online2)
    {
        result->x = x, result->y = y;
        return true;
    }

    return false;
}

bool isLineCollding(const struct line *rects, struct line ray, int num_of_rects, struct point *collision, int *index)
{
    float best_dist = 99999999;
    struct point col;

    for(int i = 0;i < num_of_rects;i++)
        if(findIntersection(rects[i], ray , &col))
        {
            if(best_dist > dist({ray.a, col}))
            {
                best_dist =  dist({ray.a, col});
                *collision = col;
                *index = i;
            }
        }
    return (best_dist == 99999999) ? (false) : (true);
}


void move_player()
{

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player.a.x -= speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player.a.x += speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.a.y -= speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.a.y += speed;
}

void generate_maze(int num_of_obs, int min_length, int max_length, struct line *obs)
{
    for(int i = 0;i < num_of_obs;i++)
    {
        struct line curr_line;

        curr_line.a.x = rand() % WIDTH;
        curr_line.a.y = rand() % HIGTH;
        do
        {
            curr_line.b.x = rand() % WIDTH;
            curr_line.b.y = rand() % HIGTH;
        }
        while(!(dist(curr_line) >= min_length && dist(curr_line) <= max_length));
        obs[i] = curr_line;
    }

}

int main()
{
    srand (static_cast <unsigned> (time(0)));

    unsigned int n, m, ray_length, max_length, min_length;
    cin >> n >> m >> ray_length;
    cin >> player.a.x >> player.a.y >> player.h >> player.w;
    cin >> min_length >> max_length;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HIGTH), "SFML works!");

    float d_angle = 360 / m;

    generate_maze(n, min_length, max_length, obs);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            window.clear();

            move_player();

            ///draw player
            sf::RectangleShape p(sf::Vector2f(player.w, player.h));
            p.setPosition(player.a.x, player.a.y);
            window.draw(p);

            for(int i = 0;i < m;i++)
            {

                int index;
                float angle = i*d_angle;
                struct point b = {cos(angle) * lengths[i] + player.a.x, sin(angle) * lengths[i] + player.a.y}, collision;

                struct line curr_ray = {{player.a.x, player.a.y}, b};

                if(isLineCollding(obs, curr_ray, n, &collision, &index))
                {
                    b = collision;

                    ///draw obs if it s visible
                    sf::VertexArray q(sf::LinesStrip, 2);
                    q[0].position = sf::Vector2f(obs[index].a.x, obs[index].a.y);
                    q[1].position = sf::Vector2f(obs[index].b.x, obs[index].b.y);
                    window.draw(q);
                }
                else
                {
                    lengths[i] = ray_length;
                }

                ///draw rays
                /*sf::VertexArray l(sf::LinesStrip, 2);
                l[0].position = sf::Vector2f(player.a.x, player.a.y);
                l[1].position = sf::Vector2f(b.x, b.y);
                window.draw(l);*/

            }
            window.display();
        }
    }
    return 0;
}


/**
1 360 500
50 50 20 100
400 400 10 10

vhod
*********************
100 360 500
500 500 10 10
60 120
*********************

108.364 50
50 66.6272
138.55 50
69.5892 50
*/
