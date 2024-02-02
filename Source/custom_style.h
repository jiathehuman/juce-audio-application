/*
  ==============================================================================

    custom_style.h
    Created: 25 Jan 2024 11:58:34am
    Author:  Jia En
    With inspiration from:
    The Audio Programmer 
    Juce Tutorial 14: https://www.youtube.com/watch?v=iH9c4uNPLwE

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
namespace juce
{
class CustomStyle  : public LookAndFeel_V4
{
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override;
};

class CustomRotarySlider: public Slider
{
    public:
    CustomRotarySlider();
};
class CustomVerticalSlider: public Slider
{
    public:
    CustomVerticalSlider();
};

class PlayButton: public TextButton
{
    public:
    PlayButton();
};

}

