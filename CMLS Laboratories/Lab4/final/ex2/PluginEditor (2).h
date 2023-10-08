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

//********************************************************************************************//
// 2) Make the class Listener as base class
//********************************************************************************************//
class DelayLineAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    DelayLineAudioProcessorEditor (DelayLineAudioProcessor&);
    ~DelayLineAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayLineAudioProcessor& audioProcessor;
    
    
    //********************************************************************************************//
    // 1) Add sliders and labels to the editor
    juce::Slider wetSlider;
    juce::Label wetLabel;
    juce::Slider drySlider;
    juce::Label dryLabel;
    juce::Slider timeSlider;
    juce::Label timeLabel;
    //********************************************************************************************//
    
    //********************************************************************************************//
    // 3) Define the function to be called when the slider is modified
    void sliderValueChanged(juce::Slider* slider) override;
    //********************************************************************************************//
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayLineAudioProcessorEditor)
};
