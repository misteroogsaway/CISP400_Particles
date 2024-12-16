#pragma once

#include <SFML/Graphics.hpp>
#include "./particles.h"

class Engine {
    public:
        Engine();
        void run();

    private:
        vector<Particle> m_particles;
        sf::RenderWindow m_window;

        void draw();
        void input();
        void update(float dt_as_seconds);
};
