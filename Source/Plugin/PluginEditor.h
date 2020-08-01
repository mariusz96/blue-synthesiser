#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Synth_v1_0AudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener
{
public:
	Synth_v1_0AudioProcessorEditor(Synth_v1_0AudioProcessor&);
	~Synth_v1_0AudioProcessorEditor();

	void paint(Graphics&) override;
	void resized() override;

private:
	void getKnobAndSliderValues();
	void setKnobWithLabel(Slider& knob, Label& label, double defaultfVal, double startVal, double endVal, String textOnLabel);
	void setSliderWithLabel(Slider& slider, Label& label, double defaultfVal, double startVal, double endVal, String textOnLabel);
	void sliderValueChanged(Slider*) override;

	Synth_v1_0AudioProcessor& processor;
	Synth_v1_0Synthesiser& synthesiser;
	MidiKeyboardState& keyboardState = processor.getMidiKeyboardState();

	//==============================================================================

	MidiKeyboardComponent keyboardComponent{ keyboardState, MidiKeyboardComponent::horizontalKeyboard };

	Slider polySlider, octaveSlider, finePitchSlider;
	Label polyLabel, octaveLabel, finePitchLabel;

	Slider attackKnob, decayKnob, sustainKnob, releaseKnob;
	Label attackLabel, decayLabel, sustainLabel, releaseLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Synth_v1_0AudioProcessorEditor)
};
