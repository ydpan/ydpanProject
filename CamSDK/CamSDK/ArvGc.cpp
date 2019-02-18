#include "ArvGc.h"
#include "ArvDomDocument.h"
ArvGc::ArvGc(ICamDevice *pDevice, const void *pXml, size_t size)
{
	m_documemt = new ArvDomDocument();
	m_documemt->Init(pXml, size);
}

ArvGc::~ArvGc()
{
	if (m_documemt)
	{
		delete m_documemt;
		m_documemt = NULL;
	}
}

IGcNode * ArvGc::arv_gc_get_node(const char *pName)
{
	return NULL;
}

void ArvGc::arv_gc_set_default_node_data(const char *node_name, const char *node_data)
{
	if (arv_gc_get_node(node_name) == NULL)
		return;

}

ICamDevice * ArvGc::arv_gc_get_device()
{
	return m_device;
}
