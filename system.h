#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "vector_field.h"

class ParticleSystem
{
public:
    uint8_t standard_radius;

    ParticleSystem()
    {
    }

    Particle &addParticle(sf::Vector2f position)
    {
        return m_particles.emplace_back(position);
    }

    void update()
    {

        m_time += m_frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{m_num_substep}; i--;)
        {

            applyGrid(step_dt);

            updateHistory();

            updateParticles(step_dt);
        }
    }

    void setStandardRadius(uint8_t radius)
    {
        standard_radius = radius;
    }

    void setSimulationUpdateRate(uint32_t rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void resizeGrid(uint16_t cell_size)
    {
        m_grid.setSize(cell_size);
        m_cell_size = cell_size;
    }

    void setCenter(sf::Vector2f window_resolution)
    {
        m_center = window_resolution * .5f;
    }

    void setSubStepsCount(uint32_t sub_steps)
    {
        m_num_substep = sub_steps;
    }

    void setWorldSize(sf::Vector2i size)
    {
        m_world_size = size;
        m_grid.window_resolution = size;
    }

    void generateField(float zoom, float curve)
    {
        m_grid.createField(zoom, curve);
    }

    void setParticleVelocity(Particle &particle, sf::Vector2f v)
    {
        particle.setVelocity(v, getStepDt());
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

    [[nodiscard]] float getStepDt() const
    {
        return m_frame_dt / static_cast<float>(m_num_substep);
    }

private:
    uint32_t m_num_substep = 1; // dont know if substeps are needed
    sf::Vector2f m_center;
    std::vector<Particle> m_particles;

    float m_time = 0.0f;
    float m_frame_dt = 0.0f;

    sf::Vector2i m_world_size;
    VectorField m_grid;
    uint16_t m_cell_size;

    void updateParticles(float dt)
    {
        for (Particle &particle : m_particles)
        {
            particle.update(dt);
        }
    }

    void applyGrid(float dt)
    {
        // uint16_t i = 0;
        for (Particle &particle : m_particles)
        {
            if (particle.position.x < 0)
            {
                particle.setPosition(m_world_size.x - standard_radius, particle.position.y);
                particle.color = sf::Color::Blue;
            }
            else if (particle.position.x > m_world_size.x)
            {
                particle.setPosition(0, particle.position.y);
                particle.color = sf::Color::Blue;
            }
            else if (particle.position.y < 0)
            {
                particle.setPosition(particle.position.x, m_world_size.y - standard_radius);
                particle.color = sf::Color::Blue;
            }
            else if (particle.position.y > m_world_size.y)
            {
                particle.setPosition(particle.position.x, 0);
                particle.color = sf::Color::Blue;
            }
            else
            {

                // get position of cell that particle is in
                uint32_t cell_position = floor(particle.position.x / m_cell_size) + floor(particle.position.y / m_cell_size + 1) * m_grid.width + 1;

                float x = cos(m_grid.data[cell_position]);
                float y = sin(m_grid.data[cell_position]);

                sf::Vector2f vec = sf::Vector2f(x, y) * 30.0f;
                // apply that cell's vector to the particle
                particle.addVelocity(vec, dt);
                particle.slowdown(.40f);

                // I don't know if it should be .addVelocity() or .setVelocity()
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