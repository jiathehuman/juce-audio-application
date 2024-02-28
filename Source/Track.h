//==============================================================================

/**
@file Track.h
@brief Track object contains a type_name that indicates if the track is for player1, player2 or a spare track.
 Track object also contains a from which the deckgui can load the sound file to the player.
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================


class Track
{
public:
    
    /**
    Constructor for track object for storing the button type as string and the path to the track as URL
    @param type_name the type of the track (player1, player2 or spare)
    @param track_url the path to the track
    */
    Track(String type_name, URL track_url);
    
    /** Deconstructor for track object */
    ~Track();
    
    String type_name; // public string for if the track is player1, player2 or spare
    URL track_url; // path to the sound file

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track) // detects leak from track
};
