#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#ifndef SFEMOVIE_DYNAMIC_FFMPEG
#define SFEMOVIE_DYNAMIC_FFMPEG 1
#endif

namespace sfe::AVFunc
{
	bool load();

#if SFEMOVIE_DYNAMIC_FFMPEG
	// avcodec
	typedef AVPacket* (*Func_av_packet_alloc)(void);
	typedef void (*Func_av_packet_unref)(AVPacket*);
	typedef AVCodecContext* (*Func_avcodec_alloc_context3)(const AVCodec*);
	typedef AVCodec* (*Func_avcodec_find_decoder)(enum AVCodecID);
	typedef void (*Func_avcodec_flush_buffers)(AVCodecContext*);
	typedef void (*Func_avcodec_free_context)(AVCodecContext**);
	typedef int (*Func_avcodec_open2)(AVCodecContext*, const AVCodec*, AVDictionary**);
	typedef int (*Func_avcodec_parameters_to_context)(AVCodecContext*, const AVCodecParameters*);
	typedef int (*Func_avcodec_receive_frame)(AVCodecContext*, AVFrame*);
	typedef int (*Func_avcodec_send_packet)(AVCodecContext*, const AVPacket*);

	// avformat
	typedef AVRational(*Func_av_guess_frame_rate)(AVFormatContext*, AVStream*, AVFrame*);
	typedef int (*Func_av_read_frame)(AVFormatContext*, AVPacket*);
	typedef AVFormatContext* (*Func_avformat_alloc_context)(void);
	typedef void (*Func_avformat_close_input)(AVFormatContext**);
	typedef int (*Func_avformat_find_stream_info)(AVFormatContext*, AVDictionary**);
	typedef int (*Func_avformat_open_input)(AVFormatContext**, const char*, const AVInputFormat*, AVDictionary**);
	typedef int (*Func_avformat_seek_file)(AVFormatContext*, int, int64_t, int64_t, int64_t, int);
	typedef AVIOContext* (*Func_avio_alloc_context)(unsigned char*, int, int, void*,
		int (*)(void*, uint8_t*, int), int (*)(void*, uint8_t*, int), int64_t(*)(void*, int64_t, int));

	// avutil
	typedef AVDictionaryEntry* (*Func_av_dict_get)(const AVDictionary*, const char*, const AVDictionaryEntry*, int);
	typedef AVFrame* (*Func_av_frame_alloc)(void);
	typedef void (*Func_av_frame_free)(AVFrame**);
	typedef void (*Func_av_free)(void*);
	typedef void (*Func_av_freep)(void*);
	typedef int (*Func_av_get_bytes_per_sample)(enum AVSampleFormat);
	typedef int64_t(*Func_av_get_default_channel_layout)(int);
	typedef int (*Func_av_image_alloc)(uint8_t* [4], int[4], int, int, enum AVPixelFormat, int);
	typedef void* (*Func_av_malloc)(size_t);
	typedef AVRational(*Func_av_mul_q)(AVRational, AVRational) av_const;
	typedef int (*Func_av_opt_set_int)(void*, const char*, int64_t, int);
	typedef int (*Func_av_opt_set_sample_fmt)(void*, const char*, enum AVSampleFormat, int);
	typedef int64_t(*Func_av_rescale_rnd)(int64_t, int64_t, int64_t, enum AVRounding) av_const;
	typedef int (*Func_av_samples_alloc)(uint8_t**, int*, int, int, enum AVSampleFormat, int);
	typedef int (*Func_av_samples_alloc_array_and_samples)(uint8_t***, int*, int, int, enum AVSampleFormat, int);
	typedef int (*Func_av_samples_get_buffer_size)(int*, int, int, enum AVSampleFormat, int);

	// swresample
	typedef struct SwrContext* (*Func_swr_alloc)(void);
	typedef int (*Func_swr_convert)(struct SwrContext*, uint8_t**, int, const uint8_t**, int);
	typedef int64_t(*Func_swr_get_delay)(struct SwrContext*, int64_t);
	typedef void (*Func_swr_free)(struct SwrContext**);
	typedef int (*Func_swr_init)(struct SwrContext*);

	// swscale
	typedef void (*Func_sws_freeContext)(struct SwsContext*);
	typedef struct SwsContext* (*Func_sws_getCachedContext)(struct SwsContext*, int, int, enum AVPixelFormat,
		int, int, enum AVPixelFormat, int, SwsFilter*, SwsFilter*, const double*);
	typedef int (*Func_sws_scale)(struct SwsContext*, const uint8_t* const [], const int[], int, int, uint8_t* const [], const int[]);

	extern Func_av_packet_alloc av_packet_alloc_f;
	extern Func_av_packet_unref av_packet_unref_f;
	extern Func_avcodec_alloc_context3 avcodec_alloc_context3_f;
	extern Func_avcodec_find_decoder avcodec_find_decoder_f;
	extern Func_avcodec_flush_buffers avcodec_flush_buffers_f;
	extern Func_avcodec_free_context avcodec_free_context_f;
	extern Func_avcodec_open2 avcodec_open2_f;
	extern Func_avcodec_parameters_to_context avcodec_parameters_to_context_f;
	extern Func_avcodec_receive_frame avcodec_receive_frame_f;
	extern Func_avcodec_send_packet avcodec_send_packet_f;

	extern Func_av_guess_frame_rate av_guess_frame_rate_f;
	extern Func_av_read_frame av_read_frame_f;
	extern Func_avformat_alloc_context avformat_alloc_context_f;
	extern Func_avformat_close_input avformat_close_input_f;
	extern Func_avformat_find_stream_info avformat_find_stream_info_f;
	extern Func_avformat_open_input avformat_open_input_f;
	extern Func_avformat_seek_file avformat_seek_file_f;
	extern Func_avio_alloc_context avio_alloc_context_f;

	extern Func_av_dict_get av_dict_get_f;
	extern Func_av_frame_alloc av_frame_alloc_f;
	extern Func_av_frame_free av_frame_free_f;
	extern Func_av_free av_free_f;
	extern Func_av_freep av_freep_f;
	extern Func_av_get_bytes_per_sample av_get_bytes_per_sample_f;
	extern Func_av_get_default_channel_layout av_get_default_channel_layout_f;
	extern Func_av_image_alloc av_image_alloc_f;
	extern Func_av_malloc av_malloc_f;
	extern Func_av_mul_q av_mul_q_f;
	extern Func_av_opt_set_int av_opt_set_int_f;
	extern Func_av_opt_set_sample_fmt av_opt_set_sample_fmt_f;
	extern Func_av_rescale_rnd av_rescale_rnd_f;
	extern Func_av_samples_alloc av_samples_alloc_f;
	extern Func_av_samples_alloc_array_and_samples av_samples_alloc_array_and_samples_f;
	extern Func_av_samples_get_buffer_size av_samples_get_buffer_size_f;

	extern Func_swr_alloc swr_alloc_f;
	extern Func_swr_convert swr_convert_f;
	extern Func_swr_get_delay swr_get_delay_f;
	extern Func_swr_free swr_free_f;
	extern Func_swr_init swr_init_f;

	extern Func_sws_freeContext sws_freeContext_f;
	extern Func_sws_getCachedContext sws_getCachedContext_f;
	extern Func_sws_scale sws_scale_f;
#endif
}

#if SFEMOVIE_DYNAMIC_FFMPEG

#define AV_PACKET_ALLOC sfe::AVFunc::av_packet_alloc_f
#define AV_PACKET_UNREF sfe::AVFunc::av_packet_unref_f
#define AVCODEC_ALLOC_CONTEXT3 sfe::AVFunc::avcodec_alloc_context3_f
#define AVCODEC_FIND_DECODER sfe::AVFunc::avcodec_find_decoder_f
#define AVCODEC_FLUSH_BUFFERS sfe::AVFunc::avcodec_flush_buffers_f
#define AVCODEC_FREE_CONTEXT sfe::AVFunc::avcodec_free_context_f
#define AVCODEC_OPEN2 sfe::AVFunc::avcodec_open2_f
#define AVCODEC_PARAMETERS_TO_CONTEXT sfe::AVFunc::avcodec_parameters_to_context_f
#define AVCODEC_RECEIVE_FRAME sfe::AVFunc::avcodec_receive_frame_f
#define AVCODEC_SEND_PACKET sfe::AVFunc::avcodec_send_packet_f

#define AV_GUESS_FRAME_RATE sfe::AVFunc::av_guess_frame_rate_f
#define AV_READ_FRAME sfe::AVFunc::av_read_frame_f
#define AVFORMAT_ALLOC_CONTEXT sfe::AVFunc::avformat_alloc_context_f
#define AVFORMAT_CLOSE_INPUT sfe::AVFunc::avformat_close_input_f
#define AVFORMAT_FIND_STREAM_INFO sfe::AVFunc::avformat_find_stream_info_f
#define AVFORMAT_OPEN_INPUT sfe::AVFunc::avformat_open_input_f
#define AVFORMAT_SEEK_FILE sfe::AVFunc::avformat_seek_file_f
#define AVIO_ALLOC_CONTEXT sfe::AVFunc::avio_alloc_context_f

#define AV_DICT_GET sfe::AVFunc::av_dict_get_f
#define AV_FRAME_ALLOC sfe::AVFunc::av_frame_alloc_f
#define AV_FRAME_FREE sfe::AVFunc::av_frame_free_f
#define AV_FREE sfe::AVFunc::av_free_f
#define AV_FREEP sfe::AVFunc::av_freep_f
#define AV_GET_BYTES_PER_SAMPLE sfe::AVFunc::av_get_bytes_per_sample_f
#define AV_GET_DEFAULT_CHANNEL_LAYOUT sfe::AVFunc::av_get_default_channel_layout_f
#define AV_IMAGE_ALLOC sfe::AVFunc::av_image_alloc_f
#define AV_MALLOC sfe::AVFunc::av_malloc_f
#define AV_MUL_Q sfe::AVFunc::av_mul_q_f
#define AV_OPT_SET_INT sfe::AVFunc::av_opt_set_int_f
#define AV_OPT_SET_SAMPLE_FMT sfe::AVFunc::av_opt_set_sample_fmt_f
#define AV_RESCALE_RND sfe::AVFunc::av_rescale_rnd_f
#define AV_SAMPLES_ALLOC sfe::AVFunc::av_samples_alloc_f
#define AV_SAMPLES_ALLOC_ARRAY_AND_SAMPLES sfe::AVFunc::av_samples_alloc_array_and_samples_f
#define AV_SAMPLES_GET_BUFFER_SIZE sfe::AVFunc::av_samples_get_buffer_size_f

#define SWR_ALLOC sfe::AVFunc::swr_alloc_f
#define SWR_CONVERT sfe::AVFunc::swr_convert_f
#define SWR_GET_DELAY sfe::AVFunc::swr_get_delay_f
#define SWR_FREE sfe::AVFunc::swr_free_f
#define SWR_INIT sfe::AVFunc::swr_init_f

#define SWS_FREECONTEXT sfe::AVFunc::sws_freeContext_f
#define SWS_GETCACHEDCONTEXT sfe::AVFunc::sws_getCachedContext_f
#define SWS_SCALE sfe::AVFunc::sws_scale_f

#else

#define AV_PACKET_ALLOC av_packet_alloc
#define AV_PACKET_UNREF av_packet_unref
#define AVCODEC_ALLOC_CONTEXT3 avcodec_alloc_context3
#define AVCODEC_FIND_DECODER avcodec_find_decoder
#define AVCODEC_FLUSH_BUFFERS avcodec_flush_buffers
#define AVCODEC_FREE_CONTEXT avcodec_free_context
#define AVCODEC_OPEN2 avcodec_open2
#define AVCODEC_PARAMETERS_TO_CONTEXT avcodec_parameters_to_context
#define AVCODEC_RECEIVE_FRAME avcodec_receive_frame
#define AVCODEC_SEND_PACKET avcodec_send_packet

#define AV_GUESS_FRAME_RATE av_guess_frame_rate
#define AV_READ_FRAME av_read_frame
#define AVFORMAT_ALLOC_CONTEXT avformat_alloc_context
#define AVFORMAT_CLOSE_INPUT avformat_close_input
#define AVFORMAT_FIND_STREAM_INFO avformat_find_stream_info
#define AVFORMAT_OPEN_INPUT avformat_open_input
#define AVFORMAT_SEEK_FILE avformat_seek_file
#define AVIO_ALLOC_CONTEXT avio_alloc_context

#define AV_DICT_GET av_dict_get
#define AV_FRAME_ALLOC av_frame_alloc
#define AV_FRAME_FREE av_frame_free
#define AV_FREE av_free
#define AV_FREEP av_freep
#define AV_GET_BYTES_PER_SAMPLE av_get_bytes_per_sample
#define AV_GET_DEFAULT_CHANNEL_LAYOUT av_get_default_channel_layout
#define AV_IMAGE_ALLOC av_image_alloc
#define AV_MALLOC av_malloc
#define AV_MUL_Q av_mul_q
#define AV_OPT_SET_INT av_opt_set_int
#define AV_OPT_SET_SAMPLE_FMT av_opt_set_sample_fmt
#define AV_RESCALE_RND av_rescale_rnd
#define AV_SAMPLES_ALLOC av_samples_alloc
#define AV_SAMPLES_ALLOC_ARRAY_AND_SAMPLES av_samples_alloc_array_and_samples
#define AV_SAMPLES_GET_BUFFER_SIZE av_samples_get_buffer_size

#define SWR_ALLOC swr_alloc
#define SWR_CONVERT swr_convert
#define SWR_GET_DELAY swr_get_delay
#define SWR_FREE swr_free
#define SWR_INIT swr_init

#define SWS_FREECONTEXT sws_freeContext
#define SWS_GETCACHEDCONTEXT sws_getCachedContext
#define SWS_SCALE sws_scale

#endif
