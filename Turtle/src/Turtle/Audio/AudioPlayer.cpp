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

	void AudioPlayer::AddSource(Ref<AudioDecoder> decoder)
	{
		m_Decoders.emplace_back(decoder);
	}

	AudioPlayer::~AudioPlayer()
	{
		ma_device_uninit(&m_Device);
	}

	void AudioPlayer::AudioPlayerDataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		// AudioUserData userData = *(AudioUserData*)(pDevice->pUserData);
		// ma_bool32 isLooping = *userData.Looping;

		
		// ma_decoder* pDecoder = userData.Decoder;

		//ma_uint64 cursorPos;
		//ma_decoder_get_cursor_in_pcm_frames(pDecoder, &cursorPos);
		//if (cursorPos >= ma_decoder_get_length_in_pcm_frames(pDecoder) && !isLooping)
		//	userData.Finished = true; 

		// 	ma_decoder_seek_to_pcm_frame(pDecoder, 0);

		//if (pDecoder == NULL) {
		//	return;
		//}

		AudioPlayer* self = (AudioPlayer*)pDevice->pUserData;

		for(auto& decoder : self->m_Decoders)
		{
			//ma_data_source_read_pcm_frames(&(decoder->m_Decoder), pOutput, frameCount, NULL, decoder->m_Looping);
			uint32_t framesRead = decoder->read_and_mix_s16((int16_t*)pOutput, frameCount);
		}

		/*
		A decoder is a data source which means you can seemlessly plug it into the ma_data_source API. We can therefore take advantage
		of the "loop" parameter of ma_data_source_read_pcm_frames() to handle looping for us.
		*/
		// ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, isLooping);

		(void)pInput;
	}

}