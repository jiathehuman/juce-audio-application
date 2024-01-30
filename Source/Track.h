/*
  ==============================================================================

    Track.h
    Created: 28 Jan 2024 3:05:50pm
    Author:  Jia En

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Track
{
public:
    Track(String btn_name, URL track_url);
    ~Track();
    
    String btn_name;
    URL track_url;

//    void paint (juce::Graphics&) override;
//    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
};
