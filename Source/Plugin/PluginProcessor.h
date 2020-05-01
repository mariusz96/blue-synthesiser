#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Synth/Synthesiser.h"

class Synth_v1_0AudioProcessor  : public AudioProcessor
{
public:
    Synth_v1_0AudioProcessor();
    ~Synth_v1_0AudioProcessor();
    
	AudioFormatReader* getFile();
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& incomingMidi) override;

	MidiKeyboardState& getMidiKeyboardState();
	Synth_v1_0Synthesiser& getSynthesiser();

	//==========================================================================
	
	void releaseResources() override;
    #ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	File audioFile;
	AudioFormatManager audioFormatManager;

	MidiKeyboardState keyboardState;

	Synth_v1_0Synthesiser synthesiser;
	bool lastWasPlaying = false;
	bool curIsPlaying = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth_v1_0AudioProcessor)
};
