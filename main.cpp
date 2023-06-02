#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

const double PI = 3.141592653589793;
const int LENGTH = 18;

void drawline(RenderWindow *window, pair<int, int> p0, pair<int, int> p1) {
    Vertex line[] =
            {
                    Vertex(Vector2f(p0.first, p0.second), Color::Black),
                    Vertex(Vector2f(p1.first, p1.second), Color::Black)
            };
    window->draw(line, 2, Lines);
}

void drawCube(RenderWindow *window, pair<int, int> vertices[], int n) {
    for (int i = 0; i < n - 1; i++)
        drawline(window, vertices[i], vertices[i + 1]);
}


int main() {
    int widthWindow = 600;
    int heightWindow = 600;
    RenderWindow window(VideoMode(widthWindow, heightWindow), "Lab 6");

    int edgeCubeLength = 0;
    int angle = 0;
    do {
        cout << "Cube edge length: ";
        cin >> edgeCubeLength;
    } while (edgeCubeLength <= 0);

    cout << "Angle: ";
    cin >> angle;
    int angle90 = angle + 90;
    int angle180 = angle + 180;
    int angle270 = angle + 270;

    int x0 = (int) ((edgeCubeLength * sin(angle * PI / 180)) + widthWindow / 2);
    int y0 = (int) ((edgeCubeLength * cos(angle * PI / 180)) + heightWindow / 2);
    int x1 = (int) ((edgeCubeLength * sin(angle90 * PI / 180)) + widthWindow / 2);
    int y1 = (int) ((edgeCubeLength * cos(angle90 * PI / 180)) + heightWindow / 2);
    int x2 = (int) ((edgeCubeLength * sin(angle180 * PI / 180)) + widthWindow / 2);
    int y2 = (int) ((edgeCubeLength * cos(angle180 * PI / 180)) + heightWindow / 2);
    int x3 = (int) ((edgeCubeLength * sin(angle270 * PI / 180)) + widthWindow / 2);
    int y3 = (int) ((edgeCubeLength * cos(angle270 * PI / 180)) + heightWindow / 2);

    pair<int, int> cube[LENGTH];
    cube[0] = make_pair(x0 - edgeCubeLength / 2, y0);
    cube[1] = make_pair(x1 - edgeCubeLength / 2, y1);
    cube[2] = make_pair(x1 - edgeCubeLength / 2, y1);
    cube[3] = make_pair(x2 - edgeCubeLength / 2, y2);
    cube[4] = make_pair(x2 - edgeCubeLength / 2, y2);
    cube[5] = make_pair(x3 - edgeCubeLength / 2, y3);
    cube[6] = make_pair(x0 - edgeCubeLength / 2, y0);

    cube[7] = make_pair(x0 + edgeCubeLength / 2, y0);
    cube[8] = make_pair(x1 + edgeCubeLength / 2, y1);
    cube[9] = make_pair(x2 + edgeCubeLength / 2, y2);
    cube[10] = make_pair(x3 + edgeCubeLength / 2, y3);
    cube[11] = make_pair(x0 + edgeCubeLength / 2, y0);

    cube[12] = make_pair(x3 + edgeCubeLength / 2, y3);
    cube[13] = make_pair(x3 - edgeCubeLength / 2, y3);
    cube[14] = make_pair(x2 - edgeCubeLength / 2, y2);
    cube[15] = make_pair(x2 + edgeCubeLength / 2, y2);
    cube[16] = make_pair(x1 + edgeCubeLength / 2, y1);
    cube[17] = make_pair(x1 - edgeCubeLength / 2, y1);

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color::White);
        drawCube(&window, cube, LENGTH);
        window.display();
    }

    return 0;
}

