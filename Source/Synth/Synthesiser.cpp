#include "Synthesiser.h"

Synth_v1_0SynthesiserSample::Synth_v1_0SynthesiserSample(AudioFormatReader& file)
{
	if (&file != nullptr)
	{
		sampleRate = file.sampleRate;
		length = (int)file.lengthInSamples;
		data = new AudioBuffer<float>(2, (int)file.lengthInSamples + 4);
		file.read(data, 0, (int)file.lengthInSamples + 4, 0, true, true);
	}
}

AudioBuffer<float>* Synth_v1_0SynthesiserSample::getData()
{
	return data;
}

double Synth_v1_0SynthesiserSample::getSampleRate()
{
	return sampleRate;
}

int Synth_v1_0SynthesiserSample::getLength()
{
	return length;
}

Synth_v1_0SynthesiserSample::~Synth_v1_0SynthesiserSample() {}

//==============================================================================

Synth_v1_0SynthesiserVoice::Synth_v1_0SynthesiserVoice() {}
Synth_v1_0SynthesiserVoice::~Synth_v1_0SynthesiserVoice() {}

void Synth_v1_0SynthesiserVoice::keyPress(int midiNote, int midiChannel, float midiVelocity, Synth_v1_0SynthesiserSample* sample, int octave, int finePitch, int finePitchRange, float attack_time, float decay_time, float sustain_val, float release_time)
{
	this->midiNote = midiNote;
	this->midiChannel = midiChannel;
	this->midiVelocity = midiVelocity;
	this->sample = sample;

	curSamplePosition = 0.0;
	pitchSpeed = calculatePitchSpeed(midiNote, octave, finePitch, finePitchRange);
	adsr.setSampleRate(this->sample->getSampleRate());
	adsr.setParameters(attack_time, decay_time, sustain_val, release_time);
	adsr.keyPress();
}

void Synth_v1_0SynthesiserVoice::keyRelease()
{
	adsr.keyRelease();
}

void Synth_v1_0SynthesiserVoice::cutOut()
{
	adsr.cutOut();
}

void Synth_v1_0SynthesiserVoice::getNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (sample != nullptr && sampleRate > 0.0)
	{
		int sampleLength = sample->getLength();
		const float* inL = sample->getData()->getReadPointer(0);
		const float* inR = sample->getData()->getReadPointer(1);
		float* outL = outputBuffer.getWritePointer(0, startSample);
		float* outR = outputBuffer.getWritePointer(1, startSample);

		for (int i = startSample; i < numSamples; i++)
		{
			int nearestSample_left = (int)curSamplePosition;
			int nearestSample_right = (int)(curSamplePosition + 1.0);
			float distance_left = curSamplePosition - nearestSample_left;
			float distance_right = nearestSample_right - curSamplePosition;
			float envelopeValue = adsr.getNextSample();

			outL[i] += (inL[nearestSample_left] * distance_right + inL[nearestSample_right] * distance_left) * (midiVelocity * envelopeValue);
			outR[i] += (inR[nearestSample_left] * distance_right + inR[nearestSample_right] * distance_left) * (midiVelocity * envelopeValue);

			curSamplePosition = curSamplePosition + pitchSpeed;
			if (curSamplePosition >= sampleLength)
				curSamplePosition = 0.0;
		}
	}
}

double Synth_v1_0SynthesiserVoice::calculatePitchSpeed(int midiNote, int octave, int finePitch, int finePitchRange)
{
	// c5 = 84, 1 octave = 12 semitones, 12 semitones = 1200 cents
	double noteSum = ((midiNote - 84.0) / 12.0) + (octave * 1.0) + ((finePitch / finePitchRange) * (finePitchRange / 1200.0));

	// +1 octave = pitch * 2, -1 octave = pitch / 2
	return pow(2.0, noteSum) * (this->sample->getSampleRate() / this->sampleRate);
}

bool Synth_v1_0SynthesiserVoice::isEnvelopeOff()
{
	return adsr.getEnvelopeStage() == Stage::off;
}

void Synth_v1_0SynthesiserVoice::setSampleRate(double sampleRate)
{
	this->sampleRate = sampleRate;
}

int Synth_v1_0SynthesiserVoice::getMidiNote()
{
	return midiNote;
}

int Synth_v1_0SynthesiserVoice::getMidiChannel()
{
	return midiChannel;
}

//==============================================================================

Synth_v1_0Synthesiser::Synth_v1_0Synthesiser() {}
Synth_v1_0Synthesiser::~Synth_v1_0Synthesiser() {}

void Synth_v1_0Synthesiser::keyPress(int midiChannel, int midiNote, float midiVelocity)
{
	if (!voices.isEmpty() && sampleRate > 0.0)
	{
		Synth_v1_0SynthesiserVoice* v = findFreeOrStealLowestVoice();
		v->setSampleRate(sampleRate);
		v->keyPress(midiNote, midiChannel, midiVelocity, sample, octave, finePitch, finePitchRange, attack_time, decay_time, sustain_val, release_time);
	}
}

void Synth_v1_0Synthesiser::keyRelease(int midiChannel, int midiNote)
{
	for (Synth_v1_0SynthesiserVoice* v : voices)
		if (v->getMidiNote() == midiNote && v->getMidiChannel() == midiChannel)
			v->keyRelease();
}

void Synth_v1_0Synthesiser::cutOutVoices()
{
	for (Synth_v1_0SynthesiserVoice* v : voices)
		v->cutOut();
}

void Synth_v1_0Synthesiser::getNextBlock(AudioBuffer<float>& outputBuffer, MidiBuffer& incomingMidi, int startSample, int numSamples)
{
	if (&outputBuffer != nullptr && sampleRate > 0.0)
	{
		MidiBuffer::Iterator midiIterator(incomingMidi);
		MidiMessage m;
		curMidiPosition = startSample;
		midiIterator.setNextSamplePosition(curMidiPosition);

		while (curMidiPosition < numSamples)
		{
			if (midiIterator.getNextEvent(m, curMidiPosition))
			{
				if (m.isNoteOn())
				{
					keyPress(m.getChannel(), m.getNoteNumber(), m.getFloatVelocity());
				}
				else if (m.isNoteOff())
				{
					keyRelease(m.getChannel(), m.getNoteNumber());
				}

				for (Synth_v1_0SynthesiserVoice* v : voices)
					v->getNextBlock(outputBuffer, lastMidiPosition, curMidiPosition);

				lastMidiPosition = curMidiPosition;
			}
			else
			{
				for (Synth_v1_0SynthesiserVoice* v : voices)
					v->getNextBlock(outputBuffer, curMidiPosition, (numSamples - curMidiPosition));

				return;
			}
		}
	}
}

Synth_v1_0SynthesiserVoice* Synth_v1_0Synthesiser::findFreeOrStealLowestVoice()
{
	if (voices.isEmpty())
	{
		return nullptr;
	}
	else
	{
		Synth_v1_0SynthesiserVoice* lowestVoice = voices.getFirst();

		for (Synth_v1_0SynthesiserVoice* v : voices)
		{
			if (v->isEnvelopeOff())
			{
				return v;
			}
			else if (v->getMidiNote() < lowestVoice->getMidiNote())
			{
				lowestVoice = v;
			}

		}
		return lowestVoice;
	}
}

void Synth_v1_0Synthesiser::setSound(Synth_v1_0SynthesiserSample* sample)
{
	this->sample = sample;
}

void Synth_v1_0Synthesiser::setSampleRate(double sampleRate)
{
	this->sampleRate = sampleRate;
	for (Synth_v1_0SynthesiserVoice* v : voices)
		v->setSampleRate(sampleRate);
}

void Synth_v1_0Synthesiser::setPoly(int poly)
{
	this->poly = poly;
	voices.clear();
	for (int i = 0; i < poly; i++)
	{
		Synth_v1_0SynthesiserVoice* new_v = new Synth_v1_0SynthesiserVoice();
		new_v->setSampleRate(this->sampleRate);
		voices.add(new_v);
	}
}

void Synth_v1_0Synthesiser::setPolyRange(int polyRange)
{
	cutOutVoices();
	this->polyRange = polyRange;
}

void Synth_v1_0Synthesiser::setOctave(int octave)
{
	cutOutVoices();
	this->octave = octave;
}

void Synth_v1_0Synthesiser::setOctaveRange(int octaveRange)
{
	cutOutVoices();
	this->octaveRange = octaveRange;
}

void Synth_v1_0Synthesiser::setFinePitch(int finePitch)
{
	cutOutVoices();
	this->finePitch = finePitch;
}

void Synth_v1_0Synthesiser::setFinePitchRange(int finePitchRange)
{
	cutOutVoices();
	this->finePitchRange = finePitchRange;
}

int Synth_v1_0Synthesiser::getPoly()
{
	return poly;
}

int Synth_v1_0Synthesiser::getPolyRange()
{
	return polyRange;
}

int Synth_v1_0Synthesiser::getOctave()
{
	return octave;
}

int Synth_v1_0Synthesiser::getOctaveRange()
{
	return octaveRange;
}

int Synth_v1_0Synthesiser::getFinePitch()
{
	return finePitch;
}

int Synth_v1_0Synthesiser::getFinePitchRange()
{
	return finePitchRange;
}

//==============================================================================

void Synth_v1_0Synthesiser::setAttack(float attack_time)
{
	this->attack_time = attack_time;
}

void Synth_v1_0Synthesiser::setDecay(float decay_time)
{
	this->decay_time = decay_time;
}

void Synth_v1_0Synthesiser::setSustain(float sustain_val)
{
	this->sustain_val = sustain_val;
}

void Synth_v1_0Synthesiser::setRelease(float release_time)
{
	this->release_time = release_time;
}

float Synth_v1_0Synthesiser::getAttack()
{
	return attack_time;
}

float Synth_v1_0Synthesiser::getDecay()
{
	return decay_time;
}

float Synth_v1_0Synthesiser::getSustain()
{
	return sustain_val;
}

float Synth_v1_0Synthesiser::getRelease()
{
	return release_time;
}