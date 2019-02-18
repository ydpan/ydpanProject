#pragma once
#include "BaseDataDefine.h"
#include "ArvDomNode.h"
class ArvDomNode;
//typedef ArvDomNodeType;
class ArvDomElement
{
public:
	ArvDomElement();
	~ArvDomElement();

	const char *	arv_dom_element_get_tag_name();
	const char *	arv_dom_element_get_attribute(const char* name);
	void	arv_dom_element_set_attribute(const char* name, const char* attribute_value);
private:
	const char *arv_dom_element_get_node_value(ArvDomNode *node)
	{
		return NULL;
	}
// 	ArvDomNodeType arv_dom_element_get_node_type(ArvDomNode *node)
// 	{
// 		return ARV_DOM_NODE_TYPE_ELEMENT_NODE;
// 	}
};

