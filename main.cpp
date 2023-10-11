// TODO:
//  Make all variables defined in main.cpp then passed down
//  Fix srand(), changing field_seed does nothing
//  Add new types of noise other than sin/cos wave noise

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include "system.h"
#include "renderer.h"
#include "spawner.h"
#include "vector_field.h"

constexpr uint16_t window_height = 850;
constexpr uint16_t window_width = window_height * 1512 / 982;
const sf::Color background_color = sf::Color{0, 0, 0, 255};
const sf::Vector2i window_resolution = {window_width, window_height};

static void generateRandField(ParticleSystem &system_)
{

    float rZoom = sin(rand()) / 3 + .5;
    float rCurve = sin(rand()) / 4 + .65;
    float rOffset = pow(sin(rand()), 2) * 7;

    system_.generateField(rZoom, rCurve, rOffset);
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

    constexpr uint8_t cell_size = 50; // cant be const for some reason
    constexpr uint16_t standard_radius = 1;

    constexpr uint8_t field_refresh_seconds = 3;
    // constexpr uint8_t substep_count = 1;

    uint8_t field_seed = 2;

    // // Set simulation attributes

    // Setup system parameters
    ParticleSystem system;

    // system.setSubStepsCount(substep_count);
    system.setSimulationUpdateRate(frame_rate);

    system.setStandardRadius(standard_radius);

    system.setWorldSize(window_resolution);
    system.resizeGrid(cell_size);

    srand(field_seed);

    generateRandField(system);

    // Setup system parameters

    // Spawner
    Spawner spawner(system, window_resolution);

    spawner.spawnParticles();
    // Spawner

    Renderer renderer{window};

    sf::Clock clock;

    sf::Clock timer;
    uint32_t fps_total;
    uint16_t frames = 0;
    constexpr uint8_t fps_frames = 30;

    while (window.isOpen())
    {
        frames++;

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }

        // handle FPS
        fps_total += 1 / timer.restart().asSeconds();

        if (frames >= fps_frames)
        {
            uint16_t avg_fps = static_cast<uint16_t>(fps_total / fps_frames);

            std::cout << "FPS: " << avg_fps << std::endl;

            fps_total = 0;
            frames = 0;
        }
        // handle FPS

        if (clock.getElapsedTime().asSeconds() >= field_refresh_seconds)
        {

            generateRandField(system);
            clock.restart();
        }

        window.clear(background_color);
        system.update();
        renderer.render(system);
        window.display();
    }
    return 0;
}