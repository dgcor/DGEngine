#include "AVFunc.hpp"
#include "Utils/LoadLibrary.h"

#if defined(_WIN32)
#define LIBAVCODEC_LIBRARY_NAME "avcodec-" AV_STRINGIFY(LIBAVCODEC_VERSION_MAJOR) ".dll"
#define LIBAVFORMAT_LIBRARY_NAME "avformat-" AV_STRINGIFY(LIBAVFORMAT_VERSION_MAJOR) ".dll"
#define LIBAVUTIL_LIBRARY_NAME "avutil-" AV_STRINGIFY(LIBAVUTIL_VERSION_MAJOR) ".dll"
#define LIBSWRESAMPLE_LIBRARY_NAME "swresample-" AV_STRINGIFY(LIBSWRESAMPLE_VERSION_MAJOR) ".dll"
#define LIBSWSCALE_LIBRARY_NAME "swscale-" AV_STRINGIFY(LIBSWSCALE_VERSION_MAJOR) ".dll"
#else
#define LIBAVCODEC_LIBRARY_NAME "libavcodec.so." AV_STRINGIFY(LIBAVCODEC_VERSION_MAJOR)
#define LIBAVFORMAT_LIBRARY_NAME "libavformat.so." AV_STRINGIFY(LIBAVFORMAT_VERSION_MAJOR)
#define LIBAVUTIL_LIBRARY_NAME "libavutil.so." AV_STRINGIFY(LIBAVUTIL_VERSION_MAJOR)
#define LIBSWRESAMPLE_LIBRARY_NAME "libswresample.so." AV_STRINGIFY(LIBSWRESAMPLE_VERSION_MAJOR)
#define LIBSWSCALE_LIBRARY_NAME "libswscale.so." AV_STRINGIFY(LIBSWSCALE_VERSION_MAJOR)
#endif

namespace sfe::AVFunc
{
#if SFEMOVIE_DYNAMIC_FFMPEG
	Func_av_packet_alloc av_packet_alloc_f{ nullptr };
	Func_av_packet_unref av_packet_unref_f{ nullptr };
	Func_avcodec_alloc_context3 avcodec_alloc_context3_f{ nullptr };
	Func_avcodec_find_decoder avcodec_find_decoder_f{ nullptr };
	Func_avcodec_flush_buffers avcodec_flush_buffers_f{ nullptr };
	Func_avcodec_free_context avcodec_free_context_f{ nullptr };
	Func_avcodec_open2 avcodec_open2_f{ nullptr };
	Func_avcodec_parameters_to_context avcodec_parameters_to_context_f{ nullptr };
	Func_avcodec_receive_frame avcodec_receive_frame_f{ nullptr };
	Func_avcodec_send_packet avcodec_send_packet_f{ nullptr };

	Func_av_guess_frame_rate av_guess_frame_rate_f{ nullptr };
	Func_av_read_frame av_read_frame_f{ nullptr };
	Func_avformat_alloc_context avformat_alloc_context_f{ nullptr };
	Func_avformat_close_input avformat_close_input_f{ nullptr };
	Func_avformat_find_stream_info avformat_find_stream_info_f{ nullptr };
	Func_avformat_open_input avformat_open_input_f{ nullptr };
	Func_avformat_seek_file avformat_seek_file_f{ nullptr };
	Func_avio_alloc_context avio_alloc_context_f{ nullptr };

	Func_av_dict_get av_dict_get_f{ nullptr };
	Func_av_frame_alloc av_frame_alloc_f{ nullptr };
	Func_av_frame_free av_frame_free_f{ nullptr };
	Func_av_free av_free_f{ nullptr };
	Func_av_freep av_freep_f{ nullptr };
	Func_av_get_bytes_per_sample av_get_bytes_per_sample_f{ nullptr };
	Func_av_get_default_channel_layout av_get_default_channel_layout_f{ nullptr };
	Func_av_image_alloc av_image_alloc_f{ nullptr };
	Func_av_malloc av_malloc_f{ nullptr };
	Func_av_mul_q av_mul_q_f{ nullptr };
	Func_av_opt_set_int av_opt_set_int_f{ nullptr };
	Func_av_opt_set_sample_fmt av_opt_set_sample_fmt_f{ nullptr };
	Func_av_rescale_rnd av_rescale_rnd_f{ nullptr };
	Func_av_samples_alloc av_samples_alloc_f{ nullptr };
	Func_av_samples_alloc_array_and_samples av_samples_alloc_array_and_samples_f{ nullptr };
	Func_av_samples_get_buffer_size av_samples_get_buffer_size_f{ nullptr };

	Func_swr_alloc swr_alloc_f{ nullptr };
	Func_swr_convert swr_convert_f{ nullptr };
	Func_swr_get_delay swr_get_delay_f{ nullptr };
	Func_swr_free swr_free_f{ nullptr };
	Func_swr_init swr_init_f{ nullptr };

	Func_sws_freeContext sws_freeContext_f{ nullptr };
	Func_sws_getCachedContext sws_getCachedContext_f{ nullptr };
	Func_sws_scale sws_scale_f{ nullptr };

	static bool hasLoadedFunctions()
	{
		if (av_packet_alloc_f == nullptr ||
			av_packet_unref_f == nullptr ||
			avcodec_alloc_context3_f == nullptr ||
			avcodec_find_decoder_f == nullptr ||
			avcodec_flush_buffers_f == nullptr ||
			avcodec_free_context_f == nullptr ||
			avcodec_open2_f == nullptr ||
			avcodec_parameters_to_context_f == nullptr ||
			avcodec_receive_frame_f == nullptr ||
			avcodec_send_packet_f == nullptr)
		{
			return false;
		}

		if (av_guess_frame_rate_f == nullptr ||
			av_read_frame_f == nullptr ||
			avformat_alloc_context_f == nullptr ||
			avformat_close_input_f == nullptr ||
			avformat_find_stream_info_f == nullptr ||
			avformat_open_input_f == nullptr ||
			avformat_seek_file_f == nullptr ||
			avio_alloc_context_f == nullptr)
		{
			return false;
		}

		if (av_dict_get_f == nullptr ||
			av_frame_alloc_f == nullptr ||
			av_frame_free_f == nullptr ||
			av_free_f == nullptr ||
			av_freep_f == nullptr ||
			av_get_bytes_per_sample_f == nullptr ||
			av_get_default_channel_layout_f == nullptr ||
			av_image_alloc_f == nullptr ||
			av_malloc_f == nullptr ||
			av_mul_q_f == nullptr ||
			av_opt_set_int_f == nullptr ||
			av_opt_set_sample_fmt_f == nullptr ||
			av_rescale_rnd_f == nullptr ||
			av_samples_alloc_f == nullptr ||
			av_samples_alloc_array_and_samples_f == nullptr ||
			av_samples_get_buffer_size_f == nullptr)
		{
			return false;
		}

		if (swr_alloc_f == nullptr ||
			swr_convert_f == nullptr ||
			swr_get_delay_f == nullptr ||
			swr_free_f == nullptr ||
			swr_init_f == nullptr)
		{
			return false;
		}

		if (sws_freeContext_f == nullptr ||
			sws_getCachedContext_f == nullptr ||
			sws_scale_f == nullptr)
		{
			return false;
		}

		return true;
	}

	bool load()
	{
		if (hasLoadedFunctions() == true)
		{
			return true;
		}

		auto hAvcodedLib = Utils::LoadExternalLibrary(LIBAVCODEC_LIBRARY_NAME);
		auto hAvformatLib = Utils::LoadExternalLibrary(LIBAVFORMAT_LIBRARY_NAME);
		auto hAvutilLib = Utils::LoadExternalLibrary(LIBAVUTIL_LIBRARY_NAME);
		auto hSwresampleLib = Utils::LoadExternalLibrary(LIBSWRESAMPLE_LIBRARY_NAME);
		auto hSwscaleLib = Utils::LoadExternalLibrary(LIBSWSCALE_LIBRARY_NAME);

		if (hAvcodedLib == nullptr ||
			hAvformatLib == nullptr ||
			hAvutilLib == nullptr ||
			hSwresampleLib == nullptr ||
			hSwscaleLib == nullptr)
		{
			return false;
		}

		av_packet_alloc_f = (Func_av_packet_alloc)Utils::GetFunctionAddress(hAvcodedLib, "av_packet_alloc");
		av_packet_unref_f = (Func_av_packet_unref)Utils::GetFunctionAddress(hAvcodedLib, "av_packet_unref");
		avcodec_alloc_context3_f = (Func_avcodec_alloc_context3)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_alloc_context3");
		avcodec_find_decoder_f = (Func_avcodec_find_decoder)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_find_decoder");
		avcodec_flush_buffers_f = (Func_avcodec_flush_buffers)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_flush_buffers");
		avcodec_free_context_f = (Func_avcodec_free_context)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_free_context");
		avcodec_open2_f = (Func_avcodec_open2)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_open2");
		avcodec_parameters_to_context_f = (Func_avcodec_parameters_to_context)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_parameters_to_context");
		avcodec_receive_frame_f = (Func_avcodec_receive_frame)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_receive_frame");
		avcodec_send_packet_f = (Func_avcodec_send_packet)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_send_packet");

		av_guess_frame_rate_f = (Func_av_guess_frame_rate)Utils::GetFunctionAddress(hAvformatLib, "av_guess_frame_rate");
		av_read_frame_f = (Func_av_read_frame)Utils::GetFunctionAddress(hAvformatLib, "av_read_frame");
		avformat_alloc_context_f = (Func_avformat_alloc_context)Utils::GetFunctionAddress(hAvformatLib, "avformat_alloc_context");
		avformat_close_input_f = (Func_avformat_close_input)Utils::GetFunctionAddress(hAvformatLib, "avformat_close_input");
		avformat_find_stream_info_f = (Func_avformat_find_stream_info)Utils::GetFunctionAddress(hAvformatLib, "avformat_find_stream_info");
		avformat_open_input_f = (Func_avformat_open_input)Utils::GetFunctionAddress(hAvformatLib, "avformat_open_input");
		avformat_seek_file_f = (Func_avformat_seek_file)Utils::GetFunctionAddress(hAvformatLib, "avformat_seek_file");
		avio_alloc_context_f = (Func_avio_alloc_context)Utils::GetFunctionAddress(hAvformatLib, "avio_alloc_context");

		av_dict_get_f = (Func_av_dict_get)Utils::GetFunctionAddress(hAvutilLib, "av_dict_get");
		av_frame_alloc_f = (Func_av_frame_alloc)Utils::GetFunctionAddress(hAvutilLib, "av_frame_alloc");
		av_frame_free_f = (Func_av_frame_free)Utils::GetFunctionAddress(hAvutilLib, "av_frame_free");
		av_free_f = (Func_av_free)Utils::GetFunctionAddress(hAvutilLib, "av_free");
		av_freep_f = (Func_av_freep)Utils::GetFunctionAddress(hAvutilLib, "av_freep");
		av_get_bytes_per_sample_f = (Func_av_get_bytes_per_sample)Utils::GetFunctionAddress(hAvutilLib, "av_get_bytes_per_sample");
		av_get_default_channel_layout_f = (Func_av_get_default_channel_layout)Utils::GetFunctionAddress(hAvutilLib, "av_get_default_channel_layout");
		av_image_alloc_f = (Func_av_image_alloc)Utils::GetFunctionAddress(hAvutilLib, "av_image_alloc");
		av_malloc_f = (Func_av_malloc)Utils::GetFunctionAddress(hAvutilLib, "av_malloc");
		av_mul_q_f = (Func_av_mul_q)Utils::GetFunctionAddress(hAvutilLib, "av_mul_q");
		av_opt_set_int_f = (Func_av_opt_set_int)Utils::GetFunctionAddress(hAvutilLib, "av_opt_set_int");
		av_opt_set_sample_fmt_f = (Func_av_opt_set_sample_fmt)Utils::GetFunctionAddress(hAvutilLib, "av_opt_set_sample_fmt");
		av_rescale_rnd_f = (Func_av_rescale_rnd)Utils::GetFunctionAddress(hAvutilLib, "av_rescale_rnd");
		av_samples_alloc_f = (Func_av_samples_alloc)Utils::GetFunctionAddress(hAvutilLib, "av_samples_alloc");
		av_samples_alloc_array_and_samples_f = (Func_av_samples_alloc_array_and_samples)Utils::GetFunctionAddress(hAvutilLib, "av_samples_alloc_array_and_samples");
		av_samples_get_buffer_size_f = (Func_av_samples_get_buffer_size)Utils::GetFunctionAddress(hAvutilLib, "av_samples_get_buffer_size");

		swr_alloc_f = (Func_swr_alloc)Utils::GetFunctionAddress(hSwresampleLib, "swr_alloc");
		swr_convert_f = (Func_swr_convert)Utils::GetFunctionAddress(hSwresampleLib, "swr_convert");
		swr_get_delay_f = (Func_swr_get_delay)Utils::GetFunctionAddress(hSwresampleLib, "swr_get_delay");
		swr_free_f = (Func_swr_free)Utils::GetFunctionAddress(hSwresampleLib, "swr_free");
		swr_init_f = (Func_swr_init)Utils::GetFunctionAddress(hSwresampleLib, "swr_init");

		sws_freeContext_f = (Func_sws_freeContext)Utils::GetFunctionAddress(hSwscaleLib, "sws_freeContext");
		sws_getCachedContext_f = (Func_sws_getCachedContext)Utils::GetFunctionAddress(hSwscaleLib, "sws_getCachedContext");
		sws_scale_f = (Func_sws_scale)Utils::GetFunctionAddress(hSwscaleLib, "sws_scale");

		return hasLoadedFunctions();
	}
#else
	bool load() { return true; }
#endif
}
