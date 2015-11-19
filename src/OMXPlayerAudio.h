#pragma once
#include "ofMain.h"


#include "LIBAV_INCLUDES.h"


#include "PCMRemap.h"

#include "OMXReader.h"
#include "OMXClock.h"
#include "OMXStreamInfo.h"
#include "OMXAudio.h"


#include "OMXAudioCodecOMX.h"

#include "OMXThread.h"

#include <deque>
#include <string>
#include <sys/types.h>


class OMXPlayerAudio : public OMXThread
{
	protected:
		int                       m_stream_id;
		std::deque<OMXPacket *>   m_packets;
		bool                      m_open;
		OMXStreamInfo            m_hints;
		double                    m_iCurrentPts;
		pthread_cond_t            m_packet_cond;
		pthread_cond_t            m_audio_cond;
		pthread_mutex_t           m_lock;
		pthread_mutex_t           m_lock_decoder;
		OMXClock                  *omxClock;
		OMXReader                 *m_omx_reader;
		OMXAudio                 *m_decoder;
		std::string               m_codec_name;
		std::string               m_device;
		bool                      m_use_passthrough;
		bool                      m_use_hw_decode;
		OMXAudio::EEncoded  m_passthrough;
		bool                      m_hw_decode;
		bool                      m_boost_on_downmix;
		bool                      m_bAbort;
		bool                      m_use_thread;
		bool                      m_flush;
		enum PCMChannels          *m_pChannelMap;
		unsigned int              m_cached_size;
		OMXAudioCodecOMX         *m_pAudioCodec;
		int                       m_speed;

		int64_t m_errortime; //timestamp of last time we measured


		bool   m_player_error;


		void Lock();
		void UnLock();
		void LockDecoder();
		void UnLockDecoder();
	private:
	public:
		OMXPlayerAudio();
		~OMXPlayerAudio();
		bool Open(OMXStreamInfo& hints,
                  OMXClock *av_clock,
                  OMXReader *omx_reader,
		          std::string device,
                  bool passthrough,
                  bool hw_decode,
		          bool boost_on_downmix,
                  bool use_thread);
    
		bool Close();
		bool Decode(OMXPacket *pkt);
		void Process();
		void Flush();
		bool AddPacket(OMXPacket *pkt);
		bool OpenAudioCodec();
		void CloseAudioCodec();
		OMXAudio::EEncoded IsPassthrough(OMXStreamInfo hints);
		bool OpenDecoder();
		bool CloseDecoder();
		//double GetDelay();
		//double GetCacheTime();
		double GetCurrentPTS()
		{
			return m_iCurrentPts;
		};
		void WaitCompletion();
		void submitEOS();
		bool EOS();

		unsigned int GetCached()
		{
			Lock();
			unsigned int cached_size = m_cached_size;
			UnLock();
			return cached_size;

		};

		void setCurrentVolume(long nVolume);
		long getCurrentVolume();
		void setSpeed(int iSpeed);
		bool Error()
		{
			return !m_player_error;
		};
};
