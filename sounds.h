#include <iostream>
#include <portaudio.h>

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

private:
    PaError error;
    PaStream *stream;

    // Define the audio callback function
    static int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
    {
        // Process the incoming audio data here
        float *in = (float *)inputBuffer;

        // Example processing: Simply print the first audio sample
        std::cout << "First sample value: " << in[0] << std::endl;

        return paContinue;
    }
};
