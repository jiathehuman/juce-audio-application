/*
  ==============================================================================

    filterClass.cpp
    Created: 1 Feb 2024 2:36:44pm
    Author:  Jia En

  ==============================================================================
*/

#include <JuceHeader.h>
#include "filterClass.h"

//==============================================================================
//filterClass::filterClass()
//{
//    // In your constructor, you should add any child components, and
//    // initialise any special settings that your component needs.
//
//}
//
//filterClass::~filterClass()
//{
//}

//void filterClass::paint (juce::Graphics& g)
//{
//    /* This demo code just fills the component's background and
//       draws some placeholder text to get you started.
//
//       You should replace everything in this method with your own
//       drawing code..
//    */
//
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("filterClass", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
//}
//
//void filterClass::resized()
//{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//
//}

void FilterClass::setHighpass(bool highpass)
{
    this -> highpass = highpass;
}
void FilterClass::setCutoffFreq(float cutoff_frequency)
{
    this -> cutoff_frequency = cutoff_frequency;
}
void FilterClass::setSamplingRate(float sampling_rate)
{
    this -> sampling_rate = sampling_rate;
}

void FilterClass::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    constexpr auto PI = 3.14159265359f; // defined hard-code
    dn_buffer.resize(buffer.getNumChannels(), 0.f);
    
    // invert output of allpass by multiplying by its negative
    const auto sign = highpass ? -1.f: 1.f;
    
    // tan
    const auto tan = std::tan(PI*cutoff_frequency/sampling_rate);
    
    // coeeficient is constant
    const auto al = (tan - 1.f)/ (tan + 1.f);
    
    // processing each channel
    for(auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto channelSamples = buffer.getWritePointer(channel);
        for(auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = channelSamples[i];
            // all pass filtering
            const auto all_filtered_sample = al * inputSample + dn_buffer[channel];
            // final filtering, scaling by 0.5 to stay in range
            const auto filterOutput = 0.5f * (inputSample + sign * all_filtered_sample);
            // to output
            channelSamples[i] = filterOutput;
        }
//
        
    }
}
