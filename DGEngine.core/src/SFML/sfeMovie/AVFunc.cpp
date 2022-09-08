#include "AVFunc.hpp"
#include "Utils/LoadLibrary.h"

namespace sfe::AVFunc
{
	Func_av_packet_alloc av_packet_alloc{ nullptr };
	Func_av_packet_unref av_packet_unref{ nullptr };
	Func_avcodec_alloc_context3 avcodec_alloc_context3{ nullptr };
	Func_avcodec_find_decoder avcodec_find_decoder{ nullptr };
	Func_avcodec_flush_buffers avcodec_flush_buffers{ nullptr };
	Func_avcodec_free_context avcodec_free_context{ nullptr };
	Func_avcodec_open2 avcodec_open2{ nullptr };
	Func_avcodec_parameters_to_context avcodec_parameters_to_context{ nullptr };
	Func_avcodec_receive_frame avcodec_receive_frame{ nullptr };
	Func_avcodec_send_packet avcodec_send_packet{ nullptr };

	Func_av_guess_frame_rate av_guess_frame_rate{ nullptr };
	Func_av_read_frame av_read_frame{ nullptr };
	Func_avformat_alloc_context avformat_alloc_context{ nullptr };
	Func_avformat_close_input avformat_close_input{ nullptr };
	Func_avformat_find_stream_info avformat_find_stream_info{ nullptr };
	Func_avformat_open_input avformat_open_input{ nullptr };
	Func_avformat_seek_file avformat_seek_file{ nullptr };
	Func_avio_alloc_context avio_alloc_context{ nullptr };

	Func_av_dict_get av_dict_get{ nullptr };
	Func_av_frame_alloc av_frame_alloc{ nullptr };
	Func_av_frame_free av_frame_free{ nullptr };
	Func_av_free av_free{ nullptr };
	Func_av_freep av_freep{ nullptr };
	Func_av_get_bytes_per_sample av_get_bytes_per_sample{ nullptr };
	Func_av_get_channel_layout_nb_channels av_get_channel_layout_nb_channels{ nullptr };
	Func_av_get_default_channel_layout av_get_default_channel_layout{ nullptr };
	Func_av_image_alloc av_image_alloc{ nullptr };
	Func_av_malloc av_malloc{ nullptr };
	Func_av_mul_q av_mul_q{ nullptr };
	Func_av_opt_set_int av_opt_set_int{ nullptr };
	Func_av_opt_set_sample_fmt av_opt_set_sample_fmt{ nullptr };
	Func_av_rescale_rnd av_rescale_rnd{ nullptr };
	Func_av_samples_alloc av_samples_alloc{ nullptr };
	Func_av_samples_alloc_array_and_samples av_samples_alloc_array_and_samples{ nullptr };
	Func_av_samples_get_buffer_size av_samples_get_buffer_size{ nullptr };

	Func_swr_alloc swr_alloc{ nullptr };
	Func_swr_convert swr_convert{ nullptr };
	Func_swr_get_delay swr_get_delay{ nullptr };
	Func_swr_free swr_free{ nullptr };
	Func_swr_init swr_init{ nullptr };

	Func_sws_freeContext sws_freeContext{ nullptr };
	Func_sws_getCachedContext sws_getCachedContext{ nullptr };
	Func_sws_scale sws_scale{ nullptr };

	static bool hasLoadedFunctions()
	{
		if (av_packet_alloc == nullptr ||
			av_packet_unref == nullptr ||
			avcodec_alloc_context3 == nullptr ||
			avcodec_find_decoder == nullptr ||
			avcodec_flush_buffers == nullptr ||
			avcodec_free_context == nullptr ||
			avcodec_open2 == nullptr ||
			avcodec_parameters_to_context == nullptr ||
			avcodec_receive_frame == nullptr ||
			avcodec_send_packet == nullptr)
		{
			return false;
		}

		if (av_guess_frame_rate == nullptr ||
			av_read_frame == nullptr ||
			avformat_alloc_context == nullptr ||
			avformat_close_input == nullptr ||
			avformat_find_stream_info == nullptr ||
			avformat_open_input == nullptr ||
			avformat_seek_file == nullptr ||
			avio_alloc_context == nullptr)
		{
			return false;
		}

		if (av_dict_get == nullptr ||
			av_frame_alloc == nullptr ||
			av_frame_free == nullptr ||
			av_free == nullptr ||
			av_freep == nullptr ||
			av_get_bytes_per_sample == nullptr ||
			av_get_channel_layout_nb_channels == nullptr ||
			av_get_default_channel_layout == nullptr ||
			av_image_alloc == nullptr ||
			av_malloc == nullptr ||
			av_mul_q == nullptr ||
			av_opt_set_int == nullptr ||
			av_opt_set_sample_fmt == nullptr ||
			av_rescale_rnd == nullptr ||
			av_samples_alloc == nullptr ||
			av_samples_alloc_array_and_samples == nullptr ||
			av_samples_get_buffer_size == nullptr)
		{
			return false;
		}

		if (swr_alloc == nullptr ||
			swr_convert == nullptr ||
			swr_get_delay == nullptr ||
			swr_free == nullptr ||
			swr_init == nullptr)
		{
			return false;
		}

		if (sws_freeContext == nullptr ||
			sws_getCachedContext == nullptr ||
			sws_scale == nullptr)
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

#if defined(_WIN32)
		auto hAvcodedLib = Utils::LoadExternalLibrary("avcodec-58.dll");
		auto hAvformatLib = Utils::LoadExternalLibrary("avformat-58.dll");
		auto hAvutilLib = Utils::LoadExternalLibrary("avutil-56.dll");
		auto hSwresampleLib = Utils::LoadExternalLibrary("swresample-3.dll");
		auto hSwscaleLib = Utils::LoadExternalLibrary("swscale-5.dll");
#else
		auto hAvcodedLib = Utils::LoadExternalLibrary("libavcodec.so.58");
		auto hAvformatLib = Utils::LoadExternalLibrary("libavformat.so.58");
		auto hAvutilLib = Utils::LoadExternalLibrary("libavutil.so.56");
		auto hSwresampleLib = Utils::LoadExternalLibrary("libswresample.so.3");
		auto hSwscaleLib = Utils::LoadExternalLibrary("libswscale.so.5");
#endif
		if (hAvcodedLib == nullptr ||
			hAvformatLib == nullptr ||
			hAvutilLib == nullptr ||
			hSwresampleLib == nullptr ||
			hSwscaleLib == nullptr)
		{
			return false;
		}

		av_packet_alloc = (Func_av_packet_alloc)Utils::GetFunctionAddress(hAvcodedLib, "av_packet_alloc");
		av_packet_unref = (Func_av_packet_unref)Utils::GetFunctionAddress(hAvcodedLib, "av_packet_unref");
		avcodec_alloc_context3 = (Func_avcodec_alloc_context3)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_alloc_context3");
		avcodec_find_decoder = (Func_avcodec_find_decoder)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_find_decoder");
		avcodec_flush_buffers = (Func_avcodec_flush_buffers)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_flush_buffers");
		avcodec_free_context = (Func_avcodec_free_context)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_free_context");
		avcodec_open2 = (Func_avcodec_open2)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_open2");
		avcodec_parameters_to_context = (Func_avcodec_parameters_to_context)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_parameters_to_context");
		avcodec_receive_frame = (Func_avcodec_receive_frame)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_receive_frame");
		avcodec_send_packet = (Func_avcodec_send_packet)Utils::GetFunctionAddress(hAvcodedLib, "avcodec_send_packet");

		av_guess_frame_rate = (Func_av_guess_frame_rate)Utils::GetFunctionAddress(hAvformatLib, "av_guess_frame_rate");
		av_read_frame = (Func_av_read_frame)Utils::GetFunctionAddress(hAvformatLib, "av_read_frame");
		avformat_alloc_context = (Func_avformat_alloc_context)Utils::GetFunctionAddress(hAvformatLib, "avformat_alloc_context");
		avformat_close_input = (Func_avformat_close_input)Utils::GetFunctionAddress(hAvformatLib, "avformat_close_input");
		avformat_find_stream_info = (Func_avformat_find_stream_info)Utils::GetFunctionAddress(hAvformatLib, "avformat_find_stream_info");
		avformat_open_input = (Func_avformat_open_input)Utils::GetFunctionAddress(hAvformatLib, "avformat_open_input");
		avformat_seek_file = (Func_avformat_seek_file)Utils::GetFunctionAddress(hAvformatLib, "avformat_seek_file");
		avio_alloc_context = (Func_avio_alloc_context)Utils::GetFunctionAddress(hAvformatLib, "avio_alloc_context");

		av_dict_get = (Func_av_dict_get)Utils::GetFunctionAddress(hAvutilLib, "av_dict_get");
		av_frame_alloc = (Func_av_frame_alloc)Utils::GetFunctionAddress(hAvutilLib, "av_frame_alloc");
		av_frame_free = (Func_av_frame_free)Utils::GetFunctionAddress(hAvutilLib, "av_frame_free");
		av_free = (Func_av_free)Utils::GetFunctionAddress(hAvutilLib, "av_free");
		av_freep = (Func_av_freep)Utils::GetFunctionAddress(hAvutilLib, "av_freep");
		av_get_bytes_per_sample = (Func_av_get_bytes_per_sample)Utils::GetFunctionAddress(hAvutilLib, "av_get_bytes_per_sample");
		av_get_channel_layout_nb_channels = (Func_av_get_channel_layout_nb_channels)Utils::GetFunctionAddress(hAvutilLib, "av_get_channel_layout_nb_channels");
		av_get_default_channel_layout = (Func_av_get_default_channel_layout)Utils::GetFunctionAddress(hAvutilLib, "av_get_default_channel_layout");
		av_image_alloc = (Func_av_image_alloc)Utils::GetFunctionAddress(hAvutilLib, "av_image_alloc");
		av_malloc = (Func_av_malloc)Utils::GetFunctionAddress(hAvutilLib, "av_malloc");
		av_mul_q = (Func_av_mul_q)Utils::GetFunctionAddress(hAvutilLib, "av_mul_q");
		av_opt_set_int = (Func_av_opt_set_int)Utils::GetFunctionAddress(hAvutilLib, "av_opt_set_int");
		av_opt_set_sample_fmt = (Func_av_opt_set_sample_fmt)Utils::GetFunctionAddress(hAvutilLib, "av_opt_set_sample_fmt");
		av_rescale_rnd = (Func_av_rescale_rnd)Utils::GetFunctionAddress(hAvutilLib, "av_rescale_rnd");
		av_samples_alloc = (Func_av_samples_alloc)Utils::GetFunctionAddress(hAvutilLib, "av_samples_alloc");
		av_samples_alloc_array_and_samples = (Func_av_samples_alloc_array_and_samples)Utils::GetFunctionAddress(hAvutilLib, "av_samples_alloc_array_and_samples");
		av_samples_get_buffer_size = (Func_av_samples_get_buffer_size)Utils::GetFunctionAddress(hAvutilLib, "av_samples_get_buffer_size");

		swr_alloc = (Func_swr_alloc)Utils::GetFunctionAddress(hSwresampleLib, "swr_alloc");
		swr_convert = (Func_swr_convert)Utils::GetFunctionAddress(hSwresampleLib, "swr_convert");
		swr_get_delay = (Func_swr_get_delay)Utils::GetFunctionAddress(hSwresampleLib, "swr_get_delay");
		swr_free = (Func_swr_free)Utils::GetFunctionAddress(hSwresampleLib, "swr_free");
		swr_init = (Func_swr_init)Utils::GetFunctionAddress(hSwresampleLib, "swr_init");

		sws_freeContext = (Func_sws_freeContext)Utils::GetFunctionAddress(hSwscaleLib, "sws_freeContext");
		sws_getCachedContext = (Func_sws_getCachedContext)Utils::GetFunctionAddress(hSwscaleLib, "sws_getCachedContext");
		sws_scale = (Func_sws_scale)Utils::GetFunctionAddress(hSwscaleLib, "sws_scale");

		return hasLoadedFunctions();
	}
}
