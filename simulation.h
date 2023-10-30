#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "system.h"
#include "renderer.h"
#include "spawner.h"

class Simulation
{

public:
    Simulation(uint_fast32_t seed)
        : system(seed)
    {
        //\\// Set up window //\\//
        sf::ContextSettings settings;
        settings.antialiasingLevel = 1;
        window.create(sf::VideoMode(window_width, window_height), "Flow Curve", sf::Style::Default, settings);

        static constexpr uint8_t frame_rate = 60;

        window.setFramerateLimit(frame_rate); // 60fps max
        //\\// Set up window //\\//

        //\\// Set simulation attributes //\\//
        constexpr uint8_t flow_cell_size = 50; // in px
        constexpr uint16_t standard_radius = 1;

        const uint16_t field_width = std::floor(window_resolution.x / flow_cell_size);
        const uint16_t field_height = std::floor(window_resolution.y / flow_cell_size);

        constexpr uint_fast32_t field_seed = 11242; // up to 4,294,967,296

        constexpr uint16_t num_particles = 1000;
        //\\// Set simulation attributes //\\//

        //\\// Setup system parameters //\\//
        // system = ParticleSystem(field_seed);

        system.setSimulationUpdateRate(frame_rate);

        system.setStandardRadius(standard_radius);
        system.setWorldSize(window_resolution);

        system.resizeGrid(field_width, field_height, flow_cell_size);
        system.generateField();

        system.reserveParticleSpace(num_particles); // pre-allocate memory so spawning takes less time
        //\\// Setup system parameters //\\//

        //\\// Spawner //\\//
        Spawner spawner(system, window_resolution, num_particles);

        spawner.spawnParticles();
        //\\// Spawner //\\//

        Renderer renderer{window};

        sf::Clock clock;
    }

    void run()
    {
        sf::Clock clock;

        // sf::Clock timer;
        // uint32_t fps_total;
        // uint16_t frames = 0;
        // constexpr uint8_t fps_frames = 30;

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

            //\\// handle FPS //\\//

            // frames++;

            // fps_total += 1 / timer.restart().asSeconds();

            // if (frames >= fps_frames)
            // {
            //     uint16_t avg_fps = static_cast<uint16_t>(fps_total / fps_frames);

            //     std::cout << "FPS: " << avg_fps << std::endl;

            //     fps_total = 0;
            //     frames = 0;
            // }

            //\\// handle FPS //\\//

            window.clear(background_color);
            system.update();
            renderer.render(system);
            window.display();
        }
    }

private:
    static constexpr uint16_t window_height = 850;
    static constexpr uint16_t window_width = window_height * 1512 / 982;
    const sf::Color background_color{0, 0, 0, 255};
    const sf::Vector2i window_resolution{window_width, window_height};

    sf::RenderWindow window;

    ParticleSystem system;
    Renderer renderer{window};
};
