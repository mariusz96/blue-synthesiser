# Blue Synthesiser
My bachelor's thesis "Sound synthesis based on audio samples as an VST3 instrument" in C++ and [JUCE](https://juce.com):<br/>![image](https://user-images.githubusercontent.com/62397363/79278180-8428df80-7eab-11ea-978e-976abcbf1bd9.png)

## Prerequisites:
- JUCE 5.4.7
- A supported IDE (XCode, Visual Studio, Linux Makefile, Andorid, Code Blocks, CLion)

## Build and install:
### Projucer:
- Open synth_v1_0.jucer (see: https://juce.com/discover/projucer)
- Choose your IDE in the exporters side panel and click "Save and Open in IDE"
### A supported IDE:
- Build the project
- Copy synth_v1_0.vst3 to "C:\Program Files\Common Files\VST3"

## Credits:
- Project implemented at Lodz University of Technology (https://www.p.lodz.pl/en)
- AudioSynthesiserDemo by https://github.com/juce-framework/JUCE/blob/master/examples/Audio/AudioSynthesiserDemo.h
- simpleAudioPlayer by https://github.com/theaudioprogrammer/simpleAudioPlayerJUCE
- Tutorial: The Slider class by https://docs.juce.com/master/tutorial_slider_values.html
- Tutorial: Customise the look and feel of your app by https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
