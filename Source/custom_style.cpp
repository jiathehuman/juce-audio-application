/*
  ==============================================================================

    custom_style.cpp
    Created: 25 Jan 2024 11:58:34am
    Author:  Jia En

  ==============================================================================
*/

#include <JuceHeader.h>
#include "custom_style.h"

//==============================================================================


namespace juce
{
void CustomStyle::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    //    auto outline = slider.findColour (Slider::rotarySliderOutlineColourId);
    //    auto fill    = slider.findColour (Slider::rotarySliderFillColourId);
    
    auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    
    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin (8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;
    
    // the background path for the rotary slider
    Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    
    
    // New code ----------------------------------------
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius - 10.0f,
                                 arcRadius - 10.0f,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    
    g.setColour (juce::Colour(34, 87, 122));
    g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
    
    // End of new code ----------------------------------------
    
    //    g.fillEllipse(bounds.getCentreX(), bounds.getCentreY(), 20.0f, 20.0f);
    //    g.drawEllipse(bounds.getCentreX(),bounds.getCentreY(),10.0f, 10.0f);
    
    if (slider.isEnabled())
    {
        Path valueArc; // fill path
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);
        
        //        g.setColour (fill);
        g.setColour(juce::Colour(128, 237, 153));
        g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }
    
    // Thumb of rotary slider
    auto thumbWidth = lineW * 1.2f;
    Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));
    
    g.setColour(juce::Colour(56, 163, 165));
    g.fillEllipse (Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
    
    // New code ----------------------------------------
    // add a line; a path is a sequence of lines and curves
    g.drawLine(backgroundArc.getBounds().getCentreX(), backgroundArc.getBounds().getCentreY(),thumbPoint.getX(),thumbPoint.getY(),lineW);
    // End of new code ----------------------------------------
}
CustomRotarySlider::CustomRotarySlider()
{
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentBlack);
    setNumDecimalPlacesToDisplay(3);
    
};

PlayButton::PlayButton()
{
    setClickingTogglesState(true);
    setColour(buttonColourId, juce::Colour(56, 163, 165)); // set blue
    setColour(buttonOnColourId, juce::Colour(242, 149, 89)); // set orange
    
}
}
