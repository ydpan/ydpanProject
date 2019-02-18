#pragma once
#include "ImageBuffer.h"
class ArvGst
{
public:
	ArvGst();
	~ArvGst();

	const char *	arv_pixel_format_to_gst_caps_string();
	PixelFormat	arv_pixel_format_from_gst_caps();
	const char *	arv_pixel_format_to_gst_0_10_caps_string();
	PixelFormat	arv_pixel_format_from_gst_0_10_caps();
};

