#include "system.h"
#include <SFML/Graphics.hpp>

class Spawner
{
public:
    Spawner(ParticleSystem &system_, sf::Vector2i world_size_)
        : system{system_}, world_size{world_size_}
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

    void respawnParticle()
    {
        float xRand = rand() % world_size.x * 0.1f;
        float yRand = rand() % world_size.y;

        sf::Vector2f pos = {xRand, yRand};

        Particle &particle = system.addParticle(pos);

        particle.color = color;
    }

private:
    ParticleSystem &system;
    sf::Vector2i world_size;
    const float transparency = 0.50f;
    const uint32_t num_particles = 2000;
    const uint8_t seed = 130;
    const sf::Color color = sf::Color{255, 0, 0, static_cast<uint8_t>(transparency * 255)};
};