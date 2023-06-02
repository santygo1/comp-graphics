#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>


using namespace sf;
using namespace std;


const double PI = 3.141592653589793;
const int LENGTH = 100;


// Function to draw a line in SFML
void drawline(RenderWindow *window, pair<double, double> p0, pair<double, double> p1)
{
    Vertex line[] =
            {
                    Vertex(Vector2f(p0.first, p0.second), Color::Black),
                    Vertex(Vector2f(p1.first, p1.second), Color::Black)
            };
    window->draw(line, 2, Lines);
}


// Function to draw a polygon, given vertices
void drawPolygon(RenderWindow *window, pair<double, double> vertices[], int n)
{
    for (int i = 0; i < n - 1; i++)
        drawline(window, vertices[i], vertices[i + 1]);
    drawline(window, vertices[0], vertices[n - 1]);
}



int main()
{
    int widthWindow = 600;
    int heightWindow = 600;
    RenderWindow window(VideoMode(widthWindow, heightWindow), "Lab 4");

    // center of coordinate
    int x0 = widthWindow / 2;
    int y0 = heightWindow / 2;

    double r = rand() % 200 + 100;  // r = [100, 200]
    double angle = rand() % 5 + 3;  // angle = [3, 5]

    // translate the polar initial coordinates into Cartesian coordinates
    double startX = x0 + r * cos(PI * angle / 180);
    double startY = heightWindow - (y0 + r * sin(PI * angle / 180));

    double x = startX;
    double y = startY;

    pair<double, double> polygon[LENGTH];
    for (auto & i : polygon)
    {
        double newR = r + rand() % 20;
        angle = angle + rand() % 5 + 3;

        double newX = x0 + newR * cos(PI * angle / 180);
        double newY = heightWindow - (y0 + newR * sin(PI * angle / 180));

        i = make_pair(newX, newY);

        x = newX;
        y = newY;
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::White);

        // draw polygon
        drawPolygon(&window, polygon, LENGTH);

        window.display();
    }

    return 0;
}
