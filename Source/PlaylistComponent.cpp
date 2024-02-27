/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 15 Aug 2023 4:59:38pm

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <vector>


//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    tableComponent.setModel(this); // current data model to show
    
    // add four columns
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Load to Player 1", 2, 100);
    tableComponent.getHeader().addColumn("Load to Player 2", 3, 100);
    tableComponent.getHeader().addColumn("Delete", 4, 100);
    
    // show the table and the songs selected
    addAndMakeVisible(tableComponent);
    // track1_drawable contains title for song selected for player1
    addAndMakeVisible(track1_drawable);
    // track2_drawable contains title for song selected for player2
    addAndMakeVisible(track2_drawable);
    
    // set text to be white
    track1_drawable.setColour(juce::Colours::whitesmoke);
    track2_drawable.setColour(juce::Colours::whitesmoke);
    
    // set colour for the table's heading
    tableComponent.getHeader().setColour(0x1003810, juce::Colour(42, 157, 143)); // id of background
    
    // initial text before track is selected
    track1_name = "track 1 not loaded";
    track2_name = "track 2 not loaded";
    
    load_playlist(); // when application starts, allow user to pick tracks

}

// deconstructor for playlist component
PlaylistComponent::~PlaylistComponent()
{
    tableComponent.removeAllChildren();
    // to prevent a leak, delete the tracks
    delete track_1;
    delete track_2;
    delete spare_track;
}

// the look of the plalist
void PlaylistComponent::paint (juce::Graphics& g)
{
    // draw a black box around the playlist component
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

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
        g.fillAll(juce::Colour(23, 67, 99));
    }else{
        g.fillAll(juce::Colour(49, 62, 68));
    }
}

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.setColour (juce::Colours::white);
    g.drawText(trackTitles[rowNumber],2,0,width-4, height, Justification::centredLeft,true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if(columnId == 2){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new juce::LoadButton{};;
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("player1_btn");
            
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if(columnId == 3){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new juce::LoadButton{};
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("player2_btn");
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if(columnId == 4){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new TextButton{"DELETE"};
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("delete_btn");
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

        load_track("player1_btn", urlResults[id]);
        track1_name = trackTitles[id];
        track1_drawable.setText("Selected for P1: " + trackTitles[id]);
//        displaySelectedTrack(trackTitles[id]);
    };
    if(btn_name == "player2_btn")
    {
        load_track("player2_btn", urlResults[id]);
        track1_name = trackTitles[id];
        track2_drawable.setText("Selected for P2: " + trackTitles[id]);
    };
    if(btn_name == "delete_btn")
    {
        std::cout << "  if(btn_name == delete) is clicked" << std::endl;
        
        trackTitles.erase(trackTitles.begin() + id);
            
        for(int i = 0; i < trackTitles.size(); ++i)
        {
            std::cout << trackTitles[i] << std::endl;
        }
        
        
        tableComponent.updateContent();
        tableComponent.repaint();
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
    spare_track = new Track("spare_track", urlResults[0]);
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
        if(track_1 != nullptr) return track_1;
        else return spare_track;
    }
    else if(btn_name == "player2_btn")
    {
        if(track_2 != nullptr) return track_2;
        else return spare_track;
    }
}

