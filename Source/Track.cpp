 /**===============================================================================
 @file Track.cpp
 @brief The CPP file implements the functions promised in the header file.
 =====================================================================================*/

#include <JuceHeader.h>
#include "Track.h"
// New code ---------------------------------------------------------------------------------------
// Constructor for track to initialise type_name and track_url
Track::Track(String type_name, URL track_url): type_name(type_name), track_url(track_url)
{
    
}

// Deconstructor for track
Track::~Track()
{}
// End of new code --------------------------------------------------------------------------------

