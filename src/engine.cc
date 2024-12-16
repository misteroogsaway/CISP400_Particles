#include "../include/engine.h"
#include <SFML/Window/Keyboard.hpp>

Engine::Engine() {
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    m_window.create(mode, "Parmticles !!!!");
}

void Engine::run() {
    sf::Clock clock;

    while (m_window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float elapsed_seconds = elapsed.asSeconds();

        input();
        update(elapsed_seconds);
        draw();
    }
}

void Engine::draw() {
    m_window.clear();

    for (const Particle& particle : m_particles) {
        m_window.draw(particle);
    }

    m_window.display();
}

void Engine::input() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;


            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    m_window.close();
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < 5; i++) {
                        int num_points = 25 + (rand() % 26);

                        sf::Vector2i location = { event.mouseButton.x, event.mouseButton.y };
                        Particle particle(m_window, num_points, location);
                        m_particles.push_back(particle);
                    }
                }
                break;

            default:
                break;
        }
    }
}

void Engine::update(float dt_as_seconds) {
    for (auto iter = m_particles.begin(); iter != m_particles.end();) {
        if (iter->get_ttl() > 0.0) {
            iter->update(dt_as_seconds);
            iter++;
        } else {
            iter = m_particles.erase(iter);
        }
    }
}
