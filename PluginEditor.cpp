#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Calculations.cpp"

//==============================================================================
DeLuxeCompressorAudioProcessorEditor::DeLuxeCompressorAudioProcessorEditor (DeLuxeCompressorAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), parameters (vts)
{
    /*=========================================================================================================================================================
    W pierwszej kolejnosci ustawiany jest rozmiar okna i ustalana jest stosowana kolorystyka. 
    Nastepnie tworzone i odpowiednio opisywane sa obiekty klasy Slider.
    Okreslane jest takze ich przywiazanie do obiektow klasy AudioProcessorValueTreeState, co umozliwia odczyt i zapis wartosci Sliderow do odpowiednich stanow.
    ==========================================================================================================================================================*/

    setSize (400, 400);

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::palegoldenrod);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::palegoldenrod);

    //thresholdSlider
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible(&thresholdSlider);
    thresholdAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "threshold", thresholdSlider));
    thresholdSlider.setTextValueSuffix(" dB");
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, thresholdSlider.getTextBoxWidth(), thresholdSlider.getTextBoxHeight());
   
    //thresholdLabel
    addAndMakeVisible(&thresholdLabel);
    thresholdLabel.setText("Threshold", juce::NotificationType::dontSendNotification);
    thresholdLabel.attachToComponent(&thresholdSlider, false);
    thresholdLabel.setJustificationType(juce::Justification::centredTop);

    //ratioSlider
    ratioSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible(&ratioSlider);
    ratioAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "ratio", ratioSlider));
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, ratioSlider.getTextBoxWidth(), ratioSlider.getTextBoxHeight());
    ratioLabel.setJustificationType(juce::Justification::centredTop);
    
    //ratioLabel
    addAndMakeVisible(&ratioLabel);
    ratioLabel.setText("Ratio", juce::NotificationType::dontSendNotification);
    ratioLabel.attachToComponent(&ratioSlider, false);

    //makeupgainSlider
    makeupgainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible(&makeupgainSlider);
    makeupAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "makeup", makeupgainSlider));
    makeupgainSlider.setTextValueSuffix(" dB");
    makeupgainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, makeupgainSlider.getTextBoxWidth(), makeupgainSlider.getTextBoxHeight());

    //makeupgainLabel
    addAndMakeVisible(&makeupgainLabel);
    makeupgainLabel.setText("Make Up", juce::NotificationType::dontSendNotification);
    makeupgainLabel.attachToComponent(&makeupgainSlider, false);
    makeupgainLabel.setJustificationType(juce::Justification::centredTop);
    
    //attackSlider
    addAndMakeVisible(&attackSlider);
    attackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "attack", attackSlider));
    attackSlider.setTextValueSuffix(" ms");
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, attackSlider.getTextBoxWidth(), attackSlider.getTextBoxHeight());

    //attackLabel
    addAndMakeVisible(&attackLabel);
    attackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, true);

    //releaseSlider
    addAndMakeVisible(&releaseSlider);
    releaseAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(parameters, "release", releaseSlider));
    releaseSlider.setTextValueSuffix(" ms");
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, releaseSlider.getTextBoxWidth(), releaseSlider.getTextBoxHeight());

    //releaseLabel
    addAndMakeVisible(&releaseLabel);
    releaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, true);
}

DeLuxeCompressorAudioProcessorEditor::~DeLuxeCompressorAudioProcessorEditor()
{
}

//==============================================================================
void DeLuxeCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::silver);
    g.setColour (juce::Colours::palegoldenrod);
    g.setFont (15.0f);
    g.drawFittedText ("DeLuxe Compressor", getLocalBounds(), juce::Justification::centredBottom, 1);
}

void DeLuxeCompressorAudioProcessorEditor::resized()
{
    /*=========================================================
    Rozmieszczenie suwakow na odpowiednich pozycjach interfejsu
    ==========================================================*/

    thresholdSlider.setBounds(0, 130, 100, 220);
    ratioSlider.setBounds(150, 270, 100, 80);
    makeupgainSlider.setBounds(300, 130, 100, 220);
    attackSlider.setBounds(140, 20, 200, 50);
    releaseSlider.setBounds(140, 50, 200, 50);
}
