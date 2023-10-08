/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/

//******************************************************************************
// 1) add a couple of constants we are going to use during the implementation of the oscillator

#define SAMPLE_RATE  (44100)
#ifndef M_PI
#define M_PI  (3.14159265)
#endif
//******************************************************************************


class OscillatorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    OscillatorAudioProcessor();
    ~OscillatorAudioProcessor() override;

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
    
    //******************************************************************************
    // EXERCISE
    void setFreq(float val);
    void setAmplitude(float val);
    //******************************************************************************
    

private:
    //==============================================================================
    //******************************************************************************
    // EXERCISE
    
    float freq;
    
    float amplitude;
    
    //******************************************************************************
    // 3) Add a variable used later for sinusois computation
    float phase;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorAudioProcessor)
};
