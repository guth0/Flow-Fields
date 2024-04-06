#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "system.h"
#include "renderer.h"
#include "spawner.h"
#include "noise.h"

class Simulation
{

public:
    Simulation(uint_fast32_t seed)
        : system(seed)
    {
        m_seed = seed;

        //\\// Set up window //\\//
        sf::ContextSettings settings;
        settings.antialiasingLevel = 1;
        window.create(sf::VideoMode(window_width, window_height), "Flow Curve", sf::Style::Default, settings);

        constexpr uint8_t frame_rate = 130; // MacBook pro reaches 120hz with "ProMotion"

        window.setFramerateLimit(frame_rate);
        //\\// Set up window //\\//

        //\\// Set simulation attributes //\\//
        constexpr uint8_t flow_cell_size = 50; // in px
        constexpr uint16_t standard_radius = 1;

        field_width = std::floor(window_resolution.x / flow_cell_size) + 2;
        field_height = std::floor(window_resolution.y / flow_cell_size) + 2;
        // +2s are for the extra cell outside of the window on each side

        constexpr uint_fast32_t field_seed = 11242; // up to 4,294,967,296

        constexpr uint16_t num_particles = PARTICLE_CAP;
        //\\// Set simulation attributes //\\//

        //\\// Setup system parameters //\\//
        // system = ParticleSystem(field_seed);

        // system.setSimulationUpdateRate(frame_rate);

        system.setStandardRadius(standard_radius);
        system.setWorldSize(window_resolution);

        system.setFieldDimensions(field_height, field_width, flow_cell_size);
        //\\// Setup system parameters //\\//

        //\\// Spawner //\\//

        Spawner spawner(system, window_resolution, num_particles);

        spawner.setTransparency(0.75f);
        spawner.setColor(sf::Color{110, 110, 255});
        spawner.setSeed(132);

        spawner.spawnParticles();
        //\\// Spawner //\\//

        Renderer renderer = Renderer(window, window_resolution);
    }

    void run()
    {

        //\\// Generate Field //\\//

        float field[field_width * field_height];

        PerlinField *perlin = new PerlinField(m_seed);

        perlin->setIntensity(5);
        perlin->setOctaves(3);
        perlin->setSize(field_width, field_height);

        perlin->generateField(field);
        delete perlin; // free it early :)

        system.setField(field);
        //\\// Generate Field //\\//

        // \\// handle FPS //\\//
        sf::Clock timer;
        uint16_t frames = 0;
        constexpr uint16_t fps_frames = 240; // This is the number of frames that it uses for the average FPS
        // \\// handle FPS //\\//

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

            // \\// handle FPS //\\//

            frames++;

            if (frames >= fps_frames)
            {
                float avg_fps = (1 / timer.restart().asSeconds()) * fps_frames;

                std::cout << "FPS: " << avg_fps << std::endl;

                frames = 0;
            }

            // \\// handle FPS //\\//

            window.clear(background_color);
            system.update();

            std::array<Particle, PARTICLE_CAP> particles = system.getParticles();
            renderer.render(particles);
            window.display();
        }
    }

private:
    int m_seed;

    static constexpr uint16_t window_height = 850;
    static constexpr uint16_t window_width = window_height * 1512 / 982;
    // this magic above number makes sure that the window is the proper ratio for a MAC
    const sf::Color background_color{0, 0, 0, 255};
    const sf::Vector2i window_resolution{window_width, window_height};

    float *field;
    int field_width;
    int field_height;

    sf::RenderWindow window;

    ParticleSystem system;
    Renderer renderer{window, window_resolution};
};
