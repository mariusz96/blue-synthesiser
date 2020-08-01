#include "PluginProcessor.h"
#include "PluginEditor.h"

Synth_v1_0AudioProcessorEditor::Synth_v1_0AudioProcessorEditor(Synth_v1_0AudioProcessor& p) : AudioProcessorEditor(&p), processor(p), synthesiser(p.getSynthesiser())
{
	int defPoly = synthesiser.getPoly();
	int polyRange = synthesiser.getPolyRange();
	int octaveRange = synthesiser.getOctaveRange();
	int finePitchRange = synthesiser.getFinePitchRange();

	setSliderWithLabel(polySlider, polyLabel, defPoly, 1, polyRange, "Poly");
	setSliderWithLabel(octaveSlider, octaveLabel, 0, -octaveRange, octaveRange, "Octave");
	setSliderWithLabel(finePitchSlider, finePitchLabel, 0, -finePitchRange, finePitchRange, "Fine");

	setKnobWithLabel(attackKnob, attackLabel, 0.0, 0.0, 20.0, "A");
	setKnobWithLabel(decayKnob, decayLabel, 0.0, 0.0, 20.0, "D");
	setKnobWithLabel(sustainKnob, sustainLabel, 0.5, 0.0, 1.0, "S");
	setKnobWithLabel(releaseKnob, releaseLabel, 0.1, 0.0, 20.0, "R");

	keyboardComponent.clearKeyMappings();
	addAndMakeVisible(keyboardComponent);
	setOpaque(true);
	setSize(370, 320);
}

Synth_v1_0AudioProcessorEditor::~Synth_v1_0AudioProcessorEditor() {}

void Synth_v1_0AudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void Synth_v1_0AudioProcessorEditor::resized()
{
	keyboardComponent.setBounds(0, getHeight() - 80, getWidth(), 80);

	int knobSide = getWidth() / 4;
	int knobY = keyboardComponent.getY() - knobSide - 10;
	attackKnob.setBounds(0 * knobSide, knobY, knobSide, knobSide);
	decayKnob.setBounds(1 * knobSide, knobY, knobSide, knobSide);
	sustainKnob.setBounds(2 * knobSide, knobY, knobSide, knobSide);
	releaseKnob.setBounds(3 * knobSide, knobY, knobSide, knobSide);

	int sliderHeight = getWidth() / 12;
	finePitchSlider.setBounds(50, attackKnob.getY() - 30 - sliderHeight, getWidth() - 50, sliderHeight);
	octaveSlider.setBounds(50, finePitchSlider.getY() - sliderHeight, getWidth() - 50, sliderHeight);
	polySlider.setBounds(50, octaveSlider.getY() - sliderHeight, getWidth() - 50, sliderHeight);

	getKnobAndSliderValues();
}

void Synth_v1_0AudioProcessorEditor::getKnobAndSliderValues()
{
	attackKnob.setValue(synthesiser.getAttack());
	decayKnob.setValue(synthesiser.getDecay());
	sustainKnob.setValue(synthesiser.getSustain());
	releaseKnob.setValue(synthesiser.getRelease());

	finePitchSlider.setValue(synthesiser.getFinePitch());
	octaveSlider.setValue(synthesiser.getOctave());
	polySlider.setValue(synthesiser.getPoly());
}

void Synth_v1_0AudioProcessorEditor::setKnobWithLabel(Slider& knob, Label& label, double defaultfVal, double startVal, double endVal, String textOnLabel)
{
	knob.setSliderStyle(Slider::Rotary);
	knob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
	knob.setRange(startVal, endVal, 0.001);
	knob.setDoubleClickReturnValue(true, defaultfVal);
	knob.addListener(this);
	addAndMakeVisible(knob);

	label.setJustificationType(Justification::centred);
	label.setText(textOnLabel, dontSendNotification);
	label.attachToComponent(&knob, false);
	addAndMakeVisible(label);
}

void Synth_v1_0AudioProcessorEditor::setSliderWithLabel(Slider& slider, Label& label, double defaultfVal, double startVal, double endVal, String textOnLabel)
{
	slider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 20);
	slider.setRange(startVal, endVal, 1);
	slider.setDoubleClickReturnValue(true, defaultfVal);
	slider.addListener(this);
	addAndMakeVisible(slider);

	label.setText(textOnLabel, dontSendNotification);
	label.attachToComponent(&slider, true);
	label.setJustificationType(Justification::centred);
	addAndMakeVisible(label);
}

void Synth_v1_0AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &attackKnob)				synthesiser.setAttack(attackKnob.getValue());
	else if (slider == &decayKnob)			synthesiser.setDecay(decayKnob.getValue());
	else if (slider == &sustainKnob)		synthesiser.setSustain(sustainKnob.getValue());
	else if (slider == &releaseKnob)		synthesiser.setRelease(releaseKnob.getValue());
	else if (slider == &polySlider)			synthesiser.setPoly((int)polySlider.getValue());
	else if (slider == &octaveSlider)		synthesiser.setOctave((int)octaveSlider.getValue());
	else if (slider == &finePitchSlider)	synthesiser.setFinePitch((int)finePitchSlider.getValue());
}