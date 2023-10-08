/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscillatorAudioProcessorEditor::OscillatorAudioProcessorEditor (OscillatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //**************************************************************************
    // EXERCISE

    freq.setRange(50,500,1);
    freq.setSliderStyle(juce::Slider::Rotary);
    freq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    
    freq.addListener(this);
    
    freqLabel.setText("Freq",juce::dontSendNotification);
    
    amplitudeLabel.setText("Ampl", juce::dontSendNotification);
    
    amplitude.setRange(0,1,0.1);
    amplitude.setSliderStyle(juce::Slider::Rotary);
    amplitude.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    amplitude.addListener(this);
    
    addAndMakeVisible(freq);
    addAndMakeVisible(freqLabel);
    addAndMakeVisible(amplitude);
    addAndMakeVisible(amplitudeLabel);
    //**************************************************************************

}

OscillatorAudioProcessorEditor::~OscillatorAudioProcessorEditor()
{
}

//==============================================================================
void OscillatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void OscillatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //**************************************************************************
    
    freqLabel.setBounds(10,50,130,20);
    freq.setBounds(10,80,100,100);
    
    amplitudeLabel.setBounds(200,50,130,20);
    amplitude.setBounds(200,80,100,100);
    //**************************************************************************


}
//******************************************************************************
// EXERCISE

void OscillatorAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &freq)
    {
        audioProcessor.setFreq(freq.getValue());
    }
    else if (slider == &amplitude)
    {
        audioProcessor.setAmplitude(amplitude.getValue());
    }
   
}
//******************************************************************************
