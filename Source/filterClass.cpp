/*
  ==============================================================================

    filterClass.cpp
    Created: 1 Feb 2024 2:36:44pm
    Author:  Jia En

  ==============================================================================
*/

#include <JuceHeader.h>
#include "filterClass.h"

// https://forum.juce.com/t/iir-filter-control-and-addition/6652
// https://www.youtube.com/watch?v=xJCt0MVZZ7Q&t=1333s to figure out low and highpass
//==============================================================================

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
    constexpr auto PI = 3.14159265359f; // defined as hard-coded number
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
