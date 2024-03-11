/**===============================================================================
 @file DJAudioPlayer.cpp
 @brief The CPP file implements the functions promised in the header file.
 =================================================================================== */

#include "DJAudioPlayer.h"
#include <optional>

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: duration(0), looping(false), formatManager(_formatManager), isPlaying(false)
{
    // Constructor for DJAudioPlayer
}
DJAudioPlayer::~DJAudioPlayer()
{
    // Destructor for DJAudioPlayer
}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    // prepare the transport source and the two IIR filter audio sources
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // New code -----------------------------------------------------------------------------------
    low_source.prepareToPlay(samplesPerBlockExpected, sampleRate);
    high_source.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // End of new code ----------------------------------------------------------------------------
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
    /** release resources when the audio playback stops */
    transportSource.releaseResources();
    resampleSource.releaseResources();
    low_source.releaseResources();
    high_source.releaseResources();

}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, 
true)); 
        // set the source to be looping or not looping, depending on the looping boolean
        newSource -> setLooping(looping);
        
        // set the new source in transport source
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());          
        
        // New code ---------------------------------------------------------------------------------
        // get the duration of the track
        duration = int((reader->lengthInSamples) / (reader->sampleRate));
        // End of new code --------------------------------------------------------------------------

    }
}
// New code -----------------------------------------------------------------------------------------
int DJAudioPlayer::setDuration()
{
    return duration; // return the duration of the track when called by deckGUI
}
// End of new code ----------------------------------------------------------------------------------

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain); // set the volume of the track
    }
   
}
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio); // set the speed of the track
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs); // set the position of the track
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs); // set the relative position
    }
}

void DJAudioPlayer::start() // when play button is toggled
{
    if(!isPlaying) transportSource.start(); // start playing if audio track is not playing
    else transportSource.stop(); // stop play if audio track is playing
    isPlaying = !isPlaying; // flip the boolean value
}


double DJAudioPlayer::getPositionRelative() // get relative position
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// New code -----------------------------------------------------------------------------------------

void DJAudioPlayer::setLowPass(double hertz)
{
    if(isPlaying) // necessary because reader is only created by format manager in loadURL
    {
        auto* reader = readerSource -> getAudioFormatReader();
        
        // low pass filter attenuates frequencies below cutoff frequencies
        low_source.setCoefficients(IIRCoefficients::makeLowPass(reader->sampleRate, hertz));
    }
}

void DJAudioPlayer::setHighPass(double hertz)
{
    if(isPlaying) // necessary because reader is only created by format manager in loadURL
    {
        auto* reader = readerSource -> getAudioFormatReader();
        
        // high pass filter attenuates frequencies above cutoff frequencies
        high_source.setCoefficients(IIRCoefficients::makeHighPass(reader->sampleRate, hertz));
    }
}
// End of new code -----------------------------------------------------------------------------------
