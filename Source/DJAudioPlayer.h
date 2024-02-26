/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew (Code provided on Coursera)

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "filterClass.h"
#include "FFTClass.h"

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    int setDuration();
    

    void start();
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();
    void setLowPass(double hertz);
    int duration;
    
    bool looping;


//    void setHighPass(double hertz);

    

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    
    IIRFilterAudioSource low_source{&transportSource, false};
//    IIRFilterAudioSource high_source{&low_source, false};
    ResamplingAudioSource resampleSource{&low_source, false, 2};
    
    bool isPlaying;


    
    
    


};




