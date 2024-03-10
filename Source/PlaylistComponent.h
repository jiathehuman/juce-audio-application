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

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;

    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    Track* track_1;
    Track* track_2;
    Track* spare_track;
    
    
    void buttonClicked(Button * button) override;
    Track* loadToPlayer(String btn_name);
    
private:

    void loadPlaylist();
    void setTracks(Array<File> audiofiles);
    void loadTrack(String btn_name, URL track_url);
    
    TableListBox tableComponent;
    std::vector<String> trackTitles;
    std::unique_ptr<FileChooser> chooser_ptr;
    Array<File> playlistFiles;
    Array<URL> urlResults;
    
    DrawableText track1_drawable;
    DrawableText track2_drawable;
    String track1_name;
    String track2_name;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
