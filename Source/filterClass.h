/*
  ==============================================================================

    filterClass.h
    Created: 1 Feb 2024 2:36:44pm
    Author:  Jia En

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterClass  
{
public:
    void setHighpass(bool highpass); // whether we are using high or low pass
    void setCutoffFreq(float cutoff_frequency);
    void setSamplingRate(float sampling_rate);
    
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private:
    bool highpass;
    float cutoff_frequency;
    float sampling_rate;
    std::vector <float> dn_buffer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterClass);
};
