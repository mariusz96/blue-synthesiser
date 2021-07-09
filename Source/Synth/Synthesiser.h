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

	JUCE_LEAK_DETECTOR(Synth_v1_0SynthesiserSample)
};

class Synth_v1_0SynthesiserVoice
{
public:
	Synth_v1_0SynthesiserVoice();
	~Synth_v1_0SynthesiserVoice();

	void keyPress(int midiNote, int midiChannel, float midiVelocity, Synth_v1_0SynthesiserSample* sample, int octave, int finePitch, int finePitchRange, float attack_time, float decay_time, float sustain_val, float release_time);
	void keyRelease();
	void cutOut();

	void getNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples);
	bool isEnvelopeOff();

	void setSampleRate(double sampleRate);
	int getMidiNote();
	int getMidiChannel();

private:
	double calculatePitchSpeed(int midiNote, int octave, int finePitch, int finePitchRange);

	Synth_v1_0ADSREnv adsr;
	Synth_v1_0SynthesiserSample* sample = nullptr;

	int midiNote = 0, midiChannel = 0;
	float midiVelocity = 0.0;

	double curSamplePosition = 0.0, pitchSpeed = 0.0;
	double sampleRate = 44100.0;

	JUCE_LEAK_DETECTOR(Synth_v1_0SynthesiserVoice)
};

class Synth_v1_0Synthesiser
{
public:
	Synth_v1_0Synthesiser();
	~Synth_v1_0Synthesiser();

	void keyPress(int midiChannel, int midiNote, float midiVelocity);
	void keyRelease(int midiChannel, int midiNote);
	void cutOutVoices();

	void getNextBlock(AudioBuffer<float>& outputBuffer, MidiBuffer& incomingMidi, int startSample, int numSamples);
	Synth_v1_0SynthesiserVoice* findFreeOrStealLowestVoice();

	void setSound(Synth_v1_0SynthesiserSample* sample);
	void setSampleRate(double sampleRate);

	void setPoly(int poly);
	void setPolyRange(int polyRange);
	void setOctave(int octave);
	void setOctaveRange(int octaveRange);
	void setFinePitch(int finePitch);
	void setFinePitchRange(int finePitchRange);

	int getPoly();
	int getPolyRange();
	int getOctave();
	int getOctaveRange();
	int getFinePitch();
	int getFinePitchRange();

	//==============================================================================

	void setAttack(float attack_time);
	void setDecay(float decay_time);
	void setSustain(float sustain_val);
	void setRelease(float release_time);

	float getAttack();
	float getDecay();
	float getSustain();
	float getRelease();

private:
	OwnedArray<Synth_v1_0SynthesiserVoice> voices;
	Synth_v1_0SynthesiserSample* sample = nullptr;

	int poly = 4, polyRange = 64, octave = 0, octaveRange = 7, finePitch = 0, finePitchRange = 600;
	float attack_time = 0.0f, decay_time = 0.0f, sustain_val = 0.5f, release_time = 0.1f;

	int curMidiPosition = 0, lastMidiPosition = 0;
	double sampleRate = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Synth_v1_0Synthesiser)
};
