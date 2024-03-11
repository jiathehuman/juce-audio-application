/** ===============================================================================
@file DeckGUI.cpp
@brief The CPP file implements the functions promised in the header file.
 =====================================================================================*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
                 ) : looping(false), 
waveformDisplay(formatManagerToUse, cacheToUse), player(_player)
{

    /** make all buttons and make it visible*/
    addAndMakeVisible(playButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
       
    /** make all sliders  and make it visible*/
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(lowSlider);
    addAndMakeVisible(highSlider);
    
    /** make all labels  and make it visible*/
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(passLabel);
    addAndMakeVisible(loopLabel);

    /** add and make waveformDisplay visible */
    addAndMakeVisible(waveformDisplay);
    
    /** add and make track_duration visible*/
    addAndMakeVisible(track_duration);
    
    // New code --------------------------------------------------------------------------------------
    /** set the style of the two filter sliders as linear vertical */
    lowSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    highSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    // End of new code -------------------------------------------------------------------------------

    /** add listeners to all the buttons*/
    playButton.addListener(this);
    loopButton.addListener(this);
    loadButton.addListener(this);
    
    /** add listeners to all the sliders **/
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    lowSlider.addListener(this);
    highSlider.addListener(this);
    
    /** set the text for play and loop */
    playButton.setButtonText("PLAY");
    loopButton.setButtonText("LOOP");
    
    // New code ---------------------------------------------------------------------------------------

    /** set the ranges for all the sliders */
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.5, 5.0);
    posSlider.setRange(0.0, 1.0);
    lowSlider.setRange(10.0, 20000.0, 20);
    highSlider.setRange(10.0, 10000.0, 20);

    /** attach all the labels to respective components */
    volLabel.attachToComponent(&volSlider, false);
    speedLabel.attachToComponent(&speedSlider, false);
    posLabel.attachToComponent(&posSlider, false);
    passLabel.attachToComponent(&lowSlider, false);
    loopLabel.attachToComponent(&loopButton, false);
    highPassLabel.attachToComponent(&highSlider, false);
    
    /** arrange all the labels to be centred */
    volLabel.setJustificationType(Justification::centred);
    speedLabel.setJustificationType(Justification::centred);
    posLabel.setJustificationType(Justification::centred);
    passLabel.setJustificationType(Justification::centred);
    loopLabel.setJustificationType(Justification::centred);
    highPassLabel.setJustificationType(Justification::centred);

    /** set the colour of 'duration' text to be gold */
    track_duration.setColour(juce::Colour(233, 196, 106));
    // End of code -------------------------------------------------------------------------------------

    startTimer(500); // start the timer
    
}

DeckGUI::~DeckGUI()
{
    stopTimer(); // when deconstructing deckGUI, stop the timer
}

void DeckGUI::paint (Graphics& g)
{
    /** draw a black outline of a box around the deckGUI */
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);
    
    loadText(); // the 'duration' of the track
    g.setColour (juce::Colour(32, 44, 57));
    g.setFont (14.0f);
}

void DeckGUI::resized()
{
    // New code ---------------------------------------------------------------------------------------
    Rectangle<int> bounds = getLocalBounds();
    
    auto rowH = getHeight()/7;
    
    // 3 main FlexBox, with height of different number of rowHs -
    // flexbox_buttons, flexbox_sliders, flexbox_additional
    // each of the FlexBox has a different number of FlexItems objects (eg. 3 buttons)

    
    // First section contains buttons and takes up 1 row
    FlexBox flexbox_buttons;
    flexbox_buttons.flexDirection = FlexBox::Direction::row;
    flexbox_buttons.flexWrap = FlexBox::Wrap::wrap;
    flexbox_buttons.alignContent = FlexBox::AlignContent::spaceAround;
    
    // Second section contains sliders and takes up 4 rows
    FlexBox flexbox_sliders;
    flexbox_sliders.flexDirection = FlexBox::Direction::row;
    flexbox_sliders.flexWrap = FlexBox::Wrap::wrap;
    flexbox_sliders.alignContent = FlexBox::AlignContent::center; // sliders are aligned in center
    flexbox_sliders.justifyContent = FlexBox::JustifyContent::spaceAround;

    // Third section contains the waveform and takes up 2 rows
    FlexBox flexbox_additional;
    flexbox_additional.flexDirection = FlexBox::Direction::row;
    flexbox_additional.flexWrap = FlexBox::Wrap::wrap;
    flexbox_additional.alignContent = FlexBox::AlignContent::stretch; // only 1 component
    
    // JUCE array object acts like a vector
    Array<FlexItem> play_load_loop_buttons;
    
    /** Each button takes up 1/3 of the width of the row */
    play_load_loop_buttons.add(FlexItem(getWidth()/3,rowH,playButton));
    play_load_loop_buttons.add(FlexItem(getWidth()/3,rowH,loadButton));
    play_load_loop_buttons.add(FlexItem(getWidth()/3,rowH,loopButton));
    
    flexbox_buttons.items = play_load_loop_buttons;
    
    /** buttons takes up first row, returns the bottom of that */
    flexbox_buttons.performLayout(bounds.removeFromTop(rowH));
    
    // JUCE array object acts like a vector
    Array<FlexItem> sliders_array;
    /** Each o the 5 slider takes up 1/6 of the width of the row , giving a bit of gap in between */
    sliders_array.add(FlexItem(getWidth()/6,100,volSlider));
    sliders_array.add(FlexItem(getWidth()/6,100,speedSlider));
    sliders_array.add(FlexItem(getWidth()/6,100,posSlider));
    sliders_array.add(FlexItem(getWidth()/6,100,lowSlider));
    sliders_array.add(FlexItem(getWidth()/6,100,highSlider));
    
    track_duration.setBoundingBox(
                                   Parallelogram<float>(
                                                 Rectangle<float> (50, rowH * 4 - 10, getWidth()/2, getHeight() * 0.2)));
    


    flexbox_sliders.items = sliders_array;
    flexbox_sliders.performLayout(bounds.removeFromTop(rowH * 4));
    
    // additional components
    Array<FlexItem> additional_components;
    additional_components.add(FlexItem(getWidth(),100,waveformDisplay)); // the waveform display
    flexbox_additional.items = additional_components;
    flexbox_additional.performLayout(bounds.removeFromTop(rowH * 2));
    
    // End of new code ----------------------------------------------------------------------------------

}

void DeckGUI::buttonClicked(Button* button)
{
    // when play button is clicked, make the player start or stop, depending on button state
    if (button == &playButton)
    {
        if((button -> getToggleState()) == true)
        {
            button -> setButtonText("STOP");
        }
        else if(((button -> getToggleState()) == false))
        {
            button -> setButtonText("PLAY");
        }
        player->start();
    }

    // when load button is clicked, allow the user to choose an audio track, load it to player and waveform display
    
       if (button == &loadButton)
       {
            auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
            fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                loadToPlayer(URL{chooser.getResult()});
                waveformDisplay.loadURL(URL{chooser.getResult()});

            });
       }
    // New code ---------------------------------------------------------------------------------------
    // when the loop button is clicked, toggle the button text and call setLoop()
    if(button == &loopButton)
    {
        setLoop();
        if((button -> getToggleState()) == true)
        {
            button -> setButtonText("LOOP");
        }
        else if(((button -> getToggleState()) == false))
        {
            button -> setButtonText("NO LOOP");
        }
        std::cout << "loop clicked" << std::endl;
    }
    // End of new code -------------------------------------------------------------------------------
}

/** registers slider events */
void DeckGUI::sliderValueChanged (Slider *slider)
{
    // when volume slider is altered, use the value from the slider and set the gain of the track
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue()); // set gain using player
    }
    // when speed slider is altered, use the value from the slider and set the speed of the track
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue()); // set speed using player
    }
    // when pos slider is altered, use the value from the slider and set the position of the track
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue()); // set position using player
    }
    
    // New code ---------------------------------------------------------------------------------------
    // when low slider is altered, apply the low pass
    if (slider == &lowSlider)
    {
        player->setLowPass(slider -> getValue()); // set low pass with player's iir audio source
    }
    // when high pass slider is altered, apply the high pass
    if (slider == &highSlider)
    {
        player->setHighPass(slider -> getValue()); // set high pass with player's iir audio source
    }
    // End of new code --------------------------------------------------------------------------------
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  return true; // deckgui is interested in file drag
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  if (files.size() == 1) // if there is only 1 file
  {
    player->loadURL(URL{File{files[0]}}); // load the player with the URL path to that track
      
  }
}

// New code ---------------------------------------------------------------------------------------
void DeckGUI::filesDropped (URL url)
{
    // reset the play button
    if(playButton.getToggleState() == true) playButton.triggerClick();

    player->loadURL(URL{url}); // load the player with the URL path to that track
    waveformDisplay.loadURL(URL{url}); // load the waveform display

}
// End of new code --------------------------------------------------------------------------------

void DeckGUI::timerCallback() // called periodically
{
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}


void DeckGUI::loadToPlayer(URL url)
{
    player->loadURL(URL{url}); // load the file to the player (playlist)
}

// New code ---------------------------------------------------------------------------------------
void DeckGUI::loadText()
{
    int duration = player -> setDuration(); // get the duration from the player in seconds
    /** change the duration into a 00:00 format */
    int minutes = floor(duration / 60); // get the minutes
    int seconds = duration - (minutes * 60);
    
    // set the string for the duration to display
    displayed_duration = "DURATION: " + std::to_string(minutes) + ":" + std::to_string(seconds);
    
    // set the drawable text
    track_duration.setText(displayed_duration);
}

void DeckGUI::setLoop()
{
    looping = !looping; // flip the loop
    player -> looping = looping; // set the looping with the new boolean values
    std::cout << "give me the button toggle state" << looping << std::endl;
}
// End of new code -----------------------------------------------------------------------------------
