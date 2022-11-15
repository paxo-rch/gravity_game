#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
double pi = 2*acos(0.0);

#include <SFML/Graphics.hpp>

using namespace std::chrono;
typedef std::basic_string<char> string;


sf::RenderWindow window(sf::VideoMode(1500, 700), "gravity");

uint64_t timeSinceEpochMicroseconds() {
  return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

double radians(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}

void rotate(double x, double y, double r, double *rx, double *ry)
{
    r*=-1;
    double cosR=cos(r);
    double sinR=sin(r);
    double a2[]={0,0};
    *rx=x*cosR-y*sinR;
    *ry=y*cosR+x*sinR;

}

void force(double w1, double x1, double y1, double w2, double x2, double y2, double *vx, double *vy)
{
    if(x1-x2==0 && y1-y2==0)
        return;

    double f = ((w1/10000)*(w2/10000))/(pow(x1-x2, 2) + pow(y1-y2, 2));
    double dx = x1-x2;
    double d = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));

    if(dx<0)
        dx*=-1;
    double a = acos(dx/d);
    *vx=cos(a)*f;
    *vy=sin(a)*f;
    if(x1>x2)
        *vx*=-1;
    if(y1>y2)
        *vy*=-1;
}

void drawCircle(int16_t x, int16_t y, int16_t radius, uint8_t r, uint8_t g, uint8_t b)
{
    sf::CircleShape shape(radius);
    shape.setPosition(sf::Vector2f(x, y));
    shape.setFillColor(sf::Color(r, g, b));
    window.draw(shape);
}

class Astre;

std::vector<Astre *> astres;

class Astre
{
    public:
    double x, y = 0;
    double vx, vy = 0;
    double weight = 0;
    uint8_t r, g, b = 0;
    bool canMove = false;

    Astre(int x, int y, int weight, uint8_t r, uint8_t g, uint8_t b, bool canMove)
    {
        astres.push_back(this);
        this->x = x;
        this->y = y;
        this->weight = weight;
        this->r = r;
        this->g = g;
        this->b = b;
        this->canMove = canMove;
        vx = 0;
        vy = 0;
    }
    void draw()
    {
        drawCircle(x, y, pow((this->weight*3)/(pi*4), (float) 1/3)/4, r, g, b);
    }
    static void update()
    {
        for (int i = 0; i < astres.size(); i++)
        {
            Astre* a = astres[i];

            if(astres[i]!=nullptr && a->canMove)
            {
                for (int j = 0; j < astres.size(); j++)
                {
                    Astre* b = astres[j];

                    double forcex, forcey = 0;

                    if(a!=b)
                        force(a->weight, a->x, a->y, b->weight, b->x, b->y, &forcex, &forcey);
                    
                    a->vx+=forcex/1000;
                    a->vy+=forcey/1000;
                }
            }
        }

        for (int i = 0; i < astres.size(); i++)
        {
            Astre* a = astres[i];
            if(astres[i]!=nullptr)
            {
                a->x+=a->vx*2000/a->weight;
                a->y+=a->vy*2000/a->weight;
            }
        }
    }
    static void drawall()
    {
        for (int i = 0; i < astres.size(); i++)
        {
            if(astres[i]!=nullptr)
            {
                astres[i]->draw();
            }
        }
    }
};

int randint(int a, int b)
{
    return rand()%(b-a)+a;
}

sf::Event event;

void eventsRead()
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

int main()
{
    srand (time(NULL));
    double gun_a = 180;

    Astre* asteroid = nullptr;
    Astre* earth = nullptr;

    bool generate = true;
    int nb_to_generate = 1;

    while (window.isOpen())
    {
        bool gunLoop = true;

        if(generate)
        {
            for ( int i = 0; i < astres.size(); i++)
            {
                delete astres[i];
            }
            for (; astres.size();)
            {
                astres.erase(astres.begin());
            }

            asteroid = new Astre(100, 100, 10000, 255, 255, 255, true);
            earth = new Astre(1400, 600, 10000, 100, 255, 100, false);

            for (int i = 0; i < nb_to_generate; i++)
            new Astre(randint(100, 1500), randint(100, 700), randint(1000000, 4000000), 150, 10, 0, false);
        }

        generate=false;

        while (gunLoop)
        {
            window.clear();

            double vx, vy = 0;
            rotate(1000, 0, gun_a, &vx, &vy);

            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(100, 100)),
                sf::Vertex(sf::Vector2f(100+vx, 100+vy))
            };

            Astre::drawall();
            window.draw(line, 2, sf::Lines);

            window.display();

            eventsRead();

            if((sf::Mouse::isButtonPressed(sf::Mouse::Left)))
                gunLoop = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                gun_a+=radians(0.002);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                gun_a-=radians(0.002);
        }

        asteroid->x=100;
        asteroid->y=100;
        rotate(0.3, 0, gun_a, &asteroid->vx, &asteroid->vy);
        
        while(true)
        {
            //window.clear();

            eventsRead();

            Astre::update();
            Astre::drawall();

            window.display();

            if((sf::Mouse::isButtonPressed(sf::Mouse::Right)))
                break;

            if(asteroid!=nullptr && earth!=nullptr && sqrt(pow(asteroid->x - earth->x, 2) + pow(asteroid->y - earth->y, 2)) < 50)
            {
                generate=true;
                nb_to_generate++;
                break;
            }
        }
    }

    return 0;
}