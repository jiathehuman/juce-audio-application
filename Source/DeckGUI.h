/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew (Code provided on Coursera)

  ==============================================================================
*/

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
    TextButton loadButton{"LOAD"};
    
    juce::CustomRotarySlider volSlider;
    juce::CustomRotarySlider speedSlider;
    juce::CustomRotarySlider posSlider;

    juce::CustomVerticalSlider lowSlider;
    

    
    
    Label volLabel{"volLabel", "Gain"};
    Label speedLabel{"speedLabel", "Speed"};
    Label posLabel{"posLabel", "Pos"};
    Label passLabel{"passLabel", "Lo/Hi Pass"};

    FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
