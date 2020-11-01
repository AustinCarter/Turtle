#include "turtpch.h"
#include "AudioPlayer.h"


namespace Turtle {

	struct AudioPlayerData
	{
		std::vector<Ref<AudioDecoder>> Decoders;
		ma_device_config DeviceConfig;
		ma_device Device;
		//Probably don't want this to be fixed in the future so make variable to facilitate possible future refactor
		uint32_t ChannelCount = 2;
	};

	static AudioPlayerData s_AudioData;

	void AudioPlayer::Init()
	{
		s_AudioData.DeviceConfig = ma_device_config_init(ma_device_type_playback);
		s_AudioData.DeviceConfig.playback.format = ma_format_s16;
		s_AudioData.DeviceConfig.playback.channels = s_AudioData.ChannelCount;
		s_AudioData.DeviceConfig.sampleRate = 48000;
		s_AudioData.DeviceConfig.dataCallback = AudioPlayerDataCallback;
		// s_AudioData.DeviceConfig.pUserData = this;

		if (ma_device_init(NULL, &s_AudioData.DeviceConfig, &s_AudioData.Device) != MA_SUCCESS) {
			TURT_CORE_ERROR("Failed to open playback device.\n");
		}

		if (ma_device_start(&s_AudioData.Device) != MA_SUCCESS) {
			ma_device_uninit(&s_AudioData.Device);
			TURT_CORE_ERROR("Failed to start playback device.\n");
		}
	}

	void AudioPlayer::Reset()
	{
		s_AudioData.Decoders.clear();
	}


	void AudioPlayer::Play(Ref<AudioDecoder> decoder)
	{
		s_AudioData.Decoders.emplace_back(decoder);
	}	

	void AudioPlayer::Pause(Ref<AudioDecoder> decoder)
	{
		decoder->Pause();
	}
	
	AudioPlayer::~AudioPlayer()
	{
		//Might move to shutdown
		s_AudioData.Decoders.clear();
		ma_device_uninit(&s_AudioData.Device);
	}

	void AudioPlayer::AudioPlayerDataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		// AudioPlayer* self = (AudioPlayer*)pDevice->pUserData;

		auto decoder = s_AudioData.Decoders.begin();

		while(decoder != s_AudioData.Decoders.end())
		{
			
			if (decoder->get()->FinishedPlaying())
			{
				Ref<AudioDecoder> decoderToReset = *decoder;
				decoder = s_AudioData.Decoders.erase(decoder);
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