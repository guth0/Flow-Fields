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

private:
    ParticleSystem &system;
    sf::Vector2i world_size;
    static constexpr float transparency = 0.75f;
    static constexpr uint32_t num_particles = 1000;
    static constexpr uint8_t seed = 131; // arbitrary (makes the simulation deterministic)
    static constexpr uint8_t alpha = static_cast<uint8_t>(transparency * 255);
    const sf::Color color = sf::Color{110, 110, 255, alpha};

    static sf::Color getRainbow(float t)
    {
        const float r = sin(t);
        const float g = sin(t + 0.33f * 2.0f * M_PI);
        const float b = sin(t + 0.66f * 2.0f * M_PI);
        return {static_cast<uint8_t>(255.0f * r * r),
                static_cast<uint8_t>(255.0f * g * g),
                static_cast<uint8_t>(255.0f * b * b)};
    }
};