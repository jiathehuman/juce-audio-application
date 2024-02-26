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
    auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    
    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin (8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;
    

    
//    g.setGradientFill(ColourGradient(Colours::blue,bounds.getX() - 100,bounds.getY() - 100, Colours::red, bounds.getCentreX(), bounds.getCentreY(), false));
    
    // https://forum.juce.com/t/is-there-a-way-to-create-radial-gradients-for-ellipses/58048
//    juce::ColourGradient gradient{ juce::Colour(233, 196, 106),
//               bounds.getBottomRight(),
//        juce::Colour(133, 96, 6).withSaturation(1.0f).withBrightness(1.2f),
//               bounds.getTopLeft(), true };
    
    juce::ColourGradient gradient{ juce::Colour(34, 87, 122),
               bounds.getBottomRight(),
        juce::Colour(0,0,0).withSaturation(1.0f),
               bounds.getTopLeft(), true };
    
//           auto transform = juce::AffineTransform::scale(0.5f, 1.5f).
//               translated(bounds.getWidth() * 0.25f, bounds.getHeight() * -0.25f);
    
    auto transform = juce::AffineTransform::scale(0.5f, 1.0f).translated(bounds.getWidth() * 0.25f, bounds.getHeight() * -0.25f);
    
           auto fill = juce::FillType{ gradient }.transformed(transform);
    
    g.setFillType(fill);
    
    g.fillEllipse(bounds.getCentreX() - arcRadius,
                  bounds.getCentreY() - arcRadius,
                  arcRadius * 2,
                  arcRadius * 2);
//    g.setColour(Colours::black);

    

    
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
    g.strokePath (backgroundArc, PathStrokeType (lineW - 5.0f, PathStrokeType::curved, PathStrokeType::rounded));
    
    // End of new code ----------------------------------------
    
    
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

//void CustomStyle::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
//                                       float sliderPos,
//                                       float minSliderPos,
//                                       float maxSliderPos,
//                                       const Slider::SliderStyle style, Slider& slider)
//{
//    if (slider.isBar())
//    {
//        g.setColour (slider.findColour (Slider::trackColourId));
//        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
//                                          : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));
//
//        drawLinearSliderOutline (g, x, y, width, height, style, slider);
//    }
//    else
//    {
//        auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
//        auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);
//
////        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);
//        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 2.5f);
//
//
//        Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
//                                 slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));
//
//        Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
//                               slider.isHorizontal() ? startPoint.y : (float) y);
//
//        Path backgroundTrack;
//        backgroundTrack.startNewSubPath (startPoint);
//        backgroundTrack.lineTo (endPoint);
////        g.setColour (slider.findColour (Slider::backgroundColourId));
//        g.setColour (juce::Colour(46, 153, 155));
//        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
//
//        Path valueTrack;
//        Point<float> minPoint, maxPoint, thumbPoint;
//
//        if (isTwoVal || isThreeVal)
//        {
//            minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
//                         slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };
//
//            if (isThreeVal)
//                thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
//                               slider.isHorizontal() ? (float) height * 0.5f : sliderPos };
//
//            maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
//                         slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
//        }
//        else
//        {
//            auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
//            auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;
//
//            minPoint = startPoint;
//            maxPoint = { kx, ky };
//        }
//
//        auto thumbWidth = getSliderThumbRadius (slider);
//
//        valueTrack.startNewSubPath (minPoint);
//        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
//        g.setColour (slider.findColour (Slider::trackColourId));
//        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
//
//        if (! isTwoVal)
//        {
//            g.setColour (slider.findColour (Slider::thumbColourId));
//            g.fillEllipse (Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint));
//        }
//
//        if (isTwoVal || isThreeVal)
//        {
//            auto sr = jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
//            auto pointerColour = slider.findColour (Slider::thumbColourId);
//
//            if (slider.isHorizontal())
//            {
//                drawPointer (g, minSliderPos - sr,
//                             jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f),
//                             trackWidth * 2.0f, pointerColour, 2);
//
//                drawPointer (g, maxSliderPos - trackWidth,
//                             jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f),
//                             trackWidth * 2.0f, pointerColour, 4);
//            }
//            else
//            {
//                drawPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f),
//                             minSliderPos - trackWidth,
//                             trackWidth * 2.0f, pointerColour, 1);
//
//                drawPointer (g, jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr,
//                             trackWidth * 2.0f, pointerColour, 3);
//            }
//        }
//
//        if (slider.isBar())
//            drawLinearSliderOutline (g, x, y, width, height, style, slider);
//    }
//}
CustomRotarySlider::CustomRotarySlider()
{
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentBlack);
    setNumDecimalPlacesToDisplay(3);
    
};

PlayButton::PlayButton()
{
    // reference: https://forum.juce.com/t/how-to-set-the-colour-of-the-outline-on-a-textbutton/37925/3
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    setClickingTogglesState(true);
//    setColour(buttonColourId, juce::Colour(56, 163, 165)); // set blue
    setColour(buttonOnColourId, juce::Colour(242, 149, 89)); // set orange
    // 128, 237, 153)
    setColour(buttonColourId, juce::Colour(118,227,143));
    setColour(textColourOffId, juce::Colours::black); // set text black

//    setColour(buttonColourId, juce::Colour(42, 157, 143));
    
}

LoadButton::LoadButton()
{
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    setClickingTogglesState(false);
    setButtonText("LOAD");
    setColour(buttonColourId, juce::Colour(38, 70, 83)); // set blue
    setColour(textColourOffId, juce::Colours::white); // set text black    
}

LoopButton::LoopButton()
{
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    setClickingTogglesState(true);
//    setColour(buttonColourId, juce::Colour(56, 163, 165)); // set blue
    setColour(buttonOnColourId, juce::Colour(40, 114, 113)); // set orange
    // 128, 237, 153)
    setColour(buttonColourId, juce::Colour(30, 104, 103));
    setColour(textColourOffId, juce::Colours::black); // set text black
    

//    setColour(buttonColourId, juce::Colour(42, 157, 143));
    
}

ConfirmButton::ConfirmButton()
{
    setColour(ComboBox::outlineColourId, juce::Colours::transparentBlack);
    setClickingTogglesState(false);
    setButtonText("CONFIRM SELECTION");
    setColour(buttonColourId, juce::Colour(138, 177, 125)); // set blue
    setColour(textColourOffId, juce::Colours::black); // set text black
}


CustomVerticalSlider::CustomVerticalSlider()
{
    setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    setColour(Slider::ColourIds::textBoxOutlineColourId, Colours::transparentBlack);
    setColour(Slider::ColourIds::thumbColourId, juce::Colour(128, 237, 153));
    setColour(Slider::ColourIds::trackColourId, juce::Colour(231, 111, 81));
    setNumDecimalPlacesToDisplay(3);
    
};
}
