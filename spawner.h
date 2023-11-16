#pragma once
#include "system.h"
#include <SFML/Graphics.hpp>

class Spawner
{
public:
    Spawner(ParticleSystem &system_, const sf::Vector2i &world_size_, const uint16_t &num_particles_)
        : system{system_}, world_size{world_size_}, num_particles{num_particles_}
    {
    }

    void spawnParticles()
    {
        srand(seed);
        for (int i = 0; i < num_particles; i++)
        {

            float xRand = rand() % world_size.x;
            float yRand = rand() % world_size.y;

            sf::Vector2f pos = {xRand, yRand};

            Particle &particle = system.addParticle(pos);

            particle.color = color;
        }
    }

private:
    ParticleSystem &system;
    sf::Vector2i world_size;
    static constexpr float transparency = 0.75f;
    uint16_t num_particles = 1000;
    static constexpr uint8_t seed = 131; // arbitrary (makes the simulation deterministic)
    static constexpr uint8_t alpha = static_cast<uint8_t>(transparency * 255);
    const sf::Color color = sf::Color{200, 50, 50, alpha};
};