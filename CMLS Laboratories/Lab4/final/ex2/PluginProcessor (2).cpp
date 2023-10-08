/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayLineAudioProcessor::DelayLineAudioProcessor()
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

DelayLineAudioProcessor::~DelayLineAudioProcessor()
{
}

//==============================================================================
const juce::String DelayLineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayLineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayLineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayLineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayLineAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayLineAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayLineAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayLineAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayLineAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayLineAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayLineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //********************************************************************************************//
    // 9) Initialize the variables that we are going to need in processBlock function: 
    // the buffer, the write and read pointer, the delay value
    
    dbuf.setSize(getTotalNumOutputChannels(), 100000);
    dbuf.clear(); 
    
    dw = 0;
    dr = 1;
    ds = 50000;
    
    //********************************************************************************************//

}

void DelayLineAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayLineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DelayLineAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    // 3) Delay line implementation
    int numSamples = buffer.getNumSamples();
    float wet_now = wet;
    float dry_now = dry;
    int ds_now = ds;
    
    float* channelOutDataL = buffer.getWritePointer(0);
    float* channelOutDataR = buffer.getWritePointer(1);
    
    const float* channelInData = buffer.getReadPointer(0);
    
    for (int i = 0; i < numSamples; ++i) {
        // setSample(int destChannel, int destSample, Type newValue)	

        dbuf.setSample(0, dw, channelInData[i]);
        dbuf.setSample(1, dw, channelInData[i]);
        
        channelOutDataL[i] = dry_now * channelInData[i] + wet_now * dbuf.getSample(0, dr); 
        channelOutDataR[i] = dry_now * channelInData[i] + wet_now * dbuf.getSample(1, dr);
        dw = (dw + 1 ) % ds_now ;
        dr = (dr + 1 ) % ds_now ;
    }
    //********************************************************************************************//
}

//==============================================================================
bool DelayLineAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayLineAudioProcessor::createEditor()
{
    return new DelayLineAudioProcessorEditor (*this);
}

//==============================================================================
void DelayLineAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayLineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayLineAudioProcessor();
}


//********************************************************************************************//
// 10) define the functions that will be used by the Editor to update slider values

void DelayLineAudioProcessor::set_wet(float val)
{
    wet = val;
}
void DelayLineAudioProcessor::set_dry(float val)
{
    dry = val;
}
void DelayLineAudioProcessor::set_ds(int val)
{
    ds = val;
}
//********************************************************************************************//
