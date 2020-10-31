#pragma once

#include <miniaudio.h>


namespace Turtle {

	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();

		void Init(const std::string& filepath, ma_bool32 looping = false);
	private:

		ma_decoder m_Decoder;
		ma_device_config m_DeviceConfig;
		ma_device m_Device;
		ma_bool32 m_Looping = false;
	};
	void AudioPlayerDataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

}