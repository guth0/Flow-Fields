#pragma once
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "noise.h"

#define PARTICLE_CAP 3000

class ParticleSystem
{
public:
    sf::Vector2i m_world_size;
    uint8_t standard_radius;

    ParticleSystem(const uint_fast32_t &seed_)
        : m_grid{seed_}, m_seed{seed_}
    {
    }

    inline Particle &addParticle(const sf::Vector2f &position)
    {
        m_particles[m_particle_count] = position;

        m_particle_count += 1;

        return m_particles[m_particle_count - 1]; // slow, I think ++m_par_count would work but not sure
    }

    // 471 μs (with "acc * dt2")
    void update()
    {

        m_time += m_frame_dt;

        // sf::Clock timer;

        applyGrid(m_frame_dt, (m_time * rotation_coefficent)); // ~65%

        updateParticles(m_frame_dt); // ~21%

        if (frames_since_last_history >= history_frames_between) // ~11%
        {
            updateHistory();
            frames_since_last_history = 0;
        }
        else
        {
            frames_since_last_history++;
        }

        //\\// No randomness in respawn //\\//
        // applyBounds(); // particles will spawn exactly across from where they exected the bounds

        //\\// complete random respawns //\\//
        // applyBoundsRand();
        //\\// small variation when on respawning //\\//

        applyBoundsSemiRand(respawn_distance); // ~3%
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

    [[nodiscard]] const std::array<Particle, PARTICLE_CAP> &getParticles() const
    {
        return m_particles;
    }

    [[nodiscard]] const uint16_t getParticleCount() const
    {
        return m_particle_count;
    }

    [[nodiscard]] float getTime() const
    {
        return m_time;
    }

private:
    sf::Vector2f m_center;
    std::array<Particle, PARTICLE_CAP> m_particles;
    uint16_t m_particle_count = 0;

    static constexpr float rotation_coefficent = 0.15f;

    static constexpr float speed_coefficent = 20.0f;
    static constexpr float slow_ratio = 0.1f; // slow down ratio%

    float m_time = 0.0f;
    float m_frame_dt;

    PerlinField m_grid;
    uint16_t m_cell_size;
    const uint_fast32_t &m_seed;

    static constexpr uint8_t respawn_distance = 100; // For semirandom respawn

    static constexpr uint8_t history_frames_between = 3;        // small effect on performnace (higher = longer & laggier lines)
    uint8_t frames_since_last_history = history_frames_between; // must to start at/above history_frames_between

    inline void updateParticles(const float &dt) // very small function might be good for inline
    {
        const float dt2 = dt * dt;

        for (Particle &particle : m_particles)
        {
            particle.update(dt2);
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
        srand(m_seed + (static_cast<int>(m_time * 100)) % 10); // add time for more randomness between frames

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
        int_fast8_t offset;
        srand(m_seed + (static_cast<int>(m_time * 100)) % 10); // add time for more randomness between frames

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

    // 310 μs
    void applyGrid(const float &dt, const float &t)
    {

        for (Particle &particle : m_particles)
        {

            if (particle.position.x > 0 && particle.position.x < m_world_size.x &&
                particle.position.y > 0 && particle.position.y < m_world_size.y)
            {

                // get position of cell that particle is in
                // the "+1"s are for the buffers of the grid
                // ~5% of runtime (of this function)
                uint32_t cell_position = floor(particle.position.x / m_cell_size) + floor(particle.position.y / m_cell_size + 1) * m_grid.width + 1;

                // ~17% of runtime
                float x = cos(m_grid.data[cell_position] + t);
                float y = sin(m_grid.data[cell_position] + t);

                // ~6% of runtime
                sf::Vector2f vec = sf::Vector2f(x, y);
                // apply that cell's vector to the particle

                // ~36.5% of runtime
                particle.addVelocity(vec * speed_coefficent, dt);

                // ~35.5% of runtime
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

// 308 - 104 - (200 / 4) - 15