/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  Matthew (Code provided on Coursera)

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
                 ) : looping(false), 
waveformDisplay(formatManagerToUse, cacheToUse), player(_player)
{

    addAndMakeVisible(playButton);
//    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(lowSlider);
    
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(passLabel);
//    addAndMakeVisible(highpass_Slider);
    
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(loopLabel);
    lowSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);


    playButton.addListener(this);
    playButton.setButtonText("PLAY");
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    lowSlider.addListener(this);
    
//    loopButton.onClick = [this] {setLoop();};
    
    loopButton.addListener(this);
    loopButton.setButtonText("LOOP");


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.5, 5.0);
    posSlider.setRange(0.0, 1.0);
    lowSlider.setRange(10.0, 20000.0,20);

                            

    volLabel.attachToComponent(&volSlider, false);
    speedLabel.attachToComponent(&speedSlider, false);
    posLabel.attachToComponent(&posSlider, false);
    passLabel.attachToComponent(&lowSlider, false);
    loopLabel.attachToComponent(&loopButton, false);
    
    volLabel.setJustificationType(Justification::centred);
    speedLabel.setJustificationType(Justification::centred);
    posLabel.setJustificationType(Justification::centred);
    passLabel.setJustificationType(Justification::centred);
    loopLabel.setJustificationType(Justification::centred);


    addAndMakeVisible(track_duration);
    track_duration.setColour(juce::Colour(233, 196, 106));


    startTimer(500);
    
//    loadText();


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    
//    g.fillAll(juce::Colour(40, 56, 69));

    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    loadText();
    g.setColour (juce::Colour(32, 44, 57));
    g.setFont (14.0f);
}

void DeckGUI::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    
    auto rowH = getHeight()/7;
    
    FlexBox flexbox_main;
    flexbox_main.flexDirection = FlexBox::Direction::column;
    flexbox_main.flexWrap = FlexBox::Wrap::wrap;
    flexbox_main.alignContent = FlexBox::AlignContent::spaceAround;
    
    FlexBox flexbox_buttons;
    flexbox_buttons.flexDirection = FlexBox::Direction::row;
    flexbox_buttons.flexWrap = FlexBox::Wrap::wrap;
    flexbox_buttons.alignContent = FlexBox::AlignContent::spaceAround;
    
    FlexBox flexbox_sliders;
    flexbox_sliders.flexDirection = FlexBox::Direction::row;
    flexbox_sliders.flexWrap = FlexBox::Wrap::wrap;
    flexbox_sliders.alignContent = FlexBox::AlignContent::center;
    flexbox_sliders.justifyContent = FlexBox::JustifyContent::spaceAround;

    
    FlexBox flexbox_additional;
    flexbox_additional.flexDirection = FlexBox::Direction::row;
    flexbox_additional.flexWrap = FlexBox::Wrap::wrap;
    flexbox_additional.alignContent = FlexBox::AlignContent::stretch;
    
    // JUCE array object acts like a vector
    Array<FlexItem> play_stop_buttons;
    play_stop_buttons.add(FlexItem(getWidth()/3,rowH,playButton));
    play_stop_buttons.add(FlexItem(getWidth()/3,rowH,loadButton));
    play_stop_buttons.add(FlexItem(getWidth()/3,rowH,loopButton));
    
    flexbox_buttons.items = play_stop_buttons;
    flexbox_buttons.performLayout(bounds.removeFromTop(rowH));
    
    // JUCE array object acts like a vector
    Array<FlexItem> sliders_array;
    sliders_array.add(FlexItem(getWidth()/5,100,volSlider));
    sliders_array.add(FlexItem(getWidth()/5,100,speedSlider));
    sliders_array.add(FlexItem(getWidth()/5,100,posSlider));
    sliders_array.add(FlexItem(getWidth()/5,100,lowSlider));
//    sliders_array.add(FlexItem(getWidth()/5, 100, loopButton));
    
    track_duration.setBoundingBox(
                                   Parallelogram<float>(
                                                 Rectangle<float> (50, rowH * 4 - 10, getWidth()/2, getHeight() * 0.2)));
    


    flexbox_sliders.items = sliders_array;
    flexbox_sliders.performLayout(bounds.removeFromTop(rowH * 4));
    
    // additional components
    Array<FlexItem> additional_components;
    additional_components.add(FlexItem(getWidth(),100,waveformDisplay));

    

    
    flexbox_additional.items = additional_components;
    flexbox_additional.performLayout(bounds.removeFromTop(rowH * 2));

}

void DeckGUI::buttonClicked(Button* button)
{
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

       if (button == &loadButton)
    {
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
//            player->loadURL(URL{chooser.getResult()});
            loadToPlayer(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
//            loadText();
        });
    }
    
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
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
    if (slider == &lowSlider)
    {

        player->setLowPass(slider -> getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{

  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::filesDropped (URL url)
{
//    std::cout << "This is playbutton state: " << playButton.getState() << std::endl;
//    std::cout << "This is called" << std::endl;
//    
//    playButton.setToggleState(false, NotificationType::dontSendNotification);
    
    if(playButton.getToggleState() == true) playButton.triggerClick();
    
    player->loadURL(URL{url});
    waveformDisplay.loadURL(URL{url}); 

}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}


void DeckGUI::loadToPlayer(URL url)
{
    player->loadURL(URL{url});
}

void DeckGUI::loadText()
{
    int duration = player -> setDuration();
    int minutes = floor(duration / 60);
    int seconds = duration - (minutes * 60);
    displayed_duration = "DURATION: " + std::to_string(minutes) + ":" + std::to_string(seconds);
    track_duration.setText(displayed_duration);
}

void DeckGUI::setLoop()
{
    looping = !looping;
    player -> looping = looping;
    std::cout << "give me the button toggle state" << looping << std::endl;
}
