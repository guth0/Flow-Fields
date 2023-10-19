// TODO:
//  remove substeps

#include <iostream>
#include <SFML/Graphics.hpp>
#include "system.h"
#include "renderer.h"
#include "spawner.h"

constexpr uint16_t window_height = 850;
constexpr uint16_t window_width = window_height * 1512 / 982;
const sf::Color background_color = sf::Color{0, 0, 0, 255};
const sf::Vector2i window_resolution = {window_width, window_height};

int main()
{

    //\\// Set up window //\\//
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Flow Curve", sf::Style::Default, settings);
    constexpr uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);
    //\\// Set up window //\\//

    //\\// Set simulation attributes //\\//
    constexpr uint8_t flow_cell_size = 50; // in px
    constexpr uint16_t standard_radius = 1;

    const uint16_t field_width = floor(window_resolution.x / flow_cell_size);
    const uint16_t field_height = floor(window_resolution.y / flow_cell_size);

    constexpr uint16_t field_seed = 11242; // up to 65,535

    // constexpr uint8_t field_refresh_seconds = 3;
    // constexpr uint8_t substep_count = 1;

    //\\// Set simulation attributes //\\//

    //\\// Setup system parameters //\\//
    ParticleSystem system(field_seed);

    // system.setSubStepsCount(substep_count);
    system.setSimulationUpdateRate(frame_rate);

    system.setStandardRadius(standard_radius);

    system.setWorldSize(window_resolution);

    system.resizeGrid(field_width, field_height, flow_cell_size);

    system.generateField();
    //\\// Setup system parameters //\\//

    //\\// Spawner //\\//
    Spawner spawner(system, window_resolution);

    spawner.spawnParticles();
    //\\// Spawner //\\//

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

        //\\// handle FPS //\\//
        fps_total += 1 / timer.restart().asSeconds();

        if (frames >= fps_frames)
        {
            uint16_t avg_fps = static_cast<uint16_t>(fps_total / fps_frames);

            std::cout << "FPS: " << avg_fps << std::endl;

            fps_total = 0;
            frames = 0;
        }
        //\\// handle FPS //\\//

        window.clear(background_color);
        system.update();
        renderer.render(system);
        window.display();
    }
    return 0;
}