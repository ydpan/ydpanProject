#ifndef _ArvDomNamedNodeMap_
#define _ArvDomNamedNodeMap_
#include "ArvDomNode.h"
#include "BaseDataDefine.h"
class ArvDomNode;
class ArvDomNamedNodeMap
{
public:
	ArvDomNamedNodeMap();
	~ArvDomNamedNodeMap();

	ArvDomNode *	arv_dom_named_node_map_get_item();
	unsigned int	arv_dom_named_node_map_get_length();
	ArvDomNode *	arv_dom_named_node_map_get_named_item();
	ArvDomNode *	arv_dom_named_node_map_set_named_item();
	ArvDomNode *	arv_dom_named_node_map_remove_named_item();
};
#endif
