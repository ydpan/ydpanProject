#pragma once
class ArvDevice;
class ArvInterface
{
public:
	ArvInterface();
	~ArvInterface();

	const char *	arv_interface_get_device_address();
	const char *	arv_interface_get_device_id();
	const char *	arv_interface_get_device_model();
	const char *	arv_interface_get_device_physical_id();
	const char *	arv_interface_get_device_serial_nbr();
	const char *	arv_interface_get_device_vendor();
	const char *	arv_interface_get_device_protocol();
	unsigned int	arv_interface_get_n_devices();
	ArvDevice *	arv_interface_open_device();
	void	arv_interface_update_device_list();
};

