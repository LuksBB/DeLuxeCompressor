#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"
#include "Calculations.h"

class Compressor:Calculations {

public:
	Compressor();
	void setThreshold(float newThreshold);
	void setRatio(float newRatio);
	void setAttack(float newAttack);
	void setRelease(float newRelease);
	void setGain(float threshold, float attack, float release);
	void setMakeUp(float newMakeUp);
	float compress(float inputSignal);
	float getThreshold();
	float getRatio();
	float getAttack();
	float getRelease();
	float getMakeUp();
	void reset();
	
private:
	CircularBuffer buffer; //bufor cykliczny
	float threshold; //prog zadzialania
	float ratio; //wspolczynnik kompresji
	float attack; //czas w sekundach
	float release; //czas w sekundach
	float slope; //nachylenie
	float gain; //wzmocnienie
	float makeup; //wzmocnienie sygnalu wyjsciowego
	float tav; //wspolczynnik usredniania
	float rms; //wartosc skuteczna
};