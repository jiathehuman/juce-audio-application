/**===============================================================================
@file DJAudioPlayer.h
@brief The code is built on the template provided by Professsor Matthew from University of London.
 The DJ Audio player prepares resources to play the audio track and controls the volume, speed and position.
 The DJ Audio player also applies the IIR filters using IIRAudioSources.
 ===================================================================================*/

#pragma once
#include <JuceHeader.h>


class DJAudioPlayer : public AudioSource {
  public:

    /**
    Constructor for the  DJAudioPlayer component
    @param _formatManager the AudioFormatManager to use
    */
    DJAudioPlayer(AudioFormatManager& _formatManager);
    
    /** Destructor for the DJAudioPlayer component*/
    ~DJAudioPlayer();

    /** Prepare the transport source, and the two IIRAudioSource (low_pass, high_pass) */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** Calls periodically to fetch the next blocks of audio data **/
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    /** Source releases anything it does not need after playback stopped*/
    void releaseResources() override;
    
    /** Starts and stops the application */
    void start();

    /** Creates a new reader source and set the transportsource with the new source.
     Get the duration of the track.
     @param audioURL the URL path to the audio track to play
     */
    void loadURL(URL audioURL);
    
    /** Sets the volume of the track
     @param gain a double ideally rom 0 to 1
     */
    void setGain(double gain);
    
    /** Sets the speed of the track
     @param ratio a double ideally from 0 to 100
     */
    void setSpeed(double ratio);
    
    /** Sets the position of the track using the transport source
     @param ratio a double of the position in seconds calculated with setPositionRelative()
     */
    void setPosition(double posInSecs);
    
    /** Calcuulates the relative position of the track
     @param pos a double from 0 to 1
     */
    void setPositionRelative(double pos);
    
    /** Sets the duration of the track*/
    int setDuration();

    /** get the relative position of the playhead */
    double getPositionRelative();
    // New code ----------------------------------------------------------------------------------------
    /** Sets the coeeficient of the low pass IIRFilterAudioSource
     @param hertz cut-off frequency
     */
    void setLowPass(double hertz);
    
    /** Sets the coeeficient of the high pass IIRFilterAudioSource
     @param hertz cut-off frequency
     */
    void setHighPass(double hertz);
    
    /** Duration of a track */
    int duration;
    /** True if the audio track is to loop, flase otherwise*/
    bool looping;
    // End of new code ---------------------------------------------------------------------------------


private:
    AudioFormatManager& formatManager; // keeps a list of audio format and decides which to use
    
    // audio source that will read from format manager
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    
    // takes a positionable audio source and allows it to be played, stopped or started
    AudioTransportSource transportSource;
    
    // New code -----------------------------------------------------------------------------------------
    // audio is passed serially transport -> low -> high -> resample
    IIRFilterAudioSource low_source{&transportSource, false}; // attenuates low frequencies
    IIRFilterAudioSource high_source{&low_source, false}; // attenuates high frequencies
    // audio source that takes in an input and changes the sample rate
    ResamplingAudioSource resampleSource{&high_source, false, 2};
    // End of new code ----------------------------------------------------------------------------------
    

    
    bool isPlaying; // true if the file is currently being played

};




