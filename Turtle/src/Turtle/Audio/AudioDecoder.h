#pragma once

#include <miniaudio.h>

namespace Turtle {
	
	class AudioDecoder
	{
	public:
		AudioDecoder() {}
		~AudioDecoder() { ma_decoder_uninit(&m_Decoder); }

		void Init(const std::string& filepath, ma_bool32 looping = false);

		void ResetCursor();

		uint32_t read_and_mix_s16(int16_t* outputBuffer, ma_uint32 frameCount);

		bool FinishedPlaying();
	private:

		ma_bool32 m_Looping = false;
		ma_decoder m_Decoder;
		//ma_event m_ResetEvent;
		float m_Volume = 0.05f; 


		//Probably don't want this to be fixed in the future so make variable to facilitate possible future refactor
		uint32_t m_ChannelCount = 2;

		//friend class AudioPlayer;
	};

}