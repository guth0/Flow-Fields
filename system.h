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
        srand(seed_);
    }

    inline Particle &addParticle(const sf::Vector2f &position)
    {
        return m_particles.emplace_back(position);
    }

    inline void reserveParticleSpace(const uint16_t &num_elements)
    {
        m_particles.reserve(num_elements);
    }

    void update()
    {

        m_time += m_frame_dt;

        applyGrid(m_frame_dt, m_time * rotation_coefficent);

        updateParticles(m_frame_dt);

        updateHistory();

        //\\// No randomness in respawn //\\//
        // applyBounds(); // particles will spawn exactly across from where they exected the bounds

        //\\// complete random respawns //\\//
        // applyBoundsRand();
        //\\// small variation when on respawning //\\//

        constexpr uint8_t respawn_distance = 100;
        applyBoundsSemiRand(respawn_distance);
    }

    void setStandardRadius(const uint8_t &radius)
    {
        standard_radius = radius;
    }

    void setSimulationUpdateRate(const uint32_t &rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void resizeGrid(const uint16_t &height, const uint16_t &width, const uint16_t &cell_size)
    {
        m_grid.setSize(width + 2, height + 2); // add buffers on each side
        m_cell_size = cell_size;
    }

    void setCenter(const sf::Vector2f &window_resolution)
    {
        m_center = window_resolution * .5f;
    }

    void setWorldSize(const sf::Vector2i &size)
    {
        m_world_size = size;
    }

    void setParticleVelocity(Particle &particle, const sf::Vector2f &v)
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

    static constexpr float rotation_coefficent = 0.1f;

    static constexpr uint16_t speed_coefficent = 20;
    static constexpr float slow_ratio = 0.1f; // slow down ratio%

    float m_time = 0.0f;
    float m_frame_dt;

    PerlinField m_grid;
    uint16_t m_cell_size;

    inline void updateParticles(const float &dt) // very small function might be good for inline
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

    void applyBoundsRand()
    {
        for (Particle &particle : m_particles)
        {
            if (particle.position.x < standard_radius)
            {
                particle.setPosition(m_world_size.x - standard_radius, rand() % m_world_size.y);
            }
            else if (particle.position.x > m_world_size.x)
            {
                particle.setPosition(standard_radius, rand() % m_world_size.y);
            }
            else if (particle.position.y < standard_radius)
            {
                particle.setPosition(rand() % m_world_size.x, m_world_size.y - standard_radius);
            }
            else if (particle.position.y > m_world_size.y)
            {
                particle.setPosition(rand() % m_world_size.x, standard_radius);
            }
        }
    }

    void applyBoundsSemiRand(const uint8_t &dist)
    {
        int8_t offset;

        for (Particle &particle : m_particles)
        {
            if (particle.position.x < standard_radius)
            {
                offset = (rand() % (2 * dist)) - dist;
                particle.setPosition(m_world_size.x - standard_radius, particle.position.y + offset);
            }
            else if (particle.position.x > m_world_size.x)
            {
                offset = (rand() % (2 * dist)) - dist;
                particle.setPosition(standard_radius, particle.position.y + offset);
            }
            else if (particle.position.y < standard_radius)
            {
                offset = (rand() % (2 * dist)) - dist;
                particle.setPosition(particle.position.x + offset, m_world_size.y - standard_radius);
            }
            else if (particle.position.y > m_world_size.y)
            {
                offset = (rand() % (2 * dist)) - dist;
                particle.setPosition(particle.position.x + offset, standard_radius);
            }
        }
    }

    void applyGrid(const float &dt, const float &t)
    {
        for (Particle &particle : m_particles)
        {

            if (particle.position.x > standard_radius && particle.position.x < m_world_size.x &&
                particle.position.y > standard_radius && particle.position.y < m_world_size.y)
            {

                // get position of cell that particle is in
                // the "+1"s are for the buffers of the grid
                uint32_t cell_position = floor(particle.position.x / m_cell_size) + floor(particle.position.y / m_cell_size + 1) * m_grid.width + 1;

                float x = cos(m_grid.data[cell_position] + t);
                float y = sin(m_grid.data[cell_position] + t);

                sf::Vector2f vec = sf::Vector2f(x, y);
                // apply that cell's vector to the particle

                particle.addVelocity(vec * (float)speed_coefficent, dt);
                particle.slowDown(slow_ratio);

                // can use particle.setVelocity aswell
            }
        }
    }

    inline void updateHistory() // very small function might be good for inline
    {
        for (Particle &particle : m_particles)
        {
            particle.updateHistory();
        }
    }
};