#pragma once
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "noise.h"

#define PARTICLE_CAP 3400

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

        return m_particles[m_particle_count++];
        // POST-INCREMENT
        // This is here because the particle that is added is going to be at (n - 1) where n is the NEW number of particles
        // If I did not post-increment here, I would have to add 1 to n, then find the index at n - 1
        // this way, it is all done in one step
    }

    void update()
    {
        m_time += .001; // I think I could convert this to just an INT that represents the number of frames

        applyGrid(m_time);

        updateParticles();

        if (frames_since_last_history >= history_frames_between)
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
        applyBoundsSemiRand(respawn_distance);
    }

    void setStandardRadius(const uint8_t &radius)
    {
        standard_radius = radius;
    }

    // void setSimulationUpdateRate(const uint32_t &rate)
    // {
    //     m_frame_dt = 1.0f / static_cast<float>(rate);
    // }

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
        particle.setVelocity(v);
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

    static constexpr float speed_coefficent = 0.2;
    static constexpr float slow_ratio = 0.1f; // slow down ratio%

    float m_time = 0.0f;
    // static constexpr uint8_t m_frame_dt = 1;

    PerlinField m_grid;
    uint16_t m_cell_size;
    const uint_fast32_t &m_seed;

    static constexpr uint8_t respawn_distance = 100;

    static constexpr uint8_t history_frames_between = 3;        // small effect on performnace (higher = longer & laggier lines)
    uint8_t frames_since_last_history = history_frames_between; // must to start at/above history_frames_between

    inline void updateParticles() // very small function might be good for inline
    {
        for (Particle &particle : m_particles)
        {
            particle.update();
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

    void applyGrid(const float &t)
    {
        for (Particle &particle : m_particles)
        {

            if (!(particle.position.x <= 0 || particle.position.x >= m_world_size.x ||
                  particle.position.y <= 0 || particle.position.y >= m_world_size.y)) // Negatives are faster than positives
            {

                // get position of cell that particle is in
                // the "+1"s are for the buffers of the grid
                uint32_t cell_position = floor(particle.position.x / m_cell_size) + floor(particle.position.y / m_cell_size + 1) * m_grid.width + 1;

                float x = cos(m_grid.data[cell_position] + t);
                float y = sin(m_grid.data[cell_position] + t);

                sf::Vector2f vec = sf::Vector2f(x, y);
                // apply that cell's vector to the particle

                particle.addVelocity(vec * (float)speed_coefficent);
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