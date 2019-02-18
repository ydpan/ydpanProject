#ifndef _ARVGC_H_
#define _ARVGC_H_
#include "BaseDataDefine.h"
#include "ImageBuffer.h"
#include "ArvDevice.h"
class ArvGc
{
public:
	ArvGc(ICamDevice *pDevice,const void *pXml,size_t size);
	~ArvGc();
	IGcNode *arv_gc_get_node(const char *pName);
	void arv_gc_set_default_node_data(const char *node_name, const char *node_data);
	ICamDevice *arv_gc_get_device();
private:
	IDomDocument *m_documemt;
	ICamDevice *m_device;
	ArvBuffer *buffer;
};

#endif