/*
  ==============================================================================

    FFTClass.h
    Created: 25 Feb 2024 6:09:28pm
    Author:  Jia En

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// https://docs.juce.com/master/tutorial_simple_fft.html#tutorial_simple_fft_fast_fourier_transform
//==============================================================================
/*
*/
class FFTClass  : public juce::Component,
                private Timer
{
public:
    FFTClass();
    ~FFTClass() override;

    void paint (juce::Graphics&) override;
    void resized(juce::Graphics& g);
//    static constexpr auto fftOrder = 10;
//    static constexpr auto fftSize = 1 << fftOrder;
//    
//    void pushSampleToFifo(float sample) noexcept;
//    
//    void timerCallback();
    
    void loadURL(URL audioURL);
    void pushNextSampleIntoFifo (float sample) noexcept;
    
    enum
    {
        fftOrder = 10,
        fftSize  = 1 << fftOrder
    };

private:
    bool fileLoaded; 
//    juce::Image fftImage;
//    juce::dsp::FFT forwardFFT; // declare the fft object
//    std::array<float, fftSize> fifo;
//    std::array<float, fftSize * 2> fftData;
//    int fifoIndex = 0;
//    bool nextFFTBlockReady = false;
//    
//    void drawNextLineofFFT();
    
    void drawNextLineOfSpectrogram();
    void timerCallback() override;


    Image spectrogramImage;
    dsp::FFT forwardFFT;

    float fifo [fftSize];
    float fftData [2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTClass)
};
