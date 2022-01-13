#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Calculations.h"

//==============================================================================
/**
*/
class DeLuxeCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                              public DeLuxeCompressorAudioProcessor,
                                              public Calculations
{
public:
    DeLuxeCompressorAudioProcessorEditor (DeLuxeCompressorAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~DeLuxeCompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    DeLuxeCompressorAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& parameters;

    /*===========================================================================
    Zdefiniowanie potrzebnych obiektow znajdujacych sie na graficznym interfejsie
    =============================================================================*/

    juce::Slider thresholdSlider;
    juce::Label thresholdLabel;
    juce::Slider ratioSlider;
    juce::Label ratioLabel;
    juce::Slider makeupgainSlider;
    juce::Label makeupgainLabel;
    juce::Slider attackSlider;
    juce::Label attackLabel;
    juce::Slider releaseSlider;
    juce::Label releaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> makeupAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeLuxeCompressorAudioProcessorEditor)
};