/**===============================================================================
 @file custom_style.cpp
 @brief The CPP file implements the functions promised in the header file.
 ====================================================================================*/


#include <JuceHeader.h>
#include "custom_style.h"

namespace juce
{
void CustomStyle::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    
    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin (8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;
    
    
    // https://forum.juce.com/t/is-there-a-way-to-create-radial-gradients-for-ellipses/58048
    
    // New code -----------------------------------------------------------------------------------
    /** Gradient on the rotary sliders*/
    juce::ColourGradient gradient{ juce::Colour(34, 87, 122),
               bounds.getBottomRight(),
        juce::Colour(0,0,0).withSaturation(1.0f),
               bounds.getTopLeft(), true };
    
    auto transform = juce::AffineTransform::scale(0.5f, 1.0f).translated(bounds.getWidth() * 0.25f, bounds.getHeight() * -0.25f);
    
           auto fill = juce::FillType{ gradient }.transformed(transform);
    
    g.setFillType(fill);
    
    g.fillEllipse(bounds.getCentreX() - arcRadius,
                  bounds.getCentreY() - arcRadius,
                  arcRadius * 2,
                  arcRadius * 2);
    
    // End of new code ------------------------------------------------------------------------------

    
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
    
    
    // New code -----------------------------------------------------------------------------------
    /** Adding a centered smaller arc as an additional detail */
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius - 10.0f,
                                 arcRadius - 10.0f,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    
    /** Set Colour as a dark blue*/
    g.setColour (juce::Colour(34, 87, 122));
    /** Draw the path*/
    g.strokePath (backgroundArc, PathStrokeType (lineW - 5.0f, PathStrokeType::curved, PathStrokeType::rounded));
    
    // End of new code -----------------------------------------------------------------------------
    
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
    
        g.setColour(juce::Colour(128, 237, 153)); // new code - setting a new colour
        g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }
    
    // Thumb of rotary slider
    auto thumbWidth = lineW * 1.2f;
    Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));
    
    g.setColour(juce::Colour(56, 163, 165));
    g.fillEllipse (Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
    
    // New code -----------------------------------------------------------------------------------
    // add a line; a path is a sequence of lines and curves
    g.drawLine(backgroundArc.getBounds().getCentreX(), backgroundArc.getBounds().getCentreY(),thumbPoint.getX(),thumbPoint.getY(),lineW);
    // End of new code ----------------------------------------------------------------------------
}

// New code ---------------------------------------------------------------------------------------
CustomRotarySlider::CustomRotarySlider() // custom textbox style
{
    /** set all styles to be rotary horizontal drag **/
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    /** set the text box to be below **/
    setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    /** set the text box to have no outline **/
    setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentBlack);
    /** txt box truncates to 3 decimal places**/
    setNumDecimalPlacesToDisplay(3);
    
};

PlayButton::PlayButton() // transparent outline and customised toggle on/of colours
{
    /** button have no outline */
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    /** button can toggle*/
    setClickingTogglesState(true);
    setColour(buttonOnColourId, juce::Colour(242, 149, 89)); // set orange for 'stop'
    setColour(buttonColourId, juce::Colour(118,227,143)); // sets green for 'play'
    setColour(textColourOffId, juce::Colours::black); // set text black
}

LoadButton::LoadButton()
{
    /** button have no outline */
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    /** button cannot toggle*/
    setClickingTogglesState(false);
    setButtonText("LOAD"); // the button text 
    setColour(buttonColourId, juce::Colour(38, 70, 83)); // set button to be blue
    setColour(textColourOnId, juce::Colours::white); // set text white
}



LoopButton::LoopButton()
{
    /** button have no outline */
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    /** button can toggle*/
    setClickingTogglesState(true);
    setColour(buttonOnColourId, juce::Colour(40, 114, 113)); // blue
    setColour(buttonColourId, juce::Colour(50, 124, 123)); // lighter blue
    setColour(textColourOffId, juce::Colours::white);
    
    
}

ConfirmButton::ConfirmButton()
{
    /** button have no outline */
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    /** button cannot toggle*/
    setClickingTogglesState(false);
    setButtonText("CONFIRM SELECTION");
    setColour(buttonColourId, juce::Colour(138, 177, 125)); // set blue
    setColour(textColourOffId, juce::Colours::black); // set text black
}

DeleteButton::DeleteButton()
{
    /** button have no outline */
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    setClickingTogglesState(false);
    /** button cannot toggle*/
    setButtonText("DELETE");
    setColour(buttonColourId, juce::Colour(32, 44, 57)); // set blue
    setColour(textColourOnId, juce::Colours::white); // set text white
}


CustomVerticalSlider::CustomVerticalSlider()
{
    /** set the text box value to be below */
    setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    /** text bix have no outline */
    setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentBlack);
    setColour(Slider::ColourIds::thumbColourId, juce::Colour(128, 237, 153)); // neon green
    setColour(Slider::ColourIds::trackColourId, juce::Colour(231, 111, 81)); // orange
    setNumDecimalPlacesToDisplay(3);
    
};
}

// End of new code ---------------------------------------------------------------------------------
