 /**===============================================================================
 @file WaveformDisplay.cpp
 @brief The CPP file implements the functions promised in the header file.
  ====================================================================================*/


#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    // set the audio thumbnail as a change listener
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


    g.setColour(juce::Colour(233, 196, 106)); // colour of the audio thumbnail
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else // file is not loaded, draw some placeholder text
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);

    }
}

void WaveformDisplay::resized()
{
 // no child components to set bounds for
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear(); // resets source and clear channel data
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL)); // true
  if (fileLoaded) // file loaded successfully
  {
    repaint(); // paint the thumbnail
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    repaint(); // continuously paint
}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position) // if the position has changed
  {
    position = pos; // update the position
    repaint();
  }

}




