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
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 
    void filesDropped (URL url);

    void timerCallback() override; 
    
    void loadToPlayer(URL url);

private:

    juce::PlayButton playButton{};
//    TextButton stopButton{"STOP"};
//    juce::LoadButton loadButton{"LOAD"};
    juce::LoadButton loadButton{};
    
    juce::LoopButton loopButton{};
    
    juce::CustomRotarySlider volSlider;
    juce::CustomRotarySlider speedSlider;
    juce::CustomRotarySlider posSlider;

    juce::CustomVerticalSlider lowSlider;
    
    juce::CustomVerticalSlider highSlider;
    
    DrawableText track_duration;
    
    void loadText();
    void setLoop();
    
    bool looping;
    

    
    
    Label volLabel{"volLabel", "Gain"};
    Label speedLabel{"speedLabel", "Speed"};
    Label posLabel{"posLabel", "Pos"};
    Label passLabel{"passLabel", "Lo Pass"};
    Label highPassLabel{"loopLabel", "Hi Pass"};
    Label loopLabel{"loopLabel", "Loop"};

    FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 
    
    String displayed_duration;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
