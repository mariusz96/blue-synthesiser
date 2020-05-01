#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSREnv.h"

class Synth_v1_0SynthesiserSample
{
public:
	Synth_v1_0SynthesiserSample(AudioFormatReader& file);
	~Synth_v1_0SynthesiserSample();

	AudioBuffer<float>* getData();
	double getSampleRate();
	int getLength();

private:
	AudioBuffer<float>* data = nullptr;
	double sampleRate = 44100.0;
	int length = 0;

    JUCE_LEAK_DETECTOR (Synth_v1_0SynthesiserSample)
};

/*
	| █ || █ || █ |
	| █ || █ || █ |
	|   ||   ||   |
*/
class Synth_v1_0SynthesiserVoice
{
public:
	Synth_v1_0SynthesiserVoice();
	~Synth_v1_0SynthesiserVoice();

	void keyPress(int midiNote, int midiChannel, float midiVelocity, Synth_v1_0SynthesiserSample* sample, double octave, double finePitch, double attack_time, double decay_time, double sustain_val, double release_time);
	void keyRelease();
	void cutOut();

	void getNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples);
	bool isEnvelopeOff();
	
	void setSampleRate(double sampleRate);
	int getMidiNote();
	int getMidiChannel();

private:
	double calculatePitchSpeed(int midiNote, double octave, double finePitch);

	Synth_v1_0ADSREnv adsr;
	Synth_v1_0SynthesiserSample* sample = nullptr;

    int midiNote = 0, midiChannel = 0;
	float midiVelocity = 0.0;

	double curSamplePosition = 0.0, pitchSpeed = 0.0;
	double sampleRate = 44100.0;

    JUCE_LEAK_DETECTOR (Synth_v1_0SynthesiserVoice)
};

/*	  ________________________
	 /    oo  oo  oo  oo     /\
	/ // /‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾/ /
	\‾‾‾‾\‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\/
	 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
*/
class Synth_v1_0Synthesiser
{
public:
	Synth_v1_0Synthesiser();
    ~Synth_v1_0Synthesiser();

    void keyPress (int midiChannel, int midiNote, float midiVelocity);
    void keyRelease(int midiChannel, int midiNote);
	void cutOutVoices();

	void getNextBlock(AudioBuffer<float>& outputBuffer, MidiBuffer& incomingMidi, int startSample, int numSamples);
	Synth_v1_0SynthesiserVoice* findFreeOrStealLowestVoice();
	
	void setSound(Synth_v1_0SynthesiserSample* sample);
	void setSampleRate(double sampleRate);

	void setPoly(int poly);
	void setOctave(int octave);
	void setFinePitch(int finePitch);

	int getPoly();
	int getOctave();
	int getFinePitch();

	//==============================================================================
	
	void setAttack(double attack_time);
	void setDecay(double decay_time);
	void setSustain(double sustain_val);
	void setRelease(double release_time);

	double getAttack();
	double getDecay();
	double getSustain();
	double getRelease();

private:
	Array<Synth_v1_0SynthesiserVoice*> voices;
	Synth_v1_0SynthesiserSample* sample;

	int poly = 4, octave = 0, finePitch = 0;
	double attack_time = 0.0, decay_time = 0.0, sustain_val = 0.5, release_time = 0.1;

	int curMidiPosition, lastMidiPosition;
	double sampleRate = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth_v1_0Synthesiser)
};
