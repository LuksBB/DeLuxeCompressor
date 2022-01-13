#include "Calculations.h"
#include <cmath>

float Calculations::amplitudeToDecibel(float amplitude)
{
	return 20.0f*log10(amplitude);
}

float Calculations::decibelToAmplitude(float decibel)
{
	return pow(10.0f, decibel / 20.0f);
}
