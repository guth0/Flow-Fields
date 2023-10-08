// use "./build/main" in terminal to run
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "system.h"
#include "renderer.h"

constexpr uint16_t window_height = 850;
constexpr uint16_t window_width = window_height * 1512 / 982;
const sf::Color background_color = {0, 0, 0};
const sf::Vector2i window_resolution = {window_width, window_height};

// constexpr uint8_t RADIUS = 3;
// const std::pair<uint8_t, uint8_t> gridSize = std::make_pair(ceil(window_width / (RADIUS * 2)), ceil(window_height / (RADIUS * 2)));

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
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Flow Curve");
    constexpr uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);
    // Set up window

    // Set simulation attributes
    float flow_zoom;
    float flow_curve;
    uint8_t cell_size = 20;
    uint16_t standard_radius = 5;
    uint8_t substep_count = 3;

    // Set simulation attributes

    // Setup system parameters
    ParticleSystem system;

    system.setSubStepsCount(substep_count);
    system.setSimulationUpdateRate(frame_rate);

    system.setStandardRadius(standard_radius);

    system.setWorldSize(window_resolution);
    system.resizeGrid(cell_size);
    system.generateField(flow_zoom, flow_curve);

    // Setup system parameters

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

            // Update particle system
        }
        system.update();
        window.clear(background_color);
        renderer.render(system);
        window.display();

        return 0;
    }
}