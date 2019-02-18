#ifndef _ArvDomNode_H_
#define  _ArvDomNode_H_
#include "BaseDataDefine.h"
#include "ArvDomNodeList.h"
#include "ArvDomNamedNodeMap.h"
#include "ArvDomDocument.h"
typedef enum {
	ARV_DOM_NODE_TYPE_ELEMENT_NODE = 1,
	ARV_DOM_NODE_TYPE_ATTRIBUTE_NODE,
	ARV_DOM_NODE_TYPE_TEXT_NODE,
	ARV_DOM_NODE_TYPE_CDATA_SECTION_NODE,
	ARV_DOM_NODE_TYPE_ENTITY_REFERENCE_NODE,
	ARV_DOM_NODE_TYPE_ENTITY_NODE,
	ARV_DOM_NODE_TYPE_PROCESSING_INSTRUCTION_NODE,
	ARV_DOM_NODE_TYPE_COMMENT_NODE,
	ARV_DOM_NODE_TYPE_DOCUMENT_NODE,
	ARV_DOM_NODE_TYPE_DOCUMENT_TYPE_NODE,
	ARV_DOM_NODE_TYPE_DOCUMENT_FRAGMENT_NODE,
	ARV_DOM_NODE_TYPE_NOTATION_NODE
} ArvDomNodeType;

typedef struct {
	//ArvDomNodeList base;

	ArvDomNode *parent_node;
} ArvDomNodeChildList;
class ArvDomDocument;
class ArvDomNodeList;
class ArvDomNode
{
public:
	ArvDomNode();
	~ArvDomNode();

	const char *	arv_dom_node_get_node_name();
	const char *	arv_dom_node_get_node_value();
	void	arv_dom_node_set_node_value(const char* new_value);
	ArvDomNodeType	arv_dom_node_get_node_type();
	ArvDomNode *	arv_dom_node_get_parent_node();
	ArvDomNodeList *	arv_dom_node_get_child_nodes();
	ArvDomNode *	arv_dom_node_get_first_child();
	ArvDomNode *	arv_dom_node_get_last_child();
	ArvDomNode *	arv_dom_node_get_previous_sibling();
	ArvDomNode *	arv_dom_node_get_next_sibling();
	//ArvDomNamedNodeMap *arv_dom_node_get_attributes();
	ArvDomNode *	arv_dom_node_insert_before(ArvDomNode* new_child, ArvDomNode* ref_child);
	ArvDomNode *	arv_dom_node_replace_child(ArvDomNode* new_child, ArvDomNode* old_child);
	ArvDomNode *	arv_dom_node_append_child(ArvDomNode* new_child);
	ArvDomNode *	arv_dom_node_remove_child(ArvDomNode* old_child);
	bool	arv_dom_node_has_child_nodes();
	void	arv_dom_node_changed();
	ArvDomDocument *	arv_dom_node_get_owner_document();
private:
// 	ArvDomNode *arv_dom_node_child_list_get_item(ArvDomNodeList *list, unsigned int index);
// 	unsigned int arv_dom_node_child_list_get_length(ArvDomNodeList *list);
// 	ArvDomNodeList *arv_dom_node_child_list_new(ArvDomNode *parent_node);
public:
	ArvDomNode	*next_sibling;
	ArvDomNode	*previous_sibling;
	ArvDomNode	*parent_node;
	ArvDomNode	*first_child;
	ArvDomNode	*last_child;
};
#endif
