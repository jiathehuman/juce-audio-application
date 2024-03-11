
 /** ===============================================================================
 @file PlaylistComponent.cpp
 @brief The CPP file implements the functions promised in the header file.
  The code in this file builds on the template provided in Coursera Topic 10.
  =====================================================================================*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <vector>


PlaylistComponent::PlaylistComponent()
{
    tableComponent.setModel(this); // current data model to show
    
    // add four columns - titles, load to player 1, load to player 2, delete track
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Load to Player 1", 2, 130);
    tableComponent.getHeader().addColumn("Load to Player 2", 3, 130);
    tableComponent.getHeader().addColumn("Delete", 4, 130);
    

    addAndMakeVisible(tableComponent); // draw playlist componet
    addAndMakeVisible(track1_drawable); // title for song selected for player1
    addAndMakeVisible(track2_drawable); // title for song selected for player2
    
    // set text to be white for the titles of songs selected
    track1_drawable.setColour(juce::Colours::whitesmoke);
    track2_drawable.setColour(juce::Colours::whitesmoke);
    
    // set colour for the table's heading
    tableComponent.getHeader().setColour(0x1003810, juce::Colour(42, 157, 143)); // id of background
    
    // initial text before track is selected
    track1_name = "";
    track2_name = "";
    
    loadPlaylist(); // when application starts, allow user to pick tracks

}

// deconstructor for playlist component
PlaylistComponent::~PlaylistComponent()
{
    // New code ---------------------------------------------------------------------------------------
    // to prevent a leak, delete the three tracks
    delete track_1;
    delete track_2;
    delete spare_track;
    tableComponent.removeAllChildren(); // remove all child components
    // End of new code --------------------------------------------------------------------------------
}

// the look of the plalist
void PlaylistComponent::paint (juce::Graphics& g)
{
    // draw a black box around the playlist component
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1); // draw an outline around the component

    g.setColour (juce::Colours::white); // colour of words
    g.setFont (14.0f);

}

void PlaylistComponent::resized()
{
    /** At around the 8/10 height, draw the titles of the two tracks. **/
    tableComponent.setBounds(0, 0, getWidth(), getHeight() * 0.8);
    // New code ---------------------------------------------------------------------------------------
    /**
     Reference for bounding box:
     https://forum.juce.com/t/resize-drawabletext/13912/4
     **/

    /** Draws the two titles stored as drawableTexts */
    track1_drawable.setBoundingBox(
                                   Parallelogram<float>(
                                                 Rectangle<float> (0, getHeight()* 0.8, getWidth()/2, getHeight() * 0.2)));
    track2_drawable.setBoundingBox(
                                   Parallelogram<float>(
                                                 Rectangle<float> (getWidth()/2,getHeight()*0.8, getWidth()/2, getHeight()* 0.2)));

    // End of new code -------------------------------------------------------------------------------
}

int PlaylistComponent::getNumRows(){
    /** number of rows is determined by the number of tracks */
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    // if the row is selected, make it a dark blue.
    if (rowIsSelected) {
        g.fillAll(juce::Colour(23, 67, 99));
    }else{ // if the row is not selected, make it a grey
        g.fillAll(juce::Colour(49, 62, 68));
    }
}

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.setColour (juce::Colours::white);
    g.drawText(trackTitles[rowNumber],2,0,width-4, height, Justification::centredLeft,true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if(columnId == 2){ // if it is the second column, update the row with a load to player 1 button
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new juce::LoadButton{};;
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("player1_btn");
            
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if(columnId == 3){ // if it is the third column, update the row with a load to player 2 button
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new juce::LoadButton{};
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("player2_btn");
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if(columnId == 4){ // if it is the fourth column, update the row with a delete button
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new juce::DeleteButton{};
            String id{std::to_string(rowNumber)};
            btn ->setComponentID(id);
            btn -> setName("delete_btn");
            btn ->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate; // return the updated component with custom parts
}


void PlaylistComponent::buttonClicked(Button * button) {
    int id = std::stoi(button->getComponentID().toStdString());
    
    // New code ---------------------------------------------------------------------------------------
    String btn_name = button -> getName();
    
    /** if the button is a player1 button, set the track to be that audio track
        and change the selected track to be track 1 drawable text*/
    if(btn_name == "player1_btn")
    {
        loadTrack("player1", urlResults[id]); // change track_1's URL to this file path of this track
        track1_name = trackTitles[id];
        track1_drawable.setText("Selected for P1: " + trackTitles[id]);
    };
    
    /** if the button is a player2 button, set the track to be that audio track
        and change the selected track to be track 2 drawable text*/
    if(btn_name == "player2_btn")
    {
        loadTrack("player2", urlResults[id]); // change track_2's URL to this file path of this track
        track1_name = trackTitles[id];
        track2_drawable.setText("Selected for P2: " + trackTitles[id]);
    };
    
    /** if the delete button is clicked, erase that track from tracktitles and update the component
        and change the selected track to be track 2 drawable text*/
    if(btn_name == "delete_btn")
    {

        trackTitles.erase(trackTitles.begin() + id); // call erase on the trackTitles vector
            
        /** update and repaint the component */
        tableComponent.updateContent();
        tableComponent.repaint();
    };
    // End of new code ----------------------------------------------------------------------------
}

// New code ---------------------------------------------------------------------------------------
void PlaylistComponent::loadPlaylist()
{
    chooser_ptr = std::make_unique<FileChooser> ("Please select the mfile you want to load...",
                                               File::getSpecialLocation (File::userDesktopDirectory));
 
    // flags to pass in the chooser pointer - users can select multiple items and files
    auto folderChooserFlags = FileBrowserComponent::canSelectMultipleItems | FileBrowserComponent::canSelectFiles;

    chooser_ptr->launchAsync (folderChooserFlags, [this] (const FileChooser& chooser)
    {
        Array<File> files = chooser.getResults(); // load all the files user picked
        urlResults = chooser.getURLResults();
        setTracks(files); // creates the three Track objects
    });

}

void PlaylistComponent::setTracks(Array<File> audiofiles)
{
    for (int i = 0; i < audiofiles.size(); i++)
    {
        /** get all the titles as a vector of strings */
        trackTitles.push_back(audiofiles[i].getFileNameWithoutExtension());
    }
    /** temporarily use the first tracks as the url for all three tracks */
    spare_track = new Track("spare_track", urlResults[0]); // used when user forgets to select a track
    track_1 = new Track("player1", urlResults[0]);
    track_2 = new Track("player2", urlResults[0]);
    tableComponent.updateContent(); // updates the playlist table
}

void PlaylistComponent::loadTrack(String type_name, URL track_url)
{
    if(type_name == "player1")
    {
        /** Track track_1 object's track_url is updated with the selected url */
        track_1-> track_url = track_url;
    }
    if(type_name == "player2")
    {
        /** Track track_2 object's track_url is updated with the selected url */
        track_2 -> track_url = track_url;
    }
};

Track* PlaylistComponent::loadToPlayer(String type_name)
{
    if(type_name == "player1")
    {
        if(track_1 != nullptr) return track_1; // if track_1 is created, return track 1
        else return spare_track; // otherwise, use the spare track
    }
    else if(type_name == "player2")
    {
        if(track_2 != nullptr) return track_2; // if track_2 is created, return track 2
        else return spare_track; // otherwise, use the spare track
    }
    else return spare_track; // in case type_name is not either above
}
// End of new code ---------------------------------------------------------------------------------
