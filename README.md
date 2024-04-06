## Flow Fields
---
# Desription
---
    This is a simulation of flow fields. It uses Perlin Noise (https://en.wikipedia.org/wiki/Perlin_noise) to generate a Psudo-Random vector field that particles will travel across. This leads to beautiful wave like movements of the particles. The particles are also equiped with a tail that traces where they have been. I am using Ryo Suzuki's perlin noise library to generate my vector field.
---
# Customization
---
    While there is no GUI or CLI for the user to input parameters, some things about the simulation can be tweaked. (I do plan to centralize the settings)
        - The seed for the Particles can be changed in Main.cpp and the seed for the flow field can be changed in Simulation::Simulation()
        - You can change the number of particles at the top of system.h.

        - The size of the particles can be changed in Simulation::Simulation()
        - The size of the cells in the vector field can be changed in Simulation::Simulation()
        - The background color is found at Simulation::background_color
        - The frame rate can be found in Simulation::Simulation (It is recommeneded to put the frame rate 5-10 frames above your desired rate)
        
        - Increasing PerlinField::field_intensitiy will create a less "zoomed in" version of the Perlin Noise and thus the particles will move more sparatically (and lowering it will do the oppposite). 
        - Increasing PerlinField::octaves will increase the number of octaves "made" by the Perlin Noise generator. This basically means that there will be more passes over the field to "smooth" it out. A higher number will make the generation of the field take longer.


