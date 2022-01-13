#include "Compressor.h"
#include <cmath>
Compressor::Compressor()
{
    buffer = CircularBuffer(128, 20);
    threshold = 0.0f;
    ratio = 1.0f;
    attack = 0.005;
    release = 0.013;
    slope = 1.0f;
    gain = 1.0f;
    makeup = 0.0f;
    tav = 0.01f;
    rms = 0.0f;
};

float Compressor::compress(float inputSignal)
{
    float compressedSignal;

    rms = (1 - tav) * rms + tav * inputSignal * inputSignal; //wyznaczanie wartosci skutecznej w zadanym przedziale czasowym

    slope = 1.0f - (1.0f / ratio);
    setGain(threshold, attack, release); //wywolanie funkcji wyznaczajacej wzmocnienie
    
    compressedSignal = buffer.getSample() * gain; //pobierana jest probka z bufora, a nastepnie wymnazana przez obliczona wartosc wzmocnienia
    buffer.setSample(inputSignal); //do bufora dopisywana jest wartosc kolejnej probki

    if (compressedSignal * decibelToAmplitude(makeup) > 1.0f) {
        compressedSignal = 1.0f;
    }
    else if (compressedSignal * decibelToAmplitude(makeup) < -1.0f) {
        compressedSignal = -1.0f;
    }
    else {
        compressedSignal *= decibelToAmplitude(makeup);
    }

    return compressedSignal;
}
float Compressor::getThreshold()
{
    return threshold;
}
float Compressor::getRatio()
{
    return ratio;
}
float Compressor::getAttack()
{
    return attack;
}
float Compressor::getRelease()
{
    return release;
}
float Compressor::getMakeUp()
{
    return makeup;
}
;

void Compressor::setThreshold(float newThreshold) {
    threshold = newThreshold;
};
void Compressor::setRatio(float newRatio) {
    ratio = newRatio;
};
void Compressor::setAttack(float newAttack) {
    attack = newAttack;
};
void Compressor::setRelease(float newRelease) {
    release = newRelease;
};

void Compressor::setMakeUp(float newMakeUp)
{
    makeup = newMakeUp;
};

void Compressor::setGain(float threshold, float attack, float release)
{
    float f, fdB, rmsdB; //inicjacja niezbednych zmiennych

    rmsdB = amplitudeToDecibel(rms); //zamiana wartosci skutecznej na skale logarytmiczna

    fdB = slope * (threshold - rmsdB);

    //jezeli wartosc skuteczna nie przekroczyla progu, wzmocnienie = 0 dB
    if (fdB > 0.0f) {
        fdB = 0.0f;
    }

    f = decibelToAmplitude(fdB); //zamiana wzmocnienia ze skali logarytmicznej na amplitudowa

    if (f < gain) {
        gain = (1.0f - attack) * gain + attack * f;
    }
    else {
        gain = (1.0f - release) * gain + release * f;
    }
};

void Compressor::reset() {
    setThreshold(0.0f);
    setRatio(1.0f);
    setAttack(0.005);
    setRelease(0.013);
};