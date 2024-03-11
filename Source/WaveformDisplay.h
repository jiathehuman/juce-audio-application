
/**===============================================================================
@file WaveformDisplay.h
@brief The waveform display uses the audio thumbnail object to show a preview of the file.
 The code is built on the template provided by Professsor Matthew from University of London.
 ====================================================================================*/
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    /**
    Constructor for the waveform display
    @param formatManagerToUse the AudioFormatManager for the audio application
    @param cacheToUse the cache object runs on a background thread, maintaining low-res previews in memory
    */
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    
    /** Destructor for waveform display **/
    ~WaveformDisplay();

    /** Paint method for the component */
    void paint (Graphics&) override;
    
    /** Resize method for the component */
    void resized() override;

    /**
    Method to receive the callback
    @param source a pointer to the Chagebroadcaster triggering the callback
    */
    void changeListenerCallback (ChangeBroadcaster *source) override;

    /**
    Sets the source for the audiothumbnail object
    @param audioURL the URL of the new source
    */
    void loadURL(URL audioURL);

    /**
    Sets the position relative to the playhead
    @param pos the position of the playhead
    */
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb; // instantiate audioThumbnail
    bool fileLoaded; // checks if the file is loaded, if it is, call repaint()
    double position; // relative position
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
