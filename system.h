// turn off clear history
// only draw line between the i and i + 1
// if "i + 1" or "i" is not the maximum that they can be

#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "vector_field.h"

class ParticleSystem
{
public:
    sf::Vector2i m_world_size;

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

        applyGrid(step_dt);

        updateParticles(step_dt);

        updateHistory();

        applyBounds();

        // std::cout << m_particles[0].position.x << " -- " << m_particles[0].position.y << std::endl;
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

    void setParticleVelocity(Particle &particle, sf::Vector2f v)
    {
        particle.setVelocity(v, getStepDt());
    }

    void generateField(float zoom, float curve, float offset)
    {
        m_grid.createField(zoom, curve, offset);
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

    float speed_coefficent = 200;

    float m_time = 0.0f;
    float m_frame_dt = 0.0f;

    VectorField m_grid;
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
                uint32_t cell_position = floor(particle.position.x / m_cell_size) + floor(particle.position.y / m_cell_size + 1) * m_grid.width + 1;

                float x = cos(m_grid.data[cell_position]);
                float y = sin(m_grid.data[cell_position]);

                sf::Vector2f vec = sf::Vector2f(x, y);
                // apply that cell's vector to the particle

                // smoother turs
                // particle.addVelocity(vec, dt);

                // particle.slowdown(.001f);

                // sharper turns
                particle.setVelocity(vec * speed_coefficent, dt);

                // both .addVelocity() and .setVelocity() work
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