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

    void setNumParticles(uint16_t num)
    {
        num_particles = num;
    }

    // This is a value from 0% to 100%
    void setTransparency(float t)
    {
        alpha = static_cast<uint8_t>(t * 255);
    }

    void setSeed(uint8_t s)
    {
        seed = s;
    }

    // the alpha value will be set to the object's member variable alpha
    void setColor(sf::Color c)
    {
        c.a = alpha; // change the alpha to the stored alpha
        color = c;
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
    uint16_t num_particles = 1000;

    uint8_t seed;        // arbitrary (makes the simulation deterministic)
    uint8_t alpha = 255; // default is full brightness
    sf::Color color = sf::Color{110, 110, 255, alpha};
};