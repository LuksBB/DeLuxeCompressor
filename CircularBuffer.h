#pragma once
#include <JuceHeader.h>

class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int size, int delay);
    float getSample(); //funkcja odczytujaca wartosc z bufora
    void setSample(float data); //funkcja dopisujaca wartosc do bufora
private:
    juce::AudioSampleBuffer buffer;
    int head; //index, pod ktory zapisujemy wartosc
    int tail; //index, spod ktorego pobieramy wartosc
    int delay;
};