/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
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

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    formatManager.registerBasicFormats();
    addAndMakeVisible(confirm_selection);
    confirm_selection.addListener(this);
//    playlistComponent.addChangeListener(this);
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
//    dsp::ProcessSpec spec;
//    spec.sampleRate = sampleRate;
//    spec.maximumBlockSize = samplesPerBlockExpected;
//    spec.numChannels = 2;

    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    
    std::cout << "MainComponent::prepareToPlay" << std::endl;
    
    // new code
    reverb_buffer.setSize(8, samplesPerBlockExpected); // change the buffer size yo 16

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto buffer_length = bufferToFill.buffer -> getNumSamples();
    mixerSource.getNextAudioBlock(bufferToFill);
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(), Justification::centred, true);
    
}

void MainComponent::resized()
{
    double rowH = getHeight()/6;
    deckGUI1.setBounds(0, 0, getWidth()/2, rowH * 4);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, rowH * 4);
    playlistComponent.setBounds(0, rowH * 4, getWidth(), rowH);
    confirm_selection.setBounds(0,rowH * 5, getWidth(),rowH);
}

void MainComponent::setTrack()
{
    Track* track_1 = playlistComponent.loadToPlayer("player1_btn");
    Track* track_2 = playlistComponent.loadToPlayer("player2_btn");
    deckGUI1.filesDropped(track_1 -> track_url);
    deckGUI2.filesDropped(track_2 -> track_url);
}


void MainComponent::buttonClicked(Button*)
{
    setTrack();
}

