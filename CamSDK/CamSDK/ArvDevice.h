#pragma once
#include "BaseDataDefine.h"
#include "ICamDevice.h"
class ArvStream;
class ArvChunkParser;
class ArvGc;
class ArvGcNode;
typedef enum ArvDeviceStatus {
	ARV_DEVICE_STATUS_UNKNOWN,//unknown status
    ARV_DEVICE_STATUS_SUCCESS,//no error has occured
	ARV_DEVICE_STATUS_TIMEOUT,//action failed on a timeout
	ARV_DEVICE_STATUS_WRITE_ERROR,//write on a read only node
	ARV_DEVICE_STATUS_TRANSFER_ERROR,//error during data transfer
	ARV_DEVICE_STATUS_NOT_CONNECTED,//device not connected
}_ArvDeviceStatus;

class ArvDevice: public ICamDevice
{
public:
	ArvDevice();
	~ArvDevice();

	ArvStream *	arv_device_create_stream();
	ArvChunkParser *	arv_device_create_chunk_parser();
	bool	arv_device_read_memory();
	bool	arv_device_write_memory();
	bool	arv_device_read_register();
	bool	arv_device_write_register();
	const char *	arv_device_get_genicam_xml();
	ArvGc *	arv_device_get_genicam();
	ArvGcNode *	arv_device_get_feature();
	ArvDeviceStatus	arv_device_get_status();
	void	arv_device_execute_command();
	bool	arv_device_get_boolean_feature_value();
	void	arv_device_set_boolean_feature_value();
	void	arv_device_set_string_feature_value();
	const char *	arv_device_get_string_feature_value();
	void	arv_device_set_integer_feature_value();
	lp_int64	arv_device_get_integer_feature_value();
	void	arv_device_get_integer_feature_bounds();
	void	arv_device_set_float_feature_value();
	double	arv_device_get_float_feature_value();
	void	arv_device_get_float_feature_bounds();
	lp_int64 *	arv_device_get_available_enumeration_feature_values();
	const char **	arv_device_get_available_enumeration_feature_values_as_strings();
};

