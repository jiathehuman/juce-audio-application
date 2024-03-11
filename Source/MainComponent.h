/**===============================================================================
@file MainComponent.h
@brief This file is auto-generated.
 The code is built on the template provided by Professsor Matthew from University of London.
 The main component prepares and processes audio from the two audio players.
 The main component also serves as a button listener for the confirm button.
 ====================================================================================*/


#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
//#include "FFTClass.h"
#include "custom_style.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public Button::Listener
{
public:
    //==============================================================================
    /** Constructor for the MainComponent  */
    MainComponent();
    /** Deconstructor for the MainCOmponent */
    ~MainComponent();

    //==============================================================================
    /** Pepare the transport source*/
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** Calls periodically to fetch the next blocks of audio data **/
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    /** Source releases anything it does not need after playback stopped*/
    void releaseResources() override;

    //==============================================================================
    /**
     Paint code for background
     @param Graphics& reference to the graphics object to draw directly on
     **/
    void paint (Graphics& g) override;
    /** Arrange and draw the child components **/
    void resized() override;
    
    //==============================================================================

    //==============================================================================
    /** gets the pointer of track_1 and track_2 from playlist and drops it in filesDropped of respective GUI*/
    void setTrack();
    /** Registers button click of the confirmation button */
    void buttonClicked(Button*) override;
    //==============================================================================


private:
    //==============================================================================
     
    AudioFormatManager formatManager; // keeps a list of audio format and decides which to use
    AudioThumbnailCache thumbCache{100};

    /** The two DJAudioPlayer objects and the respective DeckGUIs*/
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache}; 

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};


    /** The audio source that mixes the output of player1 and player2 */
    MixerAudioSource mixerSource;
    
    /** Instantiates the playlist object */
    PlaylistComponent playlistComponent;
    
    /** Instatiates the custom stype class */
    juce::CustomStyle custom_style;
    
    /** Custom confirm buttom at the bottom */
    juce::ConfirmButton confirm_selection{};
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
