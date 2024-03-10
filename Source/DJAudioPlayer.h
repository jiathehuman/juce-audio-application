/**===============================================================================
@file DJAudioPlayer.h
@brief The code is built on the template provided by Professsor Matthew from University of London.
 ===================================================================================*/

#pragma once
//#include "../JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>



class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void start();
//    void stop();

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    int setDuration();

    



    /** get the relative position of the playhead */
    double getPositionRelative();
    void setLowPass(double hertz);
    void setHighPass(double hertz);
    int duration;
    
    bool looping;


private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    
    // passed serially transport -> low -> high -> resample
    IIRFilterAudioSource low_source{&transportSource, false};
    IIRFilterAudioSource high_source{&low_source, false};
    ResamplingAudioSource resampleSource{&high_source, false, 2};
    
    bool isPlaying;


    
    
    


};




