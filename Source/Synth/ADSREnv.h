#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
/*
		 /\D
		/  \
	   /    \__ __
	  /      S    \R
	A/		       \
*/

enum class Stage {
	attack,
	decay,
	sustain,
	release,
	off
};

class Synth_v1_0ADSREnv
{
public:
	Synth_v1_0ADSREnv();
	~Synth_v1_0ADSREnv();

	void keyPress();
	void keyRelease();
	void cutOut();

	double getNextSample();
	Stage getEnvelopeStage();

	void setParameters(double attack_time, double decay_time, double sustain_val, double release_time);
	void setSampleRate(double sampleRate);

private:
	void updateInternalRates();

	Stage envelope_stage = Stage::off;
	double envelope_val = 0.0;
	double attack_time, decay_time, sustain_val, release_time;
	double attackRate_perSample = 0.0, decayRate_perSample = 0.0, releaseRate_perSample = 0.0;
	double sampleRate = 44100.0;
};