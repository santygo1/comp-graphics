#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

const double PI = 3.141592653589793;

VertexArray drawCube(int widthWindow = 600, int heightWindow = 600) {
    int edgeCubeLength = 100;
    int angle = 60;
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

    VertexArray lines(Lines, 24);
    lines[0] = Vector2f(x0 - edgeCubeLength / 2, y0);
    lines[1] = Vector2f(x1 - edgeCubeLength / 2, y1);
    lines[2] = Vector2f(x1 - edgeCubeLength / 2, y1);
    lines[3] = Vector2f(x2 - edgeCubeLength / 2, y2);
    lines[4] = Vector2f(x2 - edgeCubeLength / 2, y2);
    lines[5] = Vector2f(x3 - edgeCubeLength / 2, y3);
    lines[6] = Vector2f(x3 - edgeCubeLength / 2, y3);
    lines[7] = Vector2f(x0 - edgeCubeLength / 2, y0);

    lines[8] = Vector2f(x0 + edgeCubeLength / 2, y0);
    lines[9] = Vector2f(x1 + edgeCubeLength / 2, y1);
    lines[10] = Vector2f(x1 + edgeCubeLength / 2, y1);
    lines[11] = Vector2f(x2 + edgeCubeLength / 2, y2);
    lines[12] = Vector2f(x2 + edgeCubeLength / 2, y2);
    lines[13] = Vector2f(x3 + edgeCubeLength / 2, y3);
    lines[14] = Vector2f(x3 + edgeCubeLength / 2, y3);
    lines[15] = Vector2f(x0 + edgeCubeLength / 2, y0);

    lines[16] = Vector2f(x0 - edgeCubeLength / 2, y0);
    lines[17] = Vector2f(x0 + edgeCubeLength / 2, y0);
    lines[18] = Vector2f(x1 - edgeCubeLength / 2, y1);
    lines[19] = Vector2f(x1 + edgeCubeLength / 2, y1);
    lines[20] = Vector2f(x2 - edgeCubeLength / 2, y2);
    lines[21] = Vector2f(x2 + edgeCubeLength / 2, y2);
    lines[22] = Vector2f(x3 - edgeCubeLength / 2, y3);
    lines[23] = Vector2f(x3 + edgeCubeLength / 2, y3);

    return lines;
}


VertexArray drawPyramid(int widthWindow = 600) {
    int baseLength = 100;
    int d_x = 60, d_y = 60;
    int sqX1 = widthWindow / 2, sqY1 = widthWindow / 2;
    int sqX2 = sqX1 - d_x, sqY2 = sqY1 - d_y;

    VertexArray lines(Lines, 16);
    lines[0] = Vector2f(sqX1, sqY1);
    lines[1] = Vector2f(sqX1 + baseLength, sqY1);
    lines[2] = Vector2f(sqX2, sqY2);
    lines[3] = Vector2f(sqX2 + baseLength, sqY2);

    lines[4] = Vector2f(sqX1, sqY1);
    lines[5] = Vector2f(sqX2, sqY2);
    lines[6] = Vector2f(sqX1 + baseLength, sqY1);
    lines[7] = Vector2f(sqX2 + baseLength, sqY2);

    int centerX = (int) (sqX1 + d_x / (2 * sqrt(2)));
    int centerY = sqY1 + d_y / 2;
    int topX = centerX, topY = centerY - baseLength * 2;

    lines[8] = Vector2f(sqX1, sqY1);
    lines[9] = Vector2f(topX, topY);
    lines[10] = Vector2f(sqX1 + baseLength, sqY1);
    lines[11] = Vector2f(topX, topY);
    lines[12] = Vector2f(sqX2, sqY2);
    lines[13] = Vector2f(topX, topY);
    lines[14] = Vector2f(sqX2 + baseLength, sqY2);
    lines[15] = Vector2f(topX, topY);

    return lines;
}


int main() {
    int widthWindow = 600;
    int heightWindow = 600;
    RenderWindow window(VideoMode(widthWindow, heightWindow), "Lab 7");

    int choose = 0;
    do {
        cout << "Choose the figure:" << endl;
        cout << "1 - Cube" << endl;
        cout << "2 - Pyramid" << endl;
        cin >> choose;
    } while (choose != 1 && choose != 2 && choose != 3);

    VertexArray lines;
    if (choose == 1) {
        lines = drawCube();
    } else if (choose == 2) {
        lines = drawPyramid();
    }

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(lines);
        window.display();
    }

    return 0;
} 
