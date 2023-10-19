#include <iostream>
#include <cmath>
#include <portaudio.h>

class SoundHandler
{

public:
    SoundHandler() = default;

    void initialize()
    {
        err = Pa_Initialize();
        if (err != paNoError)
        {
            // Handle the error
            return;
        }

        // Define your audio settings, such as sample rate, buffer size, and input/output parameters.
        err = Pa_OpenDefaultStream(&stream, 1, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, nullptr);
        if (err != paNoError)
        {
            // Handle the error
            return;
        }

        // Start the audio stream
        err = Pa_StartStream(stream);
        if (err != paNoError)
        {
            // Handle the error
            return;
        }
    }

    void quit()
    {
        err = Pa_StopStream(stream);
        if (err != paNoError)
        {
            // Handle the error
        }

        err = Pa_CloseStream(stream);
        if (err != paNoError)
        {
            // Handle the error
        }

        Pa_Terminate();
    }

    double getAmplitude()
    {
        return amp;
    }

private:
    PaError err;
    PaStream *stream;

    // Sample rate and buffer size settings
    static constexpr uint32_t SAMPLE_RATE = 44100;
    static constexpr uint32_t FRAMES_PER_BUFFER = 512; // Adjust as needed

    // Time window for calculating the maximum amplitude (in seconds)
    static constexpr float WINDOW_DURATION = 1.0; // Example: 1 second

    // Number of samples in the window
    static constexpr uint32_t WINDOW_SAMPLES = static_cast<uint32_t>(WINDOW_DURATION * SAMPLE_RATE);

    static double amp;

    double CalculateAmplitude(const int16_t *audioData, size_t numSamples)
    {
        double sum = 0.0;

        for (size_t i = 0; i < numSamples; i++)
        {
            double sample = static_cast<double>(audioData[i]) / INT16_MAX;
            sum += sample * sample;
        }

        double rms = sqrt(sum / numSamples);

        return rms;
    }

    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags, void *userData)
    {
        SoundHandler *soundHandler = static_cast<SoundHandler *>(userData);
        const int16_t *audioData = static_cast<const int16_t *>(inputBuffer);

        // Calculate the amplitude
        amp = soundHandler->CalculateAmplitude(audioData, framesPerBuffer); // broken
        // look more into "->"

        return paContinue;
    }
};