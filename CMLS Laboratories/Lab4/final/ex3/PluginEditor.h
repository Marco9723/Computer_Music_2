/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

//**************************************************************************
//EXERCISE


class OscillatorAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    OscillatorAudioProcessorEditor (OscillatorAudioProcessor&);
    ~OscillatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OscillatorAudioProcessor& audioProcessor;
    
    //**************************************************************************
    // EXERCISE

    juce::Slider freq;
    juce::Label freqLabel;
    
    juce::Slider amplitude;
    juce::Label amplitudeLabel;
    
    void sliderValueChanged( juce::Slider * slider) override;
    //**************************************************************************

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorAudioProcessorEditor)
};
