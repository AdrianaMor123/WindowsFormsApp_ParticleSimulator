// ============================================================
//  PARTICLE SIMULATOR  —  SFML 2.x
//  Controls:
//    Left Click (hold)    – Spawn particles at cursor
//    Right Click (hold)   – Attract particles to cursor
//    Middle Click (hold)  – Repel particles from cursor
//    G                    – Toggle gravity
//    C                    – Clear all particles
//    1-5                  – Switch spawn mode
//       1 = Fire    2 = Water   3 = Sparks
//       4 = Smoke   5 = Rainbow
//    +/-                  – Increase/decrease spawn rate
//    R                    – Toggle trails (motion blur)
//    Esc                  – Quit
// ============================================================

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>


static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));

float randFloat(float lo, float hi) {
    std::uniform_real_distribution<float> d(lo, hi);
    return d(rng);
}

sf::Color lerpColor(sf::Color a, sf::Color b, float t) {
    t = std::clamp(t, 0.f, 1.f);
    return sf::Color(
        static_cast<sf::Uint8>(a.r + (b.r - a.r) * t),
        static_cast<sf::Uint8>(a.g + (b.g - a.g) * t),
        static_cast<sf::Uint8>(a.b + (b.b - a.b) * t),
        static_cast<sf::Uint8>(a.a + (b.a - a.a) * t)
    );
}

sf::Color hsvToRgb(float h, float s, float v) {
    h = std::fmod(h, 360.f);
    int i = static_cast<int>(h / 60.f) % 6;
    float f = h / 60.f - std::floor(h / 60.f);
    float p = v * (1.f - s);
    float q = v * (1.f - f * s);
    float t = v * (1.f - (1.f - f) * s);
    float r, g, b;
    switch (i) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
    }
    return sf::Color(
        static_cast<sf::Uint8>(r * 255),
        static_cast<sf::Uint8>(g * 255),
        static_cast<sf::Uint8>(b * 255)
    );
}

struct Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color    colorStart;
    sf::Color    colorEnd;
    float        lifetime;   
    float        age;        
    float        size;       
    float        sizeFade;   
    bool         alive;

    Particle() : lifetime(1.f), age(0.f), size(3.f), sizeFade(0.8f), alive(false) {}

    float lifeRatio() const { return age / lifetime; }

    sf::Color currentColor() const {
        return lerpColor(colorStart, colorEnd, lifeRatio());
    }

    float currentSize() const {
        float t = 1.f - sizeFade * lifeRatio();
        return size * t * t;
    }

    void update(float dt, bool gravity,
        sf::Vector2f mousePos, bool attract, bool repel,
        float windowH)
    {
        if (!alive) return;
        age += dt;
        if (age >= lifetime) { alive = false; return; }

        if (gravity) vel.y += 300.f * dt;

        if (attract || repel) {
            sf::Vector2f diff = mousePos - pos;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y) + 1.f;
            float strength = std::min(15000.f / (dist * dist), 600.f);
            sf::Vector2f dir = diff / dist;
            if (repel) vel -= dir * strength * dt;
            else       vel += dir * strength * dt;
        }

        vel *= (1.f - 0.5f * dt);  
        pos += vel * dt;

        if (pos.y > windowH - 2.f && vel.y > 0.f) {
            pos.y = windowH - 2.f;
            vel.y *= -0.4f;
            vel.x *= 0.8f;
        }
    }
};

enum class Mode { Fire = 1, Water, Sparks, Smoke, Rainbow };

Particle makeParticle(sf::Vector2f origin, Mode mode) {
    Particle p;
    p.alive = true;
    p.age = 0.f;
    p.pos = origin + sf::Vector2f(randFloat(-8.f, 8.f), randFloat(-8.f, 8.f));

    switch (mode) {
    case Mode::Fire:
        p.vel = { randFloat(-60.f, 60.f), randFloat(-320.f, -80.f) };
        p.lifetime = randFloat(0.6f, 1.5f);
        p.size = randFloat(4.f, 10.f);
        p.sizeFade = 0.9f;
        p.colorStart = lerpColor({ 255,220,20,230 }, { 255,80,0,230 }, randFloat(0, 1));
        p.colorEnd = { 30, 0, 0, 0 };
        break;

    case Mode::Water:
        p.vel = { randFloat(-120.f, 120.f), randFloat(-200.f, 50.f) };
        p.lifetime = randFloat(1.f, 2.5f);
        p.size = randFloat(2.f, 6.f);
        p.sizeFade = 0.3f;
        p.colorStart = { 80, 160, 255, 220 };
        p.colorEnd = { 20,  80, 200, 0 };
        break;

    case Mode::Sparks:
        p.vel = { randFloat(-280.f, 280.f), randFloat(-380.f, -60.f) };
        p.lifetime = randFloat(0.3f, 0.9f);
        p.size = randFloat(1.f, 3.f);
        p.sizeFade = 0.95f;
        p.colorStart = { 255, 240, 180, 255 };
        p.colorEnd = { 255,  80,   0, 0 };
        break;

    case Mode::Smoke:
        p.vel = { randFloat(-30.f, 30.f), randFloat(-80.f, -20.f) };
        p.lifetime = randFloat(2.f, 4.f);
        p.size = randFloat(10.f, 22.f);
        p.sizeFade = -0.4f;   
        p.colorStart = { 80,  80,  80, 160 };
        p.colorEnd = { 120, 120, 120, 0 };
        break;

    case Mode::Rainbow: {
        float hue = randFloat(0.f, 360.f);
        p.vel = { randFloat(-180.f, 180.f), randFloat(-260.f, -30.f) };
        p.lifetime = randFloat(1.f, 2.2f);
        p.size = randFloat(3.f, 8.f);
        p.sizeFade = 0.7f;
        p.colorStart = hsvToRgb(hue, 1.f, 1.f);
        p.colorEnd = hsvToRgb(std::fmod(hue + 120.f, 360.f), 0.7f, 0.5f);
        p.colorEnd.a = 0;
        break;
    }
    }
    return p;
}

int main() {
    const unsigned W = 1280, H = 800;

    sf::RenderWindow window(
        sf::VideoMode(W, H),
        "Particle Simulator  |  LMB=Spawn  RMB=Attract  MMB=Repel  1-5=Mode  G/C/R/+/-/Esc",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded =
        font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
        font.loadFromFile("C:/Windows/Fonts/consola.ttf") ||
        font.loadFromFile("C:/Windows/Fonts/calibri.ttf") ||
        font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
        font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");

    const size_t MAX_PARTICLES = 60000;
    std::vector<Particle> particles;
    particles.reserve(MAX_PARTICLES);

    Mode  mode = Mode::Fire;
    bool  gravity = true;
    bool  trails = true;
    int   spawnRate = 30;  
    float spawnAccum = 0.f;

    sf::RectangleShape fadeRect(sf::Vector2f(W, H));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 30));

    sf::RenderTexture rt;
    rt.create(W, H);

    sf::VertexArray va(sf::Quads);

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.05f);

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed) {
                switch (ev.key.code) {
                case sf::Keyboard::Escape: window.close();          break;
                case sf::Keyboard::G:      gravity = !gravity;      break;
                case sf::Keyboard::C:      particles.clear();       break;
                case sf::Keyboard::R:      trails = !trails;       break;
                case sf::Keyboard::Num1:   mode = Mode::Fire;       break;
                case sf::Keyboard::Num2:   mode = Mode::Water;      break;
                case sf::Keyboard::Num3:   mode = Mode::Sparks;     break;
                case sf::Keyboard::Num4:   mode = Mode::Smoke;      break;
                case sf::Keyboard::Num5:   mode = Mode::Rainbow;    break;
                case sf::Keyboard::Equal:
                case sf::Keyboard::Add:
                    spawnRate = std::min(spawnRate + 5, 300);       break;
                case sf::Keyboard::Hyphen:
                case sf::Keyboard::Subtract:
                    spawnRate = std::max(spawnRate - 5, 1);         break;
                default: break;
                }
            }
        }

        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        bool lmb = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        bool rmb = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        bool mmb = sf::Mouse::isButtonPressed(sf::Mouse::Middle);

        if (lmb) {
            spawnAccum += static_cast<float>(spawnRate) * dt;
            while (spawnAccum >= 1.f && particles.size() < MAX_PARTICLES) {
                spawnAccum -= 1.f;
                particles.push_back(makeParticle(mouse, mode));
            }
            if (particles.size() >= MAX_PARTICLES) spawnAccum = 0.f;
        }
        else {
            spawnAccum = 0.f;
        }

        for (auto& p : particles)
            p.update(dt, gravity, mouse, rmb, mmb, static_cast<float>(H));

        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [](const Particle& p) { return !p.alive; }),
            particles.end()
        );

        if (!trails) rt.clear(sf::Color::Black);
        else         rt.draw(fadeRect);  

        va.clear();
        va.resize(particles.size() * 4);
        for (size_t i = 0; i < particles.size(); ++i) {
            const Particle& p = particles[i];
            sf::Color c = p.currentColor();
            float s = std::max(p.currentSize(), 0.5f);
            float x = p.pos.x, y = p.pos.y;
            va[i * 4 + 0] = { { x - s, y - s }, c };
            va[i * 4 + 1] = { { x + s, y - s }, c };
            va[i * 4 + 2] = { { x + s, y + s }, c };
            va[i * 4 + 3] = { { x - s, y + s }, c };
        }
        rt.draw(va);

        if (fontLoaded) {
            const char* modeNames[] = { "", "Fire", "Water", "Sparks", "Smoke", "Rainbow" };
            std::ostringstream oss;
            oss << "Mode: " << modeNames[static_cast<int>(mode)]
                << "   Particles: " << particles.size() << " / " << MAX_PARTICLES
                << "   Rate: " << spawnRate << "/s"
                << "   Gravity: " << (gravity ? "ON" : "OFF")
                << "   Trails: " << (trails ? "ON" : "OFF");
            sf::Text hud(oss.str(), font, 14);
            hud.setFillColor(sf::Color(220, 220, 220, 210));
            hud.setOutlineColor(sf::Color(0, 0, 0, 180));
            hud.setOutlineThickness(1.f);
            hud.setPosition(10.f, 8.f);
            rt.draw(hud);

            sf::Text help(
                "LMB = Spawn   RMB = Attract   MMB = Repel   "
                "1-5 = Mode   +/- = Rate   G = Gravity   C = Clear   R = Trails   Esc = Quit",
                font, 12);
            help.setFillColor(sf::Color(160, 160, 160, 180));
            help.setOutlineColor(sf::Color(0, 0, 0, 160));
            help.setOutlineThickness(1.f);
            help.setPosition(10.f, static_cast<float>(H) - 22.f);
            rt.draw(help);
        }

        rt.display();
        window.clear();
        window.draw(sf::Sprite(rt.getTexture()));
        window.display();
    }

    return 0;
}