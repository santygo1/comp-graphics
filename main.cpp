#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

#define STRINGIFY(text) #text
//DECLARATION
////////////////////////////////////////////////////////////////////////////////
namespace lt {
    const float eps = 1e-6f;        // погрешность
    const float cosa = 9.99999e-1f; // для поворота на 0.01 градусов
    const float sina = 1.74533e-4f; // для поворота на 0.01 градусов
    const float two_pi = 6.2831853f;// два пи

    struct Ray {   // не нормализованный луч
        sf::Vector2f start; // начало
        sf::Vector2f dir;   // направление

        Ray() : start(0, 0), dir(0, 0) {}

        Ray(const sf::Vector2f &s, const sf::Vector2f &d)
                : start(s), dir(d) {
        }
    };

    struct Point {   // точка пересечения относ. источника света
        sf::Vector2f coord; // координата
        float dist;         // не нормализованное расстояние
        float angle;        // угол (для сортировки)

        Point() : coord(0, 0), dist(0), angle(0) {}

        Point(const sf::Vector2f &c, float d, float a)
                : coord(c), dist(d), angle(a) {
        }
    };

    inline bool operator<(const Point &lh, const Point &rh) {
        return
                (lh.angle < rh.angle) ||
                (lh.angle == rh.angle && lh.dist < rh.dist);
    }

    typedef std::vector<sf::Vector2f> Vertices;
    typedef std::vector<Ray> Lines;
    typedef std::vector<Point> Points;
    typedef std::vector<sf::Vertex> Primitives;

    Primitives to_primitives(const Vertices &v, sf::Color color);

    Primitives to_primitives(
            const Points &p,            // точки
            const sf::Vector2f &origin, // начало отсчёта(позиция света)
            sf::Color color             // цвет
    );

    // скалярное произведение
    inline float dot(const sf::Vector2f &lh, const sf::Vector2f &rh) {
        return lh.x * rh.x + lh.y * rh.y;
    }

    // псевдоскалярное произведение
    inline float cross(const sf::Vector2f &lh, const sf::Vector2f &rh) {
        return lh.x * rh.y - lh.y * rh.x;
    }

    inline Ray rot_left(const Ray &ray) {
        const float x = dot(sf::Vector2f(cosa, sina), ray.dir);
        const float y = dot(sf::Vector2f(-sina, cosa), ray.dir);
        return Ray(ray.start, sf::Vector2f(x, y));
    }

    inline Ray rot_right(const Ray &ray) {
        const float x = dot(sf::Vector2f(cosa, -sina), ray.dir);
        const float y = dot(sf::Vector2f(sina, cosa), ray.dir);
        return Ray(ray.start, sf::Vector2f(x, y));
    }

    // расчет относительного угла точки пересечения
    inline void set_angle(Point &p) {
        p.angle = std::atan2(p.coord.x, p.coord.y);
        if (p.angle < 0)
            p.angle += two_pi;
    }

    // пересечение луча и отрезка
    bool intersect(
            const Ray &ray, // луч
            const Ray &line,// отрезок
            float &dist     // возвращаемое относительное расстояние
    );

    bool nearest_intersection(
            const Ray &ray,     // луч
            const Lines &lines, // отрезки
            Point &p,           // возвращаемая точка пересечения
            bool init = false   // если true, то p - валидная точка
            // и сравнение идёт с ней
    );

    // возвращает полигон освещённой области(sf::TriangleFan)
    // (входные параметры в одной системе координат, в мировой, например)
    Primitives light_poly(
            sf::Color color,                    // цвет
            const sf::Vector2f &light_pos,      // положение источника света
            const Lines &barriers               // преграждающие отрезки
    );

    class Light_map : sf::NonCopyable {   // класс для рендера карты света
    public:
        // конструктор
        Light_map(unsigned int width, unsigned int height);

        // устанавливет ограничивающий прямоугольник
        void set_rect(const sf::FloatRect &rect);

        // устанавливает преграждающие отрезки
        void set_barriers(const Vertices &barriers);

        // устанавливает фоновое освещение(альфа канал игнорируется)
        void set_ambient_color(sf::Color color);

        // устанавливает радиус источника света(для рассеивания)
        void set_light_radius(float radius);

        // устанавливает положение источника света
        void set_light_pos(const sf::Vector2f &light_pos);

        // устанавливает цвет источника света(альфа канал игнорируется)
        void set_light_color(sf::Color color);

        // устанавливает коэффициенты затухания
        void set_light_attenuation(float constant, float linear, float qudratic);

        // возвращает текстуру карты света
        // (рисовать после отрисовки бэкграунда в режиме смешивания sf::BlendMultiply)
        const sf::Texture &get_texture() const;

        // обновляет текстуру
        void update();

    private:
        // устанавливает флаг обновления
        void set_update_flag();

        // обновляет отрезки ограничивающего прямоугольника
        void update_rect();

        static const std::string vert;  // вершинный шейдер
        static const std::string frag;  // фрагментный шейдер

        sf::RenderTexture m_target;     // текстура для отрисовки
        sf::Shader m_shader;            // шейдерная программа
        sf::RenderStates m_states;      // состояния рендера

        sf::Color m_ambient;            // цвет фонового освещения
        sf::Color m_light_color;        // цвет источника света
        float m_light_radius;           // радиус источника света
        sf::Vector2f m_light_pos;       // положение источника света
        sf::Vector2f m_light_offsets[9];// смещения источника света
        sf::FloatRect m_rect;           // ограничивающий прямоугольник
        Lines m_barriers;               // преграждающие отрезки
        bool m_updated;                 // флаг обновления
    };
}

////////////////////////////////////////////////////////////////////////////////
// генерирует отрезки для теста
lt::Vertices create_barriers();
//MAIN
////////////////////////////////////////////////////////////////////////////////


sf::Vector2f speed(float t) {
    return sf::Vector2f{1.f, -cosf(t)};
}


int main() {
    const unsigned int width = 800;
    const unsigned int height = 600;
    const float radius = 10.0f;

    sf::RenderWindow window(sf::VideoMode(width, height), "Lab 8", sf::Style::Close);
    window.setFramerateLimit(120);

    sf::Texture tex;
    tex.loadFromFile("background.jpeg");
    sf::Sprite background(tex);

    lt::Vertices barriers = create_barriers();
    // контуры объектов
    lt::Primitives objects = lt::to_primitives(barriers, sf::Color::Green);
    // прямоугольник экрана в мировых координатах
    sf::FloatRect screen_rect = window.getView().getInverseTransform().transformRect(
            sf::FloatRect(-1.0f, -1.0f, 2.0f, 2.0f));
    // маркер источника света
    sf::CircleShape light_source(radius);
    light_source.setOrigin(radius, radius);
    light_source.setFillColor(sf::Color::Transparent);
    light_source.setOutlineColor(sf::Color::Red);
    light_source.setOutlineThickness(1.0f);
    // карта света
    lt::Light_map light(width, height);
    light.set_barriers(barriers);
    light.set_rect(screen_rect);
    light.set_ambient_color(sf::Color(15, 20, 15));
    light.set_light_color(sf::Color::White);
    light.set_light_radius(radius);
    light.set_light_attenuation(1.0f, 0.0014f, 0.000007f);  // затухание на 3000 пикселей


    sf::CircleShape obj(20.f);
    obj.setPosition(0.f, 200.f);
    sf::Clock time;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved) {
                const sf::Vector2i ipos(event.mouseMove.x, event.mouseMove.y);
                const sf::Vector2f pos = window.mapPixelToCoords(ipos);
                if (screen_rect.contains(pos)) {
                    light_source.setPosition(pos);
                    light.set_light_pos(pos);
                }
            }
        }

        light.update();
        window.clear(sf::Color::White);
        sf::Sprite spr = sf::Sprite(light.get_texture());
        spr.move(speed(time.getElapsedTime().asSeconds()));
        window.draw(spr, sf::BlendMultiply);
        window.display();
    }
}
//IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////
namespace lt {

    Primitives to_primitives(const Vertices &v, sf::Color color) {
        Primitives result;
        result.reserve(v.size());
        for (size_t i = 0; i < v.size(); ++i)
            result.push_back(sf::Vertex(v[i], color));
        return result;
    }

    Primitives to_primitives(
            const Points &p,
            const sf::Vector2f &origin,
            sf::Color color
    ) {
        Primitives result;
        result.reserve(p.size() + 1);
        result.push_back(sf::Vertex(origin, color));
        for (size_t i = 0; i < p.size(); ++i) {
            result.push_back(sf::Vertex(p[i].coord + origin, color));
        }
        return result;
    }

    bool intersect(
            const Ray &ray,
            const Ray &line,
            float &dist
    ) {
        bool result = false;
        const float dd = cross(line.dir, ray.dir);
        if (std::abs(dd) > eps) {
            const sf::Vector2f delta = line.start - ray.start;
            const float t2 = cross(ray.dir, delta) / dd;
            if (0 < t2 && t2 < 1) {

                const float t1 = std::abs(ray.dir.x) > eps ?
                                 (delta.x + line.dir.x * t2) / ray.dir.x :
                                 (delta.y + line.dir.y * t2) / ray.dir.y;
                if (t1 > 0) {
                    result = true;
                    dist = t1;
                }
            }
        }
        return result;
    }

    bool nearest_intersection(
            const Ray &ray,
            const Lines &lines,
            Point &p,
            bool init
    ) {
        bool result = init;
        for (size_t i = 0; i < lines.size(); ++i) {
            float dist;
            if (intersect(ray, lines[i], dist)) {
                if (!result || dist < p.dist) {
                    result = true;
                    p.coord = ray.dir * dist;
                    p.dist = dist;
                }
            }
        }
        return result;
    }

    Primitives light_poly(
            sf::Color color,
            const sf::Vector2f &light_pos,
            const Lines &barriers
    ) {
        Points points;
        points.reserve(barriers.size() * 4 + 1);
        for (size_t i = 0; i < barriers.size(); ++i) {
            const sf::Vector2f dir1(barriers[i].start - light_pos);
            const sf::Vector2f dir2(barriers[i].start + barriers[i].dir - light_pos);

            Ray ray1(light_pos, dir1);
            Ray ray2(light_pos, dir2);

            Point intersection(dir1, 1.0f, 0.0f);
            nearest_intersection(ray1, barriers, intersection, true);
            set_angle(intersection);
            points.push_back(intersection);

            intersection = Point{dir2, 1.0f, 0.0f};
            nearest_intersection(ray2, barriers, intersection, true);
            set_angle(intersection);
            points.push_back(intersection);

            if (cross(ray1.dir, ray2.dir) < 0)
                std::swap(ray1, ray2);

            const Ray rays[2] = {
                    rot_left(ray1),
                    rot_right(ray2)
            };

            for (size_t j = 0; j < 2; ++j) {
                if (nearest_intersection(rays[j], barriers, intersection)) {
                    set_angle(intersection);
                    points.push_back(intersection);
                }
            }
        }

        if (!points.empty()) {
            std::sort(points.begin(), points.end());
            points.push_back(points[0]);
        }

        return to_primitives(points, light_pos, color);
    }

    const std::string Light_map::vert = STRINGIFY(
    //version 130 \n

            uniform vec2 u_light_pos;

            out vec2 v_delta;

            void main() {
                v_delta = gl_Vertex.xy - u_light_pos;
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                gl_FrontColor = gl_Color;
            }
    );

    const std::string Light_map::frag = STRINGIFY(
    //version 130 \n

            uniform vec3 u_attenuation;

            in vec2 v_delta;

            void main() {
                float distance = length(v_delta);
                float itensity =
                        1.0 / (u_attenuation.x + u_attenuation.y * distance + u_attenuation.z * distance * distance);
                gl_FragColor = vec4(gl_Color.rgb, gl_Color.a * itensity);
            }
    );

    Light_map::Light_map(unsigned int width, unsigned int height)
            : m_ambient(0), m_light_color(sf::Color::White), m_light_radius(10.0f), m_light_pos(0, 0), m_barriers(4),
              m_updated(true) {
        m_target.create(width, height);

        m_shader.loadFromMemory(vert, frag);
        m_shader.setUniform("u_attenuation", sf::Glsl::Vec3(1.0f, 0.0f, 0.0f));

        m_states.shader = &m_shader;
        m_states.blendMode = sf::BlendAdd;

        m_light_offsets[0] = sf::Vector2f(0.0f, 0.0f);
        m_light_offsets[1] = sf::Vector2f(10.0f, 0.0f);
        m_light_offsets[2] = sf::Vector2f(7.0f, 7.0f);
        m_light_offsets[3] = sf::Vector2f(0.0f, 10.0f);
        m_light_offsets[4] = sf::Vector2f(-7.0f, 7.0f);
        m_light_offsets[5] = sf::Vector2f(-10.0f, 0.0f);
        m_light_offsets[6] = sf::Vector2f(-7.0f, -7.0f);
        m_light_offsets[7] = sf::Vector2f(0.0f, -10.0f);
        m_light_offsets[8] = sf::Vector2f(7.0f, -7.0f);

        set_rect(sf::FloatRect(0, 0, float(width), float(height)));
    }

    void Light_map::set_rect(const sf::FloatRect &rect) {
        m_rect = rect;
        update_rect();
    }

    void Light_map::set_barriers(const Vertices &barriers) {
        m_barriers.resize(4);
        m_barriers.reserve(barriers.size() / 2 + 4);

        for (size_t i = 1; i < barriers.size(); i += 2) {
            const sf::Vector2f &start = barriers[i - 1];
            const sf::Vector2f &end = barriers[i];
            m_barriers.push_back(Ray(start, end - start));
        }

        set_update_flag();
    }

    void Light_map::set_ambient_color(sf::Color color) {
        m_ambient = color;
        m_ambient.a = 0;
        set_update_flag();
    }

    void Light_map::set_light_radius(float radius) {
        radius = std::abs(radius);
        if (radius < eps)
            return;
        const float factor = radius / m_light_radius;
        m_light_radius = std::abs(radius);
        const size_t sz = sizeof(m_light_offsets) / sizeof(m_light_offsets[0]);
        for (size_t i = 0; i < sz; ++i)
            m_light_offsets[i] *= factor;
        update_rect();
    }

    void Light_map::set_light_pos(const sf::Vector2f &light_pos) {
        //if (!m_rect.contains(light_pos)) return;
        m_light_pos = light_pos;
        set_update_flag();
    }

    void Light_map::set_light_color(sf::Color color) {
        m_light_color = color;
        m_light_color.a = 29;
        set_update_flag();
    }

    void Light_map::set_light_attenuation(float constant, float linear, float qudratic) {
        m_shader.setUniform("u_attenuation", sf::Glsl::Vec3(constant, linear, qudratic));
        set_update_flag();
    }

    const sf::Texture &Light_map::get_texture() const {
        return m_target.getTexture();
    }

    void Light_map::update() {
        if (m_updated) {
            m_target.clear(m_ambient);
            const size_t sz = sizeof(m_light_offsets) / sizeof(m_light_offsets[0]);
            for (size_t i = 0; i < sz; ++i) {
                const sf::Vector2f pos = m_light_pos + m_light_offsets[i];
                m_shader.setUniform("u_light_pos", sf::Glsl::Vec2(pos));
                const Primitives poly = light_poly(m_light_color, pos, m_barriers);
                m_target.draw(&poly[0], poly.size(), sf::TriangleFan, m_states);
            }
            m_target.display();
            m_updated = false;
        }
    }

    void Light_map::set_update_flag() {
        if (!m_updated)
            m_updated = true;
    }

    void Light_map::update_rect() {
        const float sub = m_light_radius * 2;
        const float add = m_light_radius * 4;

        const sf::FloatRect bounds(
                m_rect.left - sub,
                m_rect.top - sub,
                m_rect.width + add,
                m_rect.height + add
        );

        const sf::Vector2f p0(bounds.left, bounds.top);
        const sf::Vector2f p1(bounds.left + bounds.width, bounds.top);
        const sf::Vector2f p2(bounds.left + bounds.width, bounds.top + bounds.height);
        const sf::Vector2f p3(bounds.left, bounds.top + bounds.height);

        m_barriers[0] = Ray(p0, p1 - p0);
        m_barriers[1] = Ray(p1, p2 - p1);
        m_barriers[2] = Ray(p2, p3 - p2);
        m_barriers[3] = Ray(p3, p0 - p3);

        set_update_flag();
    }
}

lt::Vertices create_barriers() {
    lt::Vertices result;

    result.push_back(sf::Vector2f(54, 556));
    result.push_back(sf::Vector2f(161, 488));
    result.push_back(sf::Vector2f(161, 488));
    result.push_back(sf::Vector2f(73, 447));
    result.push_back(sf::Vector2f(73, 447));
    result.push_back(sf::Vector2f(54, 556));

    result.push_back(sf::Vector2f(173, 148));
    result.push_back(sf::Vector2f(209, 111));
    result.push_back(sf::Vector2f(209, 111));
    result.push_back(sf::Vector2f(296, 144));
    result.push_back(sf::Vector2f(296, 144));
    result.push_back(sf::Vector2f(245, 233));
    result.push_back(sf::Vector2f(245, 233));
    result.push_back(sf::Vector2f(173, 148));

    result.push_back(sf::Vector2f(508, 505));
    result.push_back(sf::Vector2f(524, 478));
    result.push_back(sf::Vector2f(524, 478));
    result.push_back(sf::Vector2f(594, 547));
    result.push_back(sf::Vector2f(594, 547));
    result.push_back(sf::Vector2f(508, 505));

    result.push_back(sf::Vector2f(652, 318));
    result.push_back(sf::Vector2f(571, 281));
    result.push_back(sf::Vector2f(571, 281));
    result.push_back(sf::Vector2f(519, 166));
    result.push_back(sf::Vector2f(519, 166));
    result.push_back(sf::Vector2f(640, 90));
    result.push_back(sf::Vector2f(640, 90));
    result.push_back(sf::Vector2f(749, 209));
    result.push_back(sf::Vector2f(749, 209));
    result.push_back(sf::Vector2f(652, 318));

    result.push_back(sf::Vector2f(409, 306));
    result.push_back(sf::Vector2f(293, 488));
    result.push_back(sf::Vector2f(293, 488));
    result.push_back(sf::Vector2f(385, 189));
    result.push_back(sf::Vector2f(385, 189));
    result.push_back(sf::Vector2f(551, 412));
    result.push_back(sf::Vector2f(551, 412));
    result.push_back(sf::Vector2f(409, 306));

    return result;
}
