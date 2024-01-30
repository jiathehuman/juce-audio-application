/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 15 Aug 2023 4:59:38pm

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Track title", 1, 200);
    tableComponent.getHeader().addColumn("Load to Player 1", 2, 200);
    tableComponent.getHeader().addColumn("Load to Player 2", 3, 200);

    
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(track1_drawable);
    addAndMakeVisible(track2_drawable);
    track1_drawable.setColour(juce::Colours::whitesmoke);
    track2_drawable.setColour(juce::Colours::whitesmoke);
    track1_name = "track 1 not loaded";
    track2_name = "track 2 not loaded";
    load_playlist();

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    tableComponent.setBounds(0, 0, getWidth(), getHeight() * 0.8);
    // Figuring out how to use bounding box: https://forum.juce.com/t/resize-drawabletext/13912/4
    track1_drawable.setBoundingBox(
                                   Parallelogram<float>(
                                                 Rectangle<float> (0, getHeight()* 0.8, getWidth()/2, getHeight() * 0.2)));
    track2_drawable.setBoundingBox(
                                   Parallelogram<float>(
                                                 Rectangle<float> (getWidth()/2,getHeight()*0.8, getWidth()/2, getHeight()* 0.2)));

}

int PlaylistComponent::getNumRows(){
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }else{
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.setColour (juce::Colours::white);
    g.drawText(trackTitles[rowNumber],2,0,width-4, height, Justification::centredLeft,true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if(columnId == 2){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new TextButton{"LOAD"};
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("player1_btn");
            
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if(columnId == 3){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new TextButton{"LOAD"};
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("player2_btn");
            
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(Button * button) {
    int id = std::stoi(button->getComponentID().toStdString());
    String btn_name = button -> getName();
    if(btn_name == "player1_btn")
    {
        std::cout << " This is a player 1 button" << std::endl;
        load_track("player1_btn", urlResults[id]);
        track1_name = trackTitles[id];
        track1_drawable.setText("Selected for player 1: " + trackTitles[id]);
//        displaySelectedTrack(trackTitles[id]);
    };
    if(btn_name == "player2_btn")
    {
        std::cout << " This is a player 2 button" << std::endl;
        load_track("player2_btn", urlResults[id]);
        track1_name = trackTitles[id];
        track2_drawable.setText("Selected for player 2: " + trackTitles[id]);
    };
    
}

void PlaylistComponent::load_playlist()
{
    chooser_ptr = std::make_unique<FileChooser> ("Please select the mfile you want to load...",
                                               File::getSpecialLocation (File::userDesktopDirectory));
 
    auto folderChooserFlags = FileBrowserComponent::canSelectMultipleItems | FileBrowserComponent::canSelectFiles;

    chooser_ptr->launchAsync (folderChooserFlags, [this] (const FileChooser& chooser)
    {
        Array<File> files = chooser.getResults();
        urlResults = chooser.getURLResults();
        set_tracks(files);
//        std::cout << sizeof(playlistFiles)<< std::endl;
    });

}

void PlaylistComponent::set_tracks(Array<File> audiofiles)
{
    for (int i = 0; i < audiofiles.size(); i++)
    {
        trackTitles.push_back(audiofiles[i].getFileNameWithoutExtension());
//        std::cout << urlResults[i].toString(true) << std::endl;
    }
    tableComponent.updateContent();
}

void PlaylistComponent::load_track(String btn_name, URL track_url)
{
    std::cout << btn_name << " : " << track_url.toString(true) << std::endl;
    if(btn_name == "player1_btn")
    {
        track_1 = new Track(btn_name, track_url);
    }
    if(btn_name == "player2_btn")
    {
        track_2 = new Track(btn_name, track_url);
    }
};

Track* PlaylistComponent::loadToPlayer(String btn_name)
{
    if(btn_name == "player1_btn")
    {
        return track_1;
    }
    if(btn_name == "player2_btn")
    {
        return track_2;
    }
}

void PlaylistComponent::displaySelectedTrack(String track_name)
{
//    return track_name;
}
