#pragma once
//Device discovery and instantiation
class ArvDevice;
class Arv
{
public:
	Arv();
	~Arv();

	void	arv_update_device_list();
	unsigned int	arv_get_n_devices();
	const char *	arv_get_device_id();
	const char *	arv_get_device_physical_id();
	const char *	arv_get_device_model();
	const char *	arv_get_device_serial_nbr();
	const char *	arv_get_device_vendor();
	const char *	arv_get_device_address();
	const char *	arv_get_device_protocol();
	ArvDevice *	arv_open_device();
	unsigned int	arv_get_n_interfaces();
	const char *	arv_get_interface_id();
	void	arv_disable_interface();
	void	arv_enable_interface();
	void	arv_shutdown();
};

