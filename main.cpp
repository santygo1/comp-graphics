#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

void drawPythagoreanTree(RenderTarget &, float, int);

int main() {
    const float L = 150;
    int N = 1;

    cout << "Number of shapes: ";
    cin >> N;

    while (N <= 0) {
        cout << "The number of shapes must be more than 0" << endl;
        cout << "Number of shapes: ";
        cin >> N;
    }

    const auto width = static_cast<unsigned>(6 * L);
    const auto height = static_cast<unsigned>(4 * L);
    RenderWindow window{{width, height}, "Lab 3"};
    while (window.isOpen()) {
        for (Event event; window.pollEvent(event);) {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear();
        drawPythagoreanTree(window, L, N);
        window.display();
    }
}

void drawPythagoreanTree(RenderTarget &target, const int N,
                         const RectangleShape &parent) {
    static const float halfSqrt2 = sqrt(2.f) / 2;

    if (N < 1) return;
    target.draw(parent);
    auto const &sz = parent.getSize();
    auto const &tf = parent.getTransform();

    auto childL = parent;
    childL.setSize(sz * halfSqrt2);
    childL.setOrigin(0, childL.getSize().y);
    childL.setPosition(tf.transformPoint({0, 0}));
    childL.rotate(-45);
    drawPythagoreanTree(target, N - 1, childL);

    auto childR = parent;
    childR.setSize(sz * halfSqrt2);
    childR.setOrigin(childR.getSize());
    childR.setPosition(tf.transformPoint({sz.x, 0}));
    childR.rotate(45);
    drawPythagoreanTree(target, N - 1, childR);
}

void drawPythagoreanTree(RenderTarget &target, const float L, const int N) {
    RectangleShape rect{{L, L}};
    rect.setOrigin(rect.getSize() / 2.f);
    rect.setPosition(target.getSize().x / 2.f,
                     target.getSize().y - L / 2.f);
    rect.setFillColor(Color::White);
    drawPythagoreanTree(target, N, rect);
}
