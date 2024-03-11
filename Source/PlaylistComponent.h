 /** ===============================================================================
 @file PlaylistComponent.h
 @brief The Playlist Component is a Table List Box Model component.
  Users will be prompted to select audio tracks when the application is launched.
  The playlist component acts as a button listener for the load and delete buttons.
  In this module, three tracks are created - two for each player, and a spare track.
  When the main component registers a click from the confirm button, the two tracks are loaded to the player.
  =====================================================================================*/

#pragma once
#include <JuceHeader.h>
#include "custom_style.h"
#include "Track.h"
#include <vector>
#include <string>




class PlaylistComponent  : public juce::Component, public TableListBoxModel, public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    /**
     Paint code for background
     @param Graphics& reference to the graphics object to draw directly on
     **/
    void paint (juce::Graphics&) override;
    
    /** Arrange and draw the child components **/
    void resized() override; // orgainis
    
    /** The number of rows to draw is equal tp the number of tracks */
    int getNumRows() override;

    /** Paints the row a different colour if it is selected */
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    /** Sets the text of the cell white and draws the track titles in the first column */
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /** refreshComponentForCell is used to create and add the custom load and delete buttons*/
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    /** Pointers to the three tracks created, passed to main component*/
    Track* track_1;
    Track* track_2;
    Track* spare_track;
    
    /**
    Handles the button click events for the load buttons and the delete buttons
    @param button pointer
    */
    void buttonClicked(Button * button) override;
    
    /**
    Called by the main component when the 'confirm selection' button is clicked
    Returns a reference to one of the three tracks.
    @param btn_name is a string with reference to which player the track should be loaded to
    */
    Track* loadToPlayer(String btn_name);
    
private:

    /** 
     Prompts users to choose track files at the start,
     stores the tracks as an array of files,
     stores the urlResults as an array of URLs,
     calls setTracks*/
    void loadPlaylist();
    
    /**
     Gets all the track titles as a vector of strings
    Set the url of the three tracks (sparetrack, track1, track2) with the first URL path.
    @param audiofiles is an array of the track files the user selected
    */
    void setTracks(Array<File> audiofiles);
    
    /**
     When the user clicks on the load button in either column for a particular track,
     loadTrack changes the url of either track1 or track2 to that track.
    @param audiofiles is an array of the track files the user selected
    */
    void loadTrack(String btn_name, URL track_url);
    
    TableListBox tableComponent; // the tablelistbox model
    std::vector<String> trackTitles; // a vector of string containing all the titles
    std::unique_ptr<FileChooser> chooser_ptr; // the file chooser unique pointer to laod files
    Array<File> playlistFiles; // an array of audio files chosen by user
    Array<URL> urlResults; // an array of all the url paths of the tracks
    
    DrawableText track1_drawable; // draws the selected track for track1
    DrawableText track2_drawable; // draws the selected track for track2
    String track1_name; // the name of track1
    String track2_name; // the name of track2
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
