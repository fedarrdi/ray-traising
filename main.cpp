#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include  <cstdlib>

using namespace std;

unsigned int mod(int a) {return (a < 0) ? (-a) : (a);}

const unsigned int MAX_N = 1e6, MAX_M = 720;

struct point {float x ,y;};
struct rectangle {struct point a; int w, h;};
struct line {struct point a, b; float length;};

unsigned int WIDTH = 800, HIGTH = 600;

struct line obs[MAX_N];
struct rectangle player;

unsigned int speed = 4;

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

void move_()
{

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player.a.x -= speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player.a.x += speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.a.y -= speed;

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.a.y += speed;
}

bool isLineCollding(const struct line *rects, struct line ray, int num_of_rects, struct point *collision)
{
    for(int i = 0;i < num_of_rects;i++)
    {
        if(findIntersection(rects[i], ray , collision))
        {
           return true;
        }
    }
    return false;
}

int main()
{
    unsigned int n, m, ray_length;
    cin >> n >> m >> ray_length;

    for(int i = 0;i < n;i++)
    {
        obs[i].length = ray_length;
        cin >> obs[i].a.x >> obs[i].a.y >> obs[i].b.x >> obs[i].b.y;
    }

    cin >> player.a.x >> player.a.y >> player.h >> player.w;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HIGTH), "SFML works!");

    float d_angle = 360 / m;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            window.clear();

            ///move_player
            move_();

            ///draw player
            sf::RectangleShape p(sf::Vector2f(player.w, player.h));
            p.setPosition(player.a.x, player.a.y);
            window.draw(p);

            for(int i = 0;i < m;i++)
            {
                float angle = i*d_angle;
                struct point b = {cos(angle) * obs[i].length + player.a.x, sin(angle) * obs[i].length + player.a.y}, collision;

                struct line curr_ray = {{player.a.x, player.a.y}, b};

                if(isLineCollding(obs, curr_ray, n, &collision))
                {
                    obs[i].length = dist({player.a, collision});
                    b = collision;
                }
                else
                {
                    obs[i].length = ray_length;
                }

                ///draw rays
                sf::VertexArray l(sf::LinesStrip, 2);
                l[0].position = sf::Vector2f(player.a.x, player.a.y);
                l[1].position = sf::Vector2f(b.x, b.y);
                window.draw(l);

            }

            for(int i = 0;i < n;i++)
            {
                sf::VertexArray q(sf::LinesStrip, 2);
                q[0].position = sf::Vector2f(obs[i].a.x, obs[i].a.y);
                q[1].position = sf::Vector2f(obs[i].b.x, obs[i].b.y);

                window.draw(q);
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
4 360 500
50 50 50 100
200 100 100 200
300 130 400 150
300 300 400 400
500 500 10 10
*********************

108.364 50
50 66.6272
138.55 50
69.5892 50
*/
