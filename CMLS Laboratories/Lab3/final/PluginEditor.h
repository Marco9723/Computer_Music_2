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
// 5) Let's make our editor inherit the slider listener
//********************************************************************************************//
class MIDIVelocityModifyAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    MIDIVelocityModifyAudioProcessorEditor (MIDIVelocityModifyAudioProcessor&);
    ~MIDIVelocityModifyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    
    MIDIVelocityModifyAudioProcessor& audioProcessor;
    
    //********************************************************************************************//
    // 1) Add the slider to the class declaration
    juce::Slider midiVolume;
    //********************************************************************************************//
    
    //********************************************************************************************//
    // 6) and add a function declaration for the function that will be called when the slider value changes
    void sliderValueChanged (juce::Slider* slider) override;
    //********************************************************************************************//

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIVelocityModifyAudioProcessorEditor)
};
