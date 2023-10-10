// use "./build/main" in terminal to run

// particles travel abt 1px a frame
// only respawn particles once they are
//      uint16_t respawn_buffer = max_history_length * history_frames_between * 1px/frame
// respawn_buffer pixels from the border of the screen

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include "system.h"
#include "renderer.h"
#include "spawner.h"

constexpr uint16_t window_height = 850;
constexpr uint16_t window_width = window_height * 1512 / 982;
const sf::Color background_color = sf::Color{20, 15, 0};
const sf::Vector2i window_resolution = {window_width, window_height};

static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * M_PI);
    const float b = sin(t + 0.66f * 2.0f * M_PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

int main()
{

    // Set up window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Flow Curve", sf::Style::Default, settings);
    constexpr uint32_t frame_rate = 45;
    window.setFramerateLimit(frame_rate);
    // Set up window

    // Set simulation attributes
    constexpr float flow_zoom = 0.5f;
    constexpr float flow_curve = .5f;
    constexpr float flow_offset = 2.0f;
    uint8_t cell_size = 20;
    uint16_t standard_radius = 1;
    uint8_t substep_count = 1;

    uint8_t field_seed = 220;
    srand(field_seed);
    // // Set simulation attributes

    // Setup system parameters
    ParticleSystem system;

    system.setSubStepsCount(substep_count);
    system.setSimulationUpdateRate(frame_rate);

    system.setStandardRadius(standard_radius);

    system.setWorldSize(window_resolution);
    system.resizeGrid(cell_size);
    system.generateField(flow_zoom, flow_curve, flow_offset);
    // Setup system parameters

    // Spawner
    Spawner spawner(system, window_resolution);

    spawner.spawnParticles();
    // Spawner

    Renderer renderer{window};

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }

        if (clock.getElapsedTime().asSeconds() >= 10)
        {
            float rZoom = abs(sin(rand()));
            float rCurve = sin(rand()) / 4 + .65;
            float rOffset = pow(sin(rand()), 2) * 5;

            std::cout << '(' << rZoom << ", " << rCurve << ", " << rOffset << ")" << std::endl;

            system.generateField(rZoom, rCurve, rOffset);
            clock.restart();
        }

        window.clear(background_color);
        system.update();
        renderer.render(system);
        window.display();
    }
    return 0;
}