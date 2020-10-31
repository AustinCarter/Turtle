#include "turtpch.h"
#include "AudioDecoder.h"

#include <algorithm> 

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Turtle {

	void AudioDecoder::Init(const std::string& filepath, ma_bool32 looping)
	{
		ma_result result;

		m_Looping = looping;

		ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 2, 48000);
		

		result = ma_decoder_init_file(filepath.c_str(), &config, &m_Decoder);
		if (result != MA_SUCCESS) {
			TURT_CORE_ERROR("Failed to load Audio file");
		}
	}

	bool AudioDecoder::FinishedPlaying()
	{
		ma_uint64 cursorPos;
		ma_decoder_get_cursor_in_pcm_frames(&m_Decoder, &cursorPos);
		if (!m_Looping && cursorPos >= ma_decoder_get_length_in_pcm_frames(&m_Decoder))
			return true;
		return false;
	}

    void AudioDecoder::Play()
    {
        ma_decoder_seek_to_pcm_frame(&m_Decoder, 0);
    }

    uint32_t AudioDecoder::read_and_mix_s16(int16_t* outputBuffer, ma_uint32 frameCount)
	{
        /*
       The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
       contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
       doing that in this example.
       */
        int16_t temp[4096];
        ma_uint32 tempCapInFrames = ma_countof(temp) / m_ChannelCount;
        uint32_t totalFramesRead = 0;

        while (totalFramesRead < frameCount) {
            ma_uint32 iSample;
            ma_uint32 framesReadThisIteration;
            ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
            ma_uint32 framesToReadThisIteration = tempCapInFrames;
            if (framesToReadThisIteration > totalFramesRemaining) {
                framesToReadThisIteration = totalFramesRemaining;
            }

            framesReadThisIteration = (ma_uint32)ma_decoder_read_pcm_frames(&m_Decoder, temp, framesToReadThisIteration);
            if (framesReadThisIteration == 0) {
                break;
            }

            /* Mix the frames together. */
            for (iSample = 0; iSample < framesReadThisIteration * m_ChannelCount; ++iSample) {
                outputBuffer[totalFramesRead * m_ChannelCount + iSample] += temp[iSample] * m_Volume;
                //clipp to range
                outputBuffer[totalFramesRead * m_ChannelCount + iSample] = std::min<int16_t>(outputBuffer[totalFramesRead * m_ChannelCount + iSample], 32766);
            }

            totalFramesRead += framesReadThisIteration;

            if (framesReadThisIteration < framesToReadThisIteration) {
            	if(m_Looping) ma_decoder_seek_to_pcm_frame(&m_Decoder, 0);
                break;  /* Reached EOF. */
            }
        }

        return totalFramesRead;
	}
}