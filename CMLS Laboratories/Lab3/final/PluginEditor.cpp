/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MIDIVelocityModifyAudioProcessorEditor::MIDIVelocityModifyAudioProcessorEditor (MIDIVelocityModifyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // setSize (400, 300);
    setSize(200, 200);


    
    //********************************************************************************************//
    // 2) Add the sliders to the constructor defining the parameters and making it visible
    midiVolume.setSliderStyle (juce::Slider::LinearBarVertical); 
    midiVolume.setRange(0.0, 127.0, 1.0); 
    midiVolume.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0); 
    midiVolume.setPopupDisplayEnabled (true, false, this); 
    midiVolume.setTextValueSuffix ("Volume"); 
    midiVolume.setValue(1.0);
    addAndMakeVisible (&midiVolume);
    //********************************************************************************************//
    
    //********************************************************************************************//
    // 7) add the listener to  midiVolume slider
    midiVolume.addListener (this);
    //********************************************************************************************//

}

MIDIVelocityModifyAudioProcessorEditor::~MIDIVelocityModifyAudioProcessorEditor()
{
}

//==============================================================================
void MIDIVelocityModifyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Modify Velocity", getLocalBounds(), juce::Justification::centred, 1);
}

void MIDIVelocityModifyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //********************************************************************************************//
    // 3) Specify the position of the slider
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    //********************************************************************************************//


}

//********************************************************************************************//
// 8) Define the function that will be called whenever the slider value changes: it simply access
// to the noteOnVel variable and update it
void MIDIVelocityModifyAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();

}
//********************************************************************************************//
