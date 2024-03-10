/**===============================================================================
@file custom_style.h
@brief Custom style contains all the custom styles for the buttons and sliders.
 With inspiration from:
 The Audio Programmer
 Juce Tutorial 14: https://www.youtube.com/watch?v=iH9c4uNPLwE
 ====================================================================================*/

#pragma once
#include <JuceHeader.h>


namespace juce
{
class CustomStyle  : public LookAndFeel_V4
{
public:
    /**
    Overiding the draw rotary slider to customise the look of rotary sliders by adding a pointer and additional inner circle
    */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override; // to draw the pointer to the angle
};

/**
Custom rotary sliders for the gain, speed and position sliders
*/
class CustomRotarySlider: public Slider
{
    public:
    CustomRotarySlider();
};
/**
Custom vertical sliders for highpass and lowpass
*/
class CustomVerticalSlider: public Slider
{
    public:
    CustomVerticalSlider();
};
/**
Custom play button that toggles between green and red
*/
class PlayButton: public TextButton
{
    public:
    PlayButton();
};
/**
Custom load button for the deckgui and the two load columns in the playlist component
*/
class LoadButton: public TextButton
{
    public:
    LoadButton();
};
/**
Custom confirm button at the bottom of the application
*/
class ConfirmButton: public TextButton
{
    public:
    ConfirmButton();
};

/**
Custom loop button for each track
*/
class LoopButton: public TextButton
{
    public:
    LoopButton();
};

/**
Custom delete buttons for the deleting the track in the playlist component
*/
class DeleteButton: public TextButton
{
    public:
    DeleteButton();
};


}

