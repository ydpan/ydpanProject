#pragma once
#include "BaseDataDefine.h"
#include "ImageBuffer.h"
//Class for generic camera control
typedef enum ArvAuto
{
	ARV_AUTO_OFF,//manual setting
	ARV_AUTO_ONCE,//automatic setting done once, then returns to manual
	ARV_AUTO_CONTINUOUS,//setting is adjusted continuously
}_ArvAuto;
typedef enum ArvAcquisitionMode
{
	ARV_ACQUISITION_MODE_CONTINUOUS,//continuous acquisition
	ARV_ACQUISITION_MODE_SINGLE_FRAME,//only one frame will be acquired
	ARV_ACQUISITION_MODE_MULTI_FRAME,//multiple frames will be acquired
}_ArvAcquisitionMode;
class ArvCamera;
class ArvStream;
class ArvDevice;
class ArvChunkParser;
class ArvCamera
{
public:
	ArvCamera();
	~ArvCamera();

	ArvAuto	arv_auto_from_string();
	const char *	arv_auto_to_string();
	ArvCamera *	arv_camera_new();
	ArvStream *	arv_camera_create_stream();
	ArvDevice *	arv_camera_get_device();
	const char *	arv_camera_get_vendor_name();
	const char *	arv_camera_get_model_name();
	const char *	arv_camera_get_device_id();
	void	arv_camera_get_sensor_size();
	void	arv_camera_set_region();
	void	arv_camera_get_region();
	void	arv_camera_get_x_offset_bounds();
	void	arv_camera_get_y_offset_bounds();
	void	arv_camera_get_height_bounds();
	void	arv_camera_get_width_bounds();
	void	arv_camera_set_binning();
	void	arv_camera_get_binning();
	void	arv_camera_get_x_binning_bounds();
	void	arv_camera_get_y_binning_bounds();
	bool	arv_camera_is_binning_available();
	void	arv_camera_set_pixel_format();
	PixelFormat	arv_camera_get_pixel_format();
	const char *	arv_camera_get_pixel_format_as_string();
	void	arv_camera_set_pixel_format_from_string();
	lp_uint64 *	arv_camera_get_available_pixel_formats();
	const char **	arv_camera_get_available_pixel_formats_as_display_names();
	const char **	arv_camera_get_available_pixel_formats_as_strings();
	void	arv_camera_start_acquisition();
	void	arv_camera_stop_acquisition();
	void	arv_camera_abort_acquisition();
	ArvBuffer *	arv_camera_acquisition();
	void	arv_camera_set_acquisition_mode();
	ArvAcquisitionMode	arv_camera_get_acquisition_mode();
	void	arv_camera_set_frame_count();
	lp_uint64	arv_camera_get_frame_count();
	void	arv_camera_get_frame_count_bounds();
	bool	arv_camera_is_frame_rate_available();
	void	arv_camera_set_frame_rate();
	double	arv_camera_get_frame_rate();
	void	arv_camera_get_frame_rate_bounds();
	void	arv_camera_set_trigger();
	void	arv_camera_set_trigger_source();
	const char *	arv_camera_get_trigger_source();
	const char **	arv_camera_get_available_trigger_sources();
	const char **	arv_camera_get_available_triggers();
	void	arv_camera_clear_triggers();
	void	arv_camera_software_trigger();
	bool	arv_camera_is_exposure_time_available();
	bool	arv_camera_is_exposure_auto_available();
	void	arv_camera_set_exposure_time();
	double	arv_camera_get_exposure_time();
	void	arv_camera_get_exposure_time_bounds();
	void	arv_camera_set_exposure_time_auto();
	ArvAuto	arv_camera_get_exposure_time_auto();
	bool	arv_camera_is_gain_available();
	bool	arv_camera_is_gain_auto_available();
	void	arv_camera_set_gain();
	double	arv_camera_get_gain();
	void	arv_camera_get_gain_bounds();
	void	arv_camera_set_gain_auto();
	ArvAuto	arv_camera_get_gain_auto();
	lp_uint32	arv_camera_get_payload();
	bool	arv_camera_is_gv_device();
	int	arv_camera_gv_get_n_stream_channels();
	void	arv_camera_gv_select_stream_channel();
	int	arv_camera_gv_get_current_stream_channel();
	lp_int64	arv_camera_gv_get_packet_delay();
	void	arv_camera_gv_set_packet_delay();
	lp_uint32	arv_camera_gv_get_packet_size();
	void	arv_camera_gv_set_packet_size();
	lp_uint32	arv_camera_gv_auto_packet_size();
	void	arv_camera_gv_set_stream_options();
	bool	arv_camera_is_uv_device();
	lp_uint32	arv_camera_uv_get_bandwidth();
	void	arv_camera_uv_get_bandwidth_bounds();
	bool	arv_camera_uv_is_bandwidth_control_available();
	void	arv_camera_uv_set_bandwidth();
	bool	arv_camera_get_chunk_mode();
	bool	arv_camera_get_chunk_state();
	void	arv_camera_set_chunk_mode();
	void	arv_camera_set_chunk_state();
	void	arv_camera_set_chunks();
	ArvChunkParser *	arv_camera_create_chunk_parser();
	const char *	arv_acquisition_mode_to_string();
	ArvAcquisitionMode	arv_acquisition_mode_from_string();
};

