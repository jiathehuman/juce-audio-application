/*
  ==============================================================================

    PlaylistComponent.h
    Created: 15 Aug 2023 4:59:38pm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "MainComponent.h"
#include "Track.h"
#include <vector>
#include <string>

//==============================================================================
/*
*/
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
    
    void buttonClicked(Button * button) override;
    Track* loadToPlayer(String btn_name);
    
private:

    void load_playlist();
    void set_tracks(Array<File> audiofiles);
    void load_track(String btn_name, URL track_url);
    void displaySelectedTrack(String track_name);

    
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
