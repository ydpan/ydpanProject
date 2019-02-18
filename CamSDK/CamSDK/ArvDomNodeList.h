#pragma once
#include "ArvDomNode.h"
class ArvDomNode;
class ArvDomNodeList
{
public:
	ArvDomNodeList();
	~ArvDomNodeList();

	ArvDomNode *	arv_dom_node_list_get_item();
	unsigned int	arv_dom_node_list_get_length();
	//GType	arv_dom_node_list_get_type();
};

