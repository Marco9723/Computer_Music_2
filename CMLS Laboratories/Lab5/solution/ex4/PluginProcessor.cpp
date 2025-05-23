/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthAudioProcessor::FMSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FMSynthAudioProcessor::~FMSynthAudioProcessor()
{
}

//==============================================================================
const juce::String FMSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     //********************************************************************************************//
    //3) Initialize the field of the structure FMData data
    amp = 1.0; 
    phase = 2.0;
    car_freq = 0.0;
    
    mod_freq = 0.0;
    mod_phase = 1.0;
    mod_index = 0.0;
    //********************************************************************************************//

}

void FMSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void FMSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    //********************************************************************************************//
    // 4) implement the actual FM synthesis: first retrieve the note value from the NoteOn messages
    float mod;
    juce::MidiMessage m;
    int time;
    
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.isNoteOn())
        {
            amp = 0.1;
            car_freq = m.getMidiNoteInHertz(m.getNoteNumber());
        }
        else if (m.isNoteOff())
        {
            amp = 0;
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
        }
    }
    
    // one we have the carrier frequency we can simply apply the formula (remember the Oscillator example)
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    int numSamples = buffer.getNumSamples();
    
    
    for (int i = 0; i < numSamples; ++i)
    {   
        mod =  mod_index * (float) sin((double) mod_phase);
        
        channelDataL[i] = amp * (float) sin ((double) phase + mod);
        channelDataR[i] = amp * (float) sin ((double) phase + mod);
        
        phase +=  (float) ( M_PI * 2. *( ((double) car_freq  / (double) SAMPLE_RATE)));
        if( phase >= M_PI * 2. ) phase -= M_PI * 2.;
        
        mod_phase += (float) ( M_PI * 2. * ((double) mod_freq / (double) SAMPLE_RATE) );
        if( mod_phase >= M_PI * 2. ) mod_phase -= M_PI * 2.;    
        
    }
    //********************************************************************************************//

}

void FMSynthAudioProcessor::setModFreq(float val)
{
    mod_freq = val;
}

void FMSynthAudioProcessor::setModIndex(float val)
{
    mod_index = val;
}


//==============================================================================
bool FMSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMSynthAudioProcessor::createEditor()
{
    return new FMSynthAudioProcessorEditor (*this);
}

//==============================================================================
void FMSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FMSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMSynthAudioProcessor();
}
