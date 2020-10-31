#pragma once

#include "Turtle/Audio/AudioDecoder.h"

#include <miniaudio.h>

namespace Turtle {

	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();

		// void Init(const std::string& filepath, ma_bool32 looping = false);
		void AddSource(Ref<AudioDecoder> decoder);
	private:
		static void AudioPlayerDataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
		
		std::vector<Ref<AudioDecoder>> m_Decoders;
		ma_device_config m_DeviceConfig;
		ma_device m_Device;
		//Probably don't want this to be fixed in the future so make variable to facilitate possible future refactor
		uint32_t m_ChannelCount = 2;
	};

}