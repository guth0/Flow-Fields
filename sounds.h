#include <iostream>
#include <portaudio.h>
#include <cmath>
#include <thread>
#include <chrono>

class SoundHandler
{

public:
    void quit()
    {
        // Stop and close the audio stream
        error = Pa_StopStream(stream);
        if (error != paNoError)
        {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
            return;
        }

        error = Pa_CloseStream(stream);
        if (error != paNoError)
        {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
            return;
        }

        // Terminate PortAudio
        Pa_Terminate();
    }

    void initialize()
    {
        error = Pa_Initialize();

        // Initialize PortAudio
        if (error != paNoError)
        {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
            return;
        }

        // Set up the audio stream
        error = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, 44100, paFramesPerBufferUnspecified, audioCallback, nullptr);
        if (error != paNoError)
        {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
            return;
        }

        // Start the audio stream
        error = Pa_StartStream(stream);
        if (error != paNoError)
        {
            std::cerr << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
            return;
        }
    }

    // Define the audio callback function
    int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
    {
        const float *in = static_cast<const float *>(inputBuffer);

        for (unsigned int i = 0; i < FRAMES_PER_BUFFER; i++)
        {
            float amplitude = std::abs(in[i]); // Use absolute value for both positive and negative amplitudes

            // Update the running maximum amplitude
            if (amplitude > maxAmplitude)
            {
                maxAmplitude = amplitude;
            }

            // If we have processed the specified number of samples in the time window, report the maximum
            if (i % WINDOW_SAMPLES == 0)
            {
                std::cout << "Maximum amplitude over the last " << WINDOW_DURATION << " seconds: " << maxAmplitude << std::endl;
                maxAmplitude = 0.0f; // Reset for the next window
            }
        }

        return paContinue; // Continue audio stream
    }

private:
    PaError error;
    PaStream *stream;

    // Define the audio callback function

    // Sample rate and buffer size settings
    static constexpr uint32_t SAMPLE_RATE = 44100;
    static constexpr uint16_t FRAMES_PER_BUFFER = 512; // Adjust as needed

    // Time window for calculating the maximum amplitude (in seconds)
    static constexpr uint8_t WINDOW_DURATION = 1.0; // Example: 1 second

    // Number of samples in the window
    static constexpr uint32_t WINDOW_SAMPLES = static_cast<unsigned int>(WINDOW_DURATION * SAMPLE_RATE);

    // Running maximum amplitude
    float maxAmplitude = 0.0f;
};
