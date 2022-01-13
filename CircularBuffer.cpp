#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
    buffer = juce::AudioSampleBuffer();
    head = 0;
    tail = 0;
    delay = 0;
}

CircularBuffer::CircularBuffer(int size, int delay)
{
    buffer = juce::AudioSampleBuffer(1, size);
    buffer.clear();
    head = delay;
    tail = 0;
    this->delay = delay;
}

float CircularBuffer::getSample()
{
    float sample;
    sample = buffer.getSample(0, tail); //odczytanie wartosci probki

    tail = ((buffer.getNumSamples() + head) - delay) % buffer.getNumSamples(); //zmiana wartosci wskaznika odczytu

    return sample;
}

void CircularBuffer::setSample(float data)
{
    buffer.setSample(0, head, data); //przypisanie danych wejsciowych do probki
    head = (head + 1) % buffer.getNumSamples(); //zmiana wartosci wskaznika zapisu
}