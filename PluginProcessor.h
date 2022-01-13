#pragma once
#include <JuceHeader.h>
#include "Compressor.h"

//==============================================================================
/**
*/
class DeLuxeCompressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DeLuxeCompressorAudioProcessor();
    ~DeLuxeCompressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::Array<Compressor>compressors;
    
private:
    float e = 2.71828182845904523536028747135266249775724709369995;
    juce::AudioProcessorValueTreeState compressorState;

    //========================================================================================
    //zmienne wskazujace na miejsca w pamieci, w ktorych przechowywane sa wartosci parametrow
    //========================================================================================
    std::atomic<float>* thresholdParameter = nullptr;
    std::atomic<float>* ratioParameter = nullptr;
    std::atomic<float>* attackParameter = nullptr;
    std::atomic<float>* releaseParameter = nullptr;
    std::atomic<float>* makeupParameter = nullptr;
    //===========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeLuxeCompressorAudioProcessor);
};
