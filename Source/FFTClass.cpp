/*
  ==============================================================================

    FFTClass.cpp
    Created: 25 Feb 2024 6:09:28pm
    Author:  Jia En

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FFTClass.h"

//==============================================================================
FFTClass::FFTClass(): spectrogramImage (Image::RGB, 512, 512, true),
forwardFFT (fftOrder)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

FFTClass::~FFTClass()
{
}

void FFTClass::loadURL(URL audioURL)
{
  if (fileLoaded)
  {
    repaint();
  }
  else {
    std::cout << "File not loaded! " << std::endl;
  }

}

void FFTClass::drawNextLineOfSpectrogram()
{
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight   = spectrogramImage.getHeight();

    // first, shuffle our image leftwards by 1 pixel..
    spectrogramImage.moveImageSection (0, 0, 1, 0, rightHandEdge, imageHeight);

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);

    // find the range of values produced, so we can scale our rendering to
    // show up the detail clearly
    auto maxLevel = FloatVectorOperations::findMinAndMax (fftData, fftSize / 2);
    
    std::cout << "FFTClass::drawNextLineOfSpectrogram() is called" << std::endl;

    for (auto y = 1; y < imageHeight; ++y)
    {
        auto skewedProportionY = 1.0f - std::exp (std::log ((float) y / (float) imageHeight) * 0.2f);
        auto fftDataIndex = jlimit (0, fftSize / 2, (int) (skewedProportionY * (int) fftSize / 2));
        auto level = jmap (fftData[fftDataIndex], 0.0f, jmax (maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

        spectrogramImage.setPixelAt (rightHandEdge, y, Colour::fromHSV (level, 1.0f, level, 1.0f));
    }
}

void FFTClass::timerCallback()
{
    if (nextFFTBlockReady)
    {
        drawNextLineOfSpectrogram();
        nextFFTBlockReady = false;
        repaint();
    }
}

void FFTClass::pushNextSampleIntoFifo (float sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that the next line should now be rendered..
    
    std::cout << "FFTClass::pushNextSampleIntoFifo (float sample) is called" << std::endl;
    
    if (fifoIndex == fftSize)
    {
        if (! nextFFTBlockReady)
        {
            zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}


void FFTClass::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//    g.fillAll(Colours::pink);
    
    g.setOpacity(1.0f);
    g.drawImage(spectrogramImage, getLocalBounds().toFloat());

}

void FFTClass::resized(juce::Graphics& g)
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colour(242, 212, 146));

}














