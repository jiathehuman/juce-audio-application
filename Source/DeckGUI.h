/**===============================================================================
@file DeckGUI.h
@brief The deckGUI serves as the interface between the button/slider components and the DJ audio player.
 The code is built on the template provided by Professsor Matthew from University of London.
 ====================================================================================*/

#pragma once
//#include "../JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "custom_style.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    /**
    Constructor for the deckGUI component
    @param player pointer to the DJAudioPlayer the DeckGUI associates with
    @param formatManagerToUse the audio format manager used by DJAudioPlayer
     @param cacheToUse the cache object runs on a background thread, maintaining low-res previews in memory
    */
    DeckGUI(DJAudioPlayer* player,
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    
    /** Destructor for the deckGUI component **/
    ~DeckGUI();

    /**
     Paint code for background
     @param Graphics& reference to the graphics object to draw directly on
     **/
    void paint (Graphics&) override;
    
    /** Arrange and draw the child components **/
    void resized() override;

     /** Implement Button::Listener */
    void buttonClicked (Button *) override;

    /** Implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    /** True indicating the component is interested in receiving a file */
    bool isInterestedInFileDrag (const StringArray &files) override;
    
    /** Loads the player with 1 url path from a list of strings */
    void filesDropped (const StringArray &files, int x, int y) override;
    
    /**
     Overloaded function, loads player and waveform display with URL
     Resets the play button if the player was in the middle of playing a song
     @param url the path to the audio track
     **/
    void filesDropped (URL url);

    /** Callback routine that gets called periodically **/
    void timerCallback() override;
    
    /** Called when the load button is clicked
     Loads to the player the url path of the audio path chosen by the user
     @param url the URL path to the audio file**/
    void loadToPlayer(URL url);

private:
    // New code --------------------------------------------------------------------------------
    /** Play, load and loop custom text buttons*/
    juce::PlayButton playButton{};
    juce::LoadButton loadButton{};
    juce::LoopButton loopButton{};
    
    /** Volume, speed and position custom rotary sliders **/
    juce::CustomRotarySlider volSlider;
    juce::CustomRotarySlider speedSlider;
    juce::CustomRotarySlider posSlider;

    /** Low and High Pass custom vertical sliders*/
    juce::CustomVerticalSlider lowSlider;
    juce::CustomVerticalSlider highSlider;
    
    /** Text drawn to show the duration of the track */
    DrawableText track_duration;
    
    /** Calculates duration into minutes and seconds and set the text of the track_duration Drawable Text*/
    void loadText();
    
    /** Sets if the audio player is looping or not looping a file */
    void setLoop();
    
    /** True if the player is to loop the file, false if file stops when finished */
    bool looping;
    
    /** Labels for the rotary sliders and vertical sliders*/
    Label volLabel{"volLabel", "Gain"};
    Label speedLabel{"speedLabel", "Speed"};
    Label posLabel{"posLabel", "Pos"};
    Label passLabel{"passLabel", "Lo Pass"};
    Label highPassLabel{"loopLabel", "Hi Pass"};
    Label loopLabel{"loopLabel", "Loop"};
    
    // End ofnew code ------------------------------------------------------------------------

    /** Chooser for the file when load button is clicked */
    FileChooser fChooser{"Select a file..."};

    /** Instantiate the WaveformDisplay object */
    WaveformDisplay waveformDisplay;

    /** A pointer to the DJAudioPlayer associated with the DeckGUI*/
    DJAudioPlayer* player;
    
    /** The string containing the duration in a minute:second format*/
    String displayed_duration; // new code


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
