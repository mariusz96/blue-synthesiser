#include "ADSREnv.h"

Synth_v1_0ADSREnv::Synth_v1_0ADSREnv() {}
Synth_v1_0ADSREnv::~Synth_v1_0ADSREnv() {}

void Synth_v1_0ADSREnv::keyPress()
{
	if (attack_time > 0.0)
	{
		envelope_val = 0.0;
		envelope_stage = Stage::attack;
	}
	else if (decay_time > 0.0)
	{
		envelope_val = 1.0;
		envelope_stage = Stage::decay;
	}
	else if (sustain_val > 0.0)
	{
		envelope_val = sustain_val;
		envelope_stage = Stage::sustain;
	}
	else
	{
		envelope_val = 0.0;
		envelope_stage = Stage::off;
	}
	updateInternalRates();
}

void Synth_v1_0ADSREnv::keyRelease()
{
	if (release_time > 0.0)
	{
		envelope_stage = Stage::release;
	}
	else
	{
		envelope_val = 0.0;
		envelope_stage = Stage::off;
	}
	updateInternalRates();
}

void Synth_v1_0ADSREnv::cutOut()
{
	envelope_val = 0.0;
	envelope_stage = Stage::off;
	attack_time = 0.0;
	decay_time = 0.0;
	sustain_val = 0.0;
	release_time = 0.0;
	updateInternalRates();
}

float Synth_v1_0ADSREnv::getNextSample()
{
	if (sampleRate > 0.0 && attack_time >= 0.0 && decay_time >= 0.0 && sustain_val >= 0.0 && release_time >= 0.0)
	{
		switch (envelope_stage)
		{
		case Stage::attack:
			envelope_val = envelope_val + attackRate_perSample;
			if (envelope_val >= 1.0)
			{
				envelope_val = 1.0;

				if (decay_time > 0.0)
					envelope_stage = Stage::decay;
				else if (sustain_val > 0.0)
					envelope_stage = Stage::sustain;
				else if (release_time > 0.0)
					envelope_stage = Stage::release;
				else
					envelope_stage = Stage::off;
				updateInternalRates();
			}
			break;

		case Stage::decay:
			envelope_val = envelope_val - decayRate_perSample;
			if (envelope_val <= sustain_val)
			{
				if (sustain_val > 0.0)
				{
					envelope_val = sustain_val;
					envelope_stage = Stage::sustain;
				}
				else if (envelope_val <= 0.0)
				{
					envelope_val = 0.0;
					envelope_stage = Stage::off;
				}
				updateInternalRates();
			}
			break;

		case Stage::sustain:
			envelope_val = sustain_val;
			break;

		case Stage::release:
			envelope_val = envelope_val - releaseRate_perSample;
			if (envelope_val <= 0.0) {
				envelope_val = 0.0;
				envelope_stage = Stage::off;
				updateInternalRates();
			}
			break;

		case Stage::off:
			envelope_val = 0.0;
			break;
		}
	}
	return envelope_val;
}

Stage Synth_v1_0ADSREnv::getEnvelopeStage()
{
	return envelope_stage;
}

void Synth_v1_0ADSREnv::setParameters(float attack_time, float decay_time, float sustain_val, float release_time)
{
	this->attack_time = attack_time;
	this->decay_time = decay_time;
	this->sustain_val = sustain_val;
	this->release_time = release_time;
	updateInternalRates();
}

void Synth_v1_0ADSREnv::setSampleRate(double sampleRate)
{
	this->sampleRate = sampleRate;
	updateInternalRates();
}

void Synth_v1_0ADSREnv::updateInternalRates()
{
	if (sampleRate > 0.0)
	{
		if (attack_time > 0.0)
			attackRate_perSample = 1.0 / (attack_time * sampleRate);
		else
			attackRate_perSample = 0.0;

		if (decay_time > 0.0)
			decayRate_perSample = (1.0 - sustain_val) / (decay_time * sampleRate);
		else
			decayRate_perSample = 0.0;

		if (release_time > 0.0)
			releaseRate_perSample = envelope_val / (release_time * sampleRate);
		else
			releaseRate_perSample = 0.0;
	}
}