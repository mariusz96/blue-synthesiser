#include "PluginProcessor.h"
#include "PluginEditor.h"

Synth_v1_0AudioProcessor::Synth_v1_0AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	audioFormatManager.registerBasicFormats();

	synthesiser.setPoly(4);
	synthesiser.setSound(new Synth_v1_0SynthesiserSample(*getFile()));
}

AudioFormatReader* Synth_v1_0AudioProcessor::getFile()
{
	AudioFormatReader* reader = nullptr;
	FileChooser fileChooser("wybierz plik audio", audioFile, "");

	if (fileChooser.browseForFileToOpen())
	{
		File file = fileChooser.getResult();
		reader = audioFormatManager.createReaderFor(file);
	}

	return reader;
}

void Synth_v1_0AudioProcessor::prepareToPlay(double sampleRate,	int samplesPerBlock)
{
	synthesiser.cutOutVoices();
	synthesiser.setSampleRate(sampleRate);
}

void Synth_v1_0AudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& incomingMidi)
{
	buffer.clear();
	keyboardState.processNextMidiBuffer(incomingMidi, 0, buffer.getNumSamples(), true);

	synthesiser.getNextBlock(buffer, incomingMidi, 0, buffer.getNumSamples());
}

MidiKeyboardState& Synth_v1_0AudioProcessor::getMidiKeyboardState()
{
	return keyboardState;
}

Synth_v1_0Synthesiser& Synth_v1_0AudioProcessor::getSynthesiser()
{
	return synthesiser;
}

Synth_v1_0AudioProcessor::~Synth_v1_0AudioProcessor() {}

//==============================================================================

const String Synth_v1_0AudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool Synth_v1_0AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool Synth_v1_0AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool Synth_v1_0AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double Synth_v1_0AudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int Synth_v1_0AudioProcessor::getNumPrograms()
{
	return 1;
}

int Synth_v1_0AudioProcessor::getCurrentProgram()
{
	return 0;
}

void Synth_v1_0AudioProcessor::setCurrentProgram(int index)
{
}

const String Synth_v1_0AudioProcessor::getProgramName(int index)
{
	return {};
}

void Synth_v1_0AudioProcessor::changeProgramName(int index, const String& newName)
{
}

void Synth_v1_0AudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Synth_v1_0AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

bool Synth_v1_0AudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* Synth_v1_0AudioProcessor::createEditor()
{
	return new Synth_v1_0AudioProcessorEditor(*this);
}

void Synth_v1_0AudioProcessor::getStateInformation(MemoryBlock& destData)
{
}

void Synth_v1_0AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Synth_v1_0AudioProcessor();
}
