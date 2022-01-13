#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DeLuxeCompressorAudioProcessor::DeLuxeCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    ,
    compressorState(*this, nullptr, juce::Identifier("parameters"), {
            std::make_unique<juce::AudioParameterFloat>(
              "threshold",
              "Threshold",
              juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f),0.0f),
            std::make_unique<juce::AudioParameterFloat>(
              "ratio",
              "Ratio",
              juce::NormalisableRange<float>(1.0f, 10.0f, 0.1f),1.0f),
            std::make_unique<juce::AudioParameterFloat>(
              "attack",
              "Attack",
              juce::NormalisableRange<float>(0.1f, 100.0f, 0.1f),5.0f),
            std::make_unique<juce::AudioParameterFloat>(
              "release",
              "Release",
              juce::NormalisableRange<float>(0.1, 500.0f, 0.1f),13.0f),
            std::make_unique<juce::AudioParameterFloat>(
              "makeup",
              "MakeUp",
              juce::NormalisableRange<float>(-10.0f, 10.0f, 0.1f),0.0f)
    }
    )

#endif
{
}

DeLuxeCompressorAudioProcessor::~DeLuxeCompressorAudioProcessor()
{
}

//==============================================================================
const juce::String DeLuxeCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DeLuxeCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DeLuxeCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DeLuxeCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DeLuxeCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DeLuxeCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DeLuxeCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DeLuxeCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DeLuxeCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void DeLuxeCompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DeLuxeCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /*===================================================================================================================
    tworzony jest obiekt klasy Compressor dla kazdego kanalu
    (poziomy moga sie roznic dla poszczegolnych kanalow, dlatego konieczne jest indywidualne podejscie do kazdego z nich)
    Parametry kompresji dla obu obiektow beda rowne, jednak poziom sygnalu wejsciowego moze sie roznic.
    =====================================================================================================================*/
    for (int channel = 0; channel < getNumOutputChannels(); ++channel) {
        Compressor comp = Compressor();
        compressors.add(comp);
    }

    thresholdParameter = compressorState.getRawParameterValue("threshold");
    ratioParameter = compressorState.getRawParameterValue("ratio");
    attackParameter = compressorState.getRawParameterValue("attack");
    releaseParameter = compressorState.getRawParameterValue("release");
    makeupParameter = compressorState.getRawParameterValue("makeup");
}

void DeLuxeCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DeLuxeCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DeLuxeCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto totalNumOutputChannels = getTotalNumOutputChannels(); //zmienna przechowujaca liczbe kanalow wyjsciowych
    auto totalNumSamples = buffer.getNumSamples(); //zmienna przechowujaca liczbe probek w buforze

    for (int sample = 0; sample < totalNumSamples; ++sample) {
        
        for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
            auto* data = buffer.getWritePointer(channel); //pozyskiwany jest adres do kanalu
            Compressor* compressor; //tworzony jest wskaznik do zmiennej typu Compressor dla danego kanalu
            compressor = &compressors.getReference(channel); //pobierana i przypisywana do zmiennej jest referencja do kompresora dla danego kanalu
            compressor->setThreshold(*thresholdParameter);
            compressor->setRatio(*ratioParameter + 0.1);
            compressor->setAttack(1 - pow(e, ((1 / getSampleRate()) * -2.2f) / (*attackParameter / 1000.0f)));
            compressor->setRelease(1 - pow(e, ((1 / getSampleRate()) * -2.2f) / (*releaseParameter / 1000.0f)));
            compressor->setMakeUp(*makeupParameter);
            data[sample] = compressor->compress(data[sample]); //wykonywana jest kompresja dla danej probki
        }
    }
}

//==============================================================================
bool DeLuxeCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DeLuxeCompressorAudioProcessor::createEditor()
{
    return new DeLuxeCompressorAudioProcessorEditor (*this, compressorState);
}

//==============================================================================
void DeLuxeCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = compressorState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DeLuxeCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(compressorState.state.getType())) {
            compressorState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
       
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DeLuxeCompressorAudioProcessor();
}