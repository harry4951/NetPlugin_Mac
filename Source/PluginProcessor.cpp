/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
NetPluginAudioProcessor::NetPluginAudioProcessor()
: ReadNetwork(buffer_)
{
}

NetPluginAudioProcessor::~NetPluginAudioProcessor()
{
}

//==============================================================================
const String NetPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NetPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NetPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double NetPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NetPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NetPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NetPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String NetPluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void NetPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NetPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NetPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NetPluginAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void NetPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    
    buffer_.switch_buffers();
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
       /* float* channelData = buffer.getWritePointer (channel);
        
        //simple clipper
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            if (channelData[i] > 0.01f)
                channelData[i] = 0.01f;
            else if (channelData[i] < - 0.01f)
                channelData[i] = -0.01f;
        }*/
        
        
        float* buffer_to_send = buffer_.GetHeadBuffer();

        //buffer = buffer_to_send;
        //for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        //{
            buffer.copyFrom(channel, 0, buffer_to_send, buffer.getNumSamples());
        //}
        // ..do something to the data...
    }
}

//==============================================================================
bool NetPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NetPluginAudioProcessor::createEditor()
{
    return new NetPluginAudioProcessorEditor (*this);
}

//==============================================================================
void NetPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NetPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NetPluginAudioProcessor();
}
