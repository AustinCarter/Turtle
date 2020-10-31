#include "turtpch.h"
#include "AudioPlayer.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Turtle {
	struct AudioUserData
	{
		ma_decoder* Decoder;
		ma_bool32* Looping;
	};
	
	AudioPlayer::AudioPlayer(){}

	AudioPlayer::~AudioPlayer()
	{
		ma_device_uninit(&m_Device);
		ma_decoder_uninit(&m_Decoder);
	}

	void AudioPlayer::Init(const std::string& filepath, ma_bool32 looping)
	{
		ma_result result;

		m_Looping = looping;
		

		result = ma_decoder_init_file(filepath.c_str(), NULL, &m_Decoder);
		if (result != MA_SUCCESS) {
			TURT_CORE_ERROR("Failed to load Audio file");
		}

		m_DeviceConfig = ma_device_config_init(ma_device_type_playback);
		m_DeviceConfig.playback.format = m_Decoder.outputFormat;
		m_DeviceConfig.playback.channels = m_Decoder.outputChannels;
		m_DeviceConfig.sampleRate = m_Decoder.outputSampleRate;
		m_DeviceConfig.dataCallback = AudioPlayerDataCallback;
		m_DeviceConfig.pUserData = new AudioUserData{ &m_Decoder, &m_Looping };

		if (ma_device_init(NULL, &m_DeviceConfig, &m_Device) != MA_SUCCESS) {
			ma_decoder_uninit(&m_Decoder);
			TURT_CORE_ERROR("Failed to open playback device.\n");
		}

		if (ma_device_start(&m_Device) != MA_SUCCESS) {
			ma_device_uninit(&m_Device);
			ma_decoder_uninit(&m_Decoder);
			TURT_CORE_ERROR("Failed to start playback device.\n");
		}
	}

	void AudioPlayerDataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		AudioUserData userData = *(AudioUserData*)(pDevice->pUserData);
		ma_bool32 isLooping = *userData.Looping;

		
		ma_decoder* pDecoder = userData.Decoder;

		//ma_uint64 cursorPos;
		//ma_decoder_get_cursor_in_pcm_frames(pDecoder, &cursorPos);
		//if (cursorPos >= ma_decoder_get_length_in_pcm_frames(pDecoder) && !isLooping)
		//	userData.Finished = true; 

		// 	ma_decoder_seek_to_pcm_frame(pDecoder, 0);

		if (pDecoder == NULL) {
			return;
		}

		/*
		A decoder is a data source which means you can seemlessly plug it into the ma_data_source API. We can therefore take advantage
		of the "loop" parameter of ma_data_source_read_pcm_frames() to handle looping for us.
		*/
		ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, isLooping);

		(void)pInput;
	}

}