#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

namespace sfe::AVFunc
{
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
	typedef int (*Func_avformat_open_input)(AVFormatContext**, const char*, ff_const59 AVInputFormat*, AVDictionary**);
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
	typedef int (*Func_av_get_channel_layout_nb_channels)(uint64_t);
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

	bool load();

	extern Func_av_packet_alloc av_packet_alloc;
	extern Func_av_packet_unref av_packet_unref;
	extern Func_avcodec_alloc_context3 avcodec_alloc_context3;
	extern Func_avcodec_find_decoder avcodec_find_decoder;
	extern Func_avcodec_flush_buffers avcodec_flush_buffers;
	extern Func_avcodec_free_context avcodec_free_context;
	extern Func_avcodec_open2 avcodec_open2;
	extern Func_avcodec_parameters_to_context avcodec_parameters_to_context;
	extern Func_avcodec_receive_frame avcodec_receive_frame;
	extern Func_avcodec_send_packet avcodec_send_packet;

	extern Func_av_guess_frame_rate av_guess_frame_rate;
	extern Func_av_read_frame av_read_frame;
	extern Func_avformat_alloc_context avformat_alloc_context;
	extern Func_avformat_close_input avformat_close_input;
	extern Func_avformat_find_stream_info avformat_find_stream_info;
	extern Func_avformat_open_input avformat_open_input;
	extern Func_avformat_seek_file avformat_seek_file;
	extern Func_avio_alloc_context avio_alloc_context;

	extern Func_av_dict_get av_dict_get;
	extern Func_av_frame_alloc av_frame_alloc;
	extern Func_av_frame_free av_frame_free;
	extern Func_av_free av_free;
	extern Func_av_freep av_freep;
	extern Func_av_get_bytes_per_sample av_get_bytes_per_sample;
	extern Func_av_get_channel_layout_nb_channels av_get_channel_layout_nb_channels;
	extern Func_av_get_default_channel_layout av_get_default_channel_layout;
	extern Func_av_image_alloc av_image_alloc;
	extern Func_av_malloc av_malloc;
	extern Func_av_mul_q av_mul_q;
	extern Func_av_opt_set_int av_opt_set_int;
	extern Func_av_opt_set_sample_fmt av_opt_set_sample_fmt;
	extern Func_av_rescale_rnd av_rescale_rnd;
	extern Func_av_samples_alloc av_samples_alloc;
	extern Func_av_samples_alloc_array_and_samples av_samples_alloc_array_and_samples;
	extern Func_av_samples_get_buffer_size av_samples_get_buffer_size;

	extern Func_swr_alloc swr_alloc;
	extern Func_swr_convert swr_convert;
	extern Func_swr_get_delay swr_get_delay;
	extern Func_swr_free swr_free;
	extern Func_swr_init swr_init;

	extern Func_sws_freeContext sws_freeContext;
	extern Func_sws_getCachedContext sws_getCachedContext;
	extern Func_sws_scale sws_scale;
}
