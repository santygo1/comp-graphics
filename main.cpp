#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
const int windowWidth = 800;
const int windowHeight = 600;

bool CheckPoint(float x, float y) {
    if (x >= -windowWidth / 2 && x <= windowWidth / 2 &&
        y >= -windowHeight / 2 && y <= windowHeight / 2)
        return true;
    else
        return false;
}
using namespace sf;
using namespace std;

int main() {

    RenderWindow window(VideoMode(windowWidth, windowHeight), "lab 1");

    float x1, y1, x2, y2;

    cout << "X Y (point 1): ";
    cin >> x1 >> y1;

    while (!CheckPoint( x1, y1)) {
        cout << "X must be in [-" << windowWidth / 2 << ", " << windowWidth / 2 << "]" << endl;
        cout << "And Y must be in [0, " << windowHeight << "]" << endl;
        cout << "X Y (point 1): ";
        cin >> x1 >> y1;
    }
    x1 += windowWidth / 2;
    y1 = 0 - y1 + windowWidth / 2;

    cout << "X Y (point 2): ";
    cin >> x2 >> y2;

    while (!CheckPoint( x2, y2)) {
        cout << "X must be in [0, " << windowWidth << "]" << endl;
        cout << "And Y must be in [0, " << windowHeight << "]" << endl;
        cout << "X Y (point 2): ";
        cin >> x2 >> y2;
    }
    x2 += windowWidth / 2;
    y2 = 0 - y2 + windowWidth / 2;

    float pointX, pointY;

    cout
            << "X Y (point relative to which the straight line will be rotated): ";
    cin >> pointX >> pointY;

    while (!CheckPoint( pointX, pointY)) {
        cout << "X must be [0, " << windowWidth << "]" << endl;
        cout << "And Y must be [0, " << windowHeight << "]" << endl;
        cout << "X Y (point relative to which the straight line will be rotated): ";
        cin >> pointX >> pointY;
    }

    pointX += windowWidth / 2;
    pointY = 0 - pointY + windowWidth / 2;

    float angle;
    cout << "Angle: ";
    cin >> angle;

    bool flag = true;
    while (flag) {
        if (0 < angle && angle <= 360) {
            flag = false;
        } else {
            cout << "Angle must be (0, 360]\n";
            cout << "Angle: ";
            cin >> angle;
        }
    }
    angle = (angle * M_PI) / 180;

    Vertex line[] =
            {
                    Vertex(Vector2f(x1, y1)),
                    Vertex(Vector2f(x2, y2))
            };

    Vertex point[] =
            {
                    Vertex(Vector2f(pointX, pointY), Color::Red)
            };


    // new line
    float newX1, newX2, newY1, newY2;
    newX1 = x1 + (pointX - x1) * cos(angle) - (pointY - y1) * sin(angle);
    newY1 = y1 + (pointX - x1) * sin(angle) - (pointY - y1) * cos(angle);
    newX2 = x2 + (pointX - x2) * cos(angle) - (pointY - y2) * sin(angle);
    newY2 = y2 + (pointX - x2) * sin(angle) - (pointY - y2) * cos(angle);
    Vertex newLine[] =
            {
                    Vertex(Vector2f(newX1, newY1), Color::Blue),
                    Vertex(Vector2f(newX2, newY2), Color::Blue)
            };

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.draw(line, 2, Lines);
        window.draw(newLine, 2, Lines);
        window.draw(point, 1, Points);
        window.display();
    }
    return 0;
}