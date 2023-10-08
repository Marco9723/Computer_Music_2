/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthAudioProcessorEditor::FMSynthAudioProcessorEditor (FMSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
    modFreq.setRange(0.0,1000.0,1.0);
    modFreq.setSliderStyle(juce::Slider::Rotary);
    modFreq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    modFreq.addListener(this);
    
    modFreqLabel.setText("Modulation Frequency",juce::dontSendNotification);
    modIndexLabel.setText("Modulation Index", juce::dontSendNotification);
    
    modIndex.setRange(0.0,10.0,0.5);
    modIndex.setSliderStyle(juce::Slider::Rotary);
    modIndex.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    modIndex.addListener(this);
    
    addAndMakeVisible(modFreq);
    addAndMakeVisible(modFreqLabel);
    addAndMakeVisible(modIndex);
    addAndMakeVisible(modIndexLabel);
    
    
}

FMSynthAudioProcessorEditor::~FMSynthAudioProcessorEditor()
{
}

//==============================================================================
void FMSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FMSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    modFreqLabel.setBounds(10,50,130,20);
    modFreq.setBounds(10,80,100,100);
    modIndexLabel.setBounds(200,50,130,20);
    modIndex.setBounds(200,80,100,100);
}


void FMSynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
   if (slider == &modFreq)
    {
        audioProcessor.setModFreq(modFreq.getValue());
    }
    else if (slider == &modIndex)
    {
        audioProcessor.setModIndex(modIndex.getValue());
    }
}