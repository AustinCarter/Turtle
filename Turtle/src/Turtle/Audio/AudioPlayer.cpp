#include "turtpch.h"
#include "AudioPlayer.h"


namespace Turtle {
	
	AudioPlayer::AudioPlayer()
	{
		m_DeviceConfig = ma_device_config_init(ma_device_type_playback);
		m_DeviceConfig.playback.format = ma_format_s16;//m_Decoder.outputFormat;
		m_DeviceConfig.playback.channels = m_ChannelCount;//m_Decoder.outputChannels;
		m_DeviceConfig.sampleRate = 48000;//m_Decoder.outputSampleRate;
		m_DeviceConfig.dataCallback = AudioPlayerDataCallback;
		m_DeviceConfig.pUserData = this;//new AudioUserData{ &m_Decoder, &m_Looping };

		if (ma_device_init(NULL, &m_DeviceConfig, &m_Device) != MA_SUCCESS) {
			TURT_CORE_ERROR("Failed to open playback device.\n");
		}

		if (ma_device_start(&m_Device) != MA_SUCCESS) {
			ma_device_uninit(&m_Device);
			TURT_CORE_ERROR("Failed to start playback device.\n");
		}
	}

	void AudioPlayer::Play(Ref<AudioDecoder> decoder)
	{
		//ma_event_init(&decoder->m_ResetEvent);
		//ma_event_wait(&decoder->m_ResetEvent);
		m_Decoders.emplace_back(decoder);
	}

	AudioPlayer::~AudioPlayer()
	{
		ma_device_uninit(&m_Device);
	}

	void AudioPlayer::AudioPlayerDataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		AudioPlayer* self = (AudioPlayer*)pDevice->pUserData;

		auto decoder = self->m_Decoders.begin();

		while(decoder != self->m_Decoders.end())
		{
			
			if (decoder->get()->FinishedPlaying())
			{
				Ref<AudioDecoder> decoderToReset = *decoder;
				decoder = self->m_Decoders.erase(decoder);
				decoderToReset->ResetCursor();
			}
			else
			{
				uint32_t framesRead = decoder->get()->read_and_mix_s16((int16_t*)pOutput, frameCount);
				decoder++;
			}
		}

		(void)pInput;
	}

}