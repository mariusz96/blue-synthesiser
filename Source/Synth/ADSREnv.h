#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

enum class Stage {
	attack,
	decay,
	sustain,
	release,
	off
};

/*
		 /\D
		/  \
	   /    \__ __
	  /      S    \R
	A/		       \
*/
class Synth_v1_0ADSREnv
{
public:
	Synth_v1_0ADSREnv();
	~Synth_v1_0ADSREnv();

	void keyPress();
	void keyRelease();
	void cutOut();

	float getNextSample();
	Stage getEnvelopeStage();

	void setParameters(float attack_time, float decay_time, float sustain_val, float release_time);
	void setSampleRate(double sampleRate);

private:
	void updateInternalRates();

	Stage envelope_stage = Stage::off;
	float envelope_val = 0.0f;
	float attack_time = 0.0f, decay_time = 0.0f, sustain_val = 0.0f, release_time = 0.0f;
	float attackRate_perSample = 0.0f, decayRate_perSample = 0.0f, releaseRate_perSample = 0.0f;
	double sampleRate = 44100.0;
};