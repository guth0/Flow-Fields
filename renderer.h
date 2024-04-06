#pragma once
#include "system.h"

class Renderer
{

public:
    explicit Renderer(sf::RenderTarget &target, const sf::Vector2i &world_size)
        : m_target{target}, m_world_size{world_size}, m_origin{m_world_size / 2}
    {
    }

    void render(std::array<Particle, PARTICLE_CAP> &particles) const
    {

        for (const Particle &particle : particles)
        {
            //\\// Render Tail //\\//
            uint8_t history_size = particle.history.size();
            uint8_t offset = 0;

            if (history_size > 1)
            {
                for (int i = 0; i < history_size; i++)
                {
                    sf::Vector2f pos = particle.history[i].position;

                    if (pos.x <= 2 || pos.x >= m_world_size.x - 2 || pos.y <= 2 || pos.y >= m_world_size.y - 2)
                    {
                        sf::VertexArray line(sf::LineStrip, i - offset);

                        for (int j = offset; j < i; j++)
                        {
                            line[j - offset] = particle.history[j];
                        }

                        m_target.draw(line);

                        offset = i + 1;
                    }
                }

                sf::VertexArray line(sf::LineStrip, history_size - offset);

                for (int i = offset; i < history_size; i++)
                {
                    const uint8_t index = i - offset;
                    line[index] = particle.history[i];
                }

                m_target.draw(line);
            }
            //\\// Render Tail //\\//
        }
    }

private:
    sf::RenderTarget &m_target;
    const sf::Vector2i &m_world_size;
    const sf::Vector2i m_origin;

    // float getSlope(const sf::Vertex line[]) const
    // {
    //     const sf::Vector2f v1 = line[0].position;
    //     const sf::Vector2f v2 = line[1].position;

    //     return ((v1.x - v2.x) / (v1.y - v2.y));
    // }

    // uint16_t getDist(const sf::Vertex line[]) const
    // {
    //     const sf::Vector2f v1 = line[0].position;
    //     const sf::Vector2f v2 = line[1].position;

    //     return (v1.x + v1.y + v2.x + v2.y);
    // }

    // uint16_t getOriginDist(const sf::Vector2f &v1) const
    // {
    //     const uint16_t dx = m_origin.x - v1.x;
    //     const uint16_t dy = m_origin.y - v1.y;
    //     return abs(dx * dx - dy * dy);
    // }

    // sf::Color getRainbow(const float &t) const
    // {
    //     const float r = sin(t);
    //     const float g = sin(t + 0.33f * 2.0f * M_PI);
    //     const float b = sin(t + 0.66f * 2.0f * M_PI);
    //     return {static_cast<uint8_t>(255.0f * r * r),
    //             static_cast<uint8_t>(255.0f * g * g),
    //             static_cast<uint8_t>(255.0f * b * b)};
    // }
};
