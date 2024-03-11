/** ===============================================================================
@file MainComponent.cpp
@brief The CPP file implements the functions promised in the header file.
 =====================================================================================*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

    setSize (800, 600); // the size of the whole thing
    
    // use the CustomStyle class created
    juce::LookAndFeel::setDefaultLookAndFeel(&custom_style);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    /** add and make visible the two deckGUIs, the playlist component and the confirm selection button*/
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(confirm_selection);
    
    formatManager.registerBasicFormats(); // register basic formats
    confirm_selection.addListener(this);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr); // set look and feel back to nullpointer
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Prepares two DJAudioPlayers
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // Prepares mixerSource
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Adds reference to two DJAudioPlayers as inputs
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    
    std::cout << "MainComponent::prepareToPlay" << std::endl;

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill); // periodically called
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour(Colours::white); // text colour
    g.setFont(14.0f);
}

void MainComponent::resized()
{
    double rowH = getHeight()/9;

    // Position of the deckGUI
    deckGUI1.setBounds(0, 0, getWidth()/2, rowH * 5);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, rowH * 5);
    
    // Position of the playlist component and confirm button
    playlistComponent.setBounds(0, rowH * 5, getWidth(), rowH * 3);
    confirm_selection.setBounds(0,rowH * 8, getWidth(),rowH );
    
}
// New code -----------------------------------------------------------------------------------------
void MainComponent::buttonClicked(Button*)
{
    setTrack(); // calls set track below to load the tracks to deckGUI
}


void MainComponent::setTrack()
{
    /** get the Track from playlist */
    Track* track_1 = playlistComponent.loadToPlayer("player1"); 
    Track* track_2 = playlistComponent.loadToPlayer("player2");
    deckGUI1.filesDropped(track_1 -> track_url); // filesDropped passes track to the first audio player
    deckGUI2.filesDropped(track_2 -> track_url); // filesDropped passes track to the second audio player
}
// End of new code ------------------------------------------------------------------------------------


