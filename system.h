#pragma once
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "noise.h"

class ParticleSystem
{
public:
    sf::Vector2i m_world_size;

    uint8_t standard_radius;

    ParticleSystem(uint16_t seed_)
        : m_grid{seed_}
    {
    }

    Particle &addParticle(sf::Vector2f position)
    {
        return m_particles.emplace_back(position);
    }

    void update()
    {

        m_time += m_frame_dt;

        applyGrid(m_frame_dt);

        updateParticles(m_frame_dt);

        updateHistory();

        applyBounds();
    }

    void setStandardRadius(uint8_t radius)
    {
        standard_radius = radius;
    }

    void setSimulationUpdateRate(uint32_t rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void resizeGrid(uint16_t height, uint16_t width, uint16_t cell_size)
    {
        m_grid.setSize(width + 2, height + 2); // add buffers on each side
        m_cell_size = cell_size;
    }

    void setCenter(sf::Vector2f window_resolution)
    {
        m_center = window_resolution * .5f;
    }

    void setWorldSize(sf::Vector2i size)
    {
        m_world_size = size;
        // m_grid.window_resolution = size;
    }

    void setParticleVelocity(Particle &particle, sf::Vector2f v)
    {
        particle.setVelocity(v, m_frame_dt);
    }

    void generateField()
    {
        m_grid.generateField();
    }

    [[nodiscard]] const std::vector<Particle> &getParticles() const
    {
        return m_particles;
    }

    [[nodiscard]] uint64_t getParticleCount() const
    {
        return m_particles.size();
    }

    [[nodiscard]] float getTime() const
    {
        return m_time;
    }

private:
    sf::Vector2f m_center;
    std::vector<Particle> m_particles;

    static constexpr uint16_t speed_coefficent1 = 20;
    static constexpr float slow_ratio = 0.1f; // slow down ratio%

    static constexpr uint16_t speed_coefficent2 = 200;

    float m_time = 0.0f;
    float m_frame_dt;

    PerlinField m_grid;
    uint16_t m_cell_size;

    void updateParticles(float dt)
    {
        for (Particle &particle : m_particles)
        {
            particle.update(dt);
        }
    }

    void applyBounds()
    {
        for (Particle &particle : m_particles)
        {
            if (particle.position.x < standard_radius)
            {
                particle.setPosition(m_world_size.x - standard_radius, particle.position.y);
            }
            else if (particle.position.x > m_world_size.x)
            {
                particle.setPosition(standard_radius, particle.position.y);
            }
            else if (particle.position.y < standard_radius)
            {
                particle.setPosition(particle.position.x, m_world_size.y - standard_radius);
            }
            else if (particle.position.y > m_world_size.y)
            {
                particle.setPosition(particle.position.x, standard_radius);
            }
        }
    }

    void applyGrid(float dt)
    {
        for (Particle &particle : m_particles)
        {

            if (particle.position.x > standard_radius && particle.position.x < m_world_size.x &&
                particle.position.y > standard_radius && particle.position.y < m_world_size.y)
            {

                // get position of cell that particle is in
                // the "+1"s are for the buffers of the grid
                uint32_t cell_position = floor(particle.position.x / m_cell_size) + floor(particle.position.y / m_cell_size + 1) * m_grid.width + 1;

                float x = cos(m_grid.data[cell_position]);
                float y = sin(m_grid.data[cell_position]);

                sf::Vector2f vec = sf::Vector2f(x, y);
                // apply that cell's vector to the particle

                //// smoother turs ////
                particle.addVelocity(vec * (float)speed_coefficent1, dt);
                particle.slowDown(slow_ratio);

                //// sharper turns ////
                // particle.setVelocity(vec * (float)speed_coefficent2, dt);
            }
        }
    }

    void updateHistory()
    {
        for (Particle &particle : m_particles)
        {
            particle.updateHistory();
        }
    }
};

//