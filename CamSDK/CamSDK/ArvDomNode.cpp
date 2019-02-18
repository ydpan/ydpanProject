#include "ArvDomNode.h"



ArvDomNode::ArvDomNode()
{
}


ArvDomNode::~ArvDomNode()
{
}

const char * ArvDomNode::arv_dom_node_get_node_name()
{
// 	ArvDomNodeClass *node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	g_return_val_if_fail(node_class != NULL, NULL);
// 
// 	if (node_class->get_node_name)
// 		return node_class->get_node_name(self);

	return NULL;
}

const char * ArvDomNode::arv_dom_node_get_node_value()
{
// 	ArvDomNodeClass *node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	g_return_val_if_fail(node_class != NULL, NULL);
// 
// 	if (node_class->get_node_value)
// 		return node_class->get_node_value(self);

	return NULL;
}

void ArvDomNode::arv_dom_node_set_node_value(const char* new_value)
{
// 	ArvDomNodeClass *node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	g_return_if_fail(node_class != NULL);
// 	g_return_if_fail(new_value != NULL);
// 
// 	if (node_class->set_node_value)
// 		node_class->set_node_value(self, new_value);
}

ArvDomNodeType ArvDomNode::arv_dom_node_get_node_type()
{
// 	ArvDomNodeClass *node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	g_return_val_if_fail(node_class != NULL, 0);
// 
// 	if (node_class->get_node_type)
// 		return node_class->get_node_type(self);

	return (ArvDomNodeType)0;
}

ArvDomNode * ArvDomNode::arv_dom_node_get_parent_node()
{
	//g_return_val_if_fail(ARV_IS_DOM_NODE(self), NULL);

	return this->parent_node;
}

ArvDomNodeList * ArvDomNode::arv_dom_node_get_child_nodes()
{
	ArvDomNodeList *list;

// 	g_return_val_if_fail(ARV_IS_DOM_NODE(self), NULL);
// 
// 	list = g_object_get_data(G_OBJECT(self), "child-nodes");
// 
// 	if (list == NULL) {
// 		list = arv_dom_node_child_list_new(self);
// 		g_object_set_data_full(G_OBJECT(self), "child-nodes", list, g_object_unref);
// 	}

	return list;
}

ArvDomNode * ArvDomNode::arv_dom_node_get_first_child()
{
	//g_return_val_if_fail(ARV_IS_DOM_NODE(self), NULL);

	return this->first_child;
}

ArvDomNode * ArvDomNode::arv_dom_node_get_last_child()
{
	return this->last_child;
}

ArvDomNode * ArvDomNode::arv_dom_node_get_previous_sibling()
{
	return this->previous_sibling;
}

ArvDomNode * ArvDomNode::arv_dom_node_get_next_sibling()
{
	return this->next_sibling;
}

ArvDomNode * ArvDomNode::arv_dom_node_insert_before(ArvDomNode* new_child, ArvDomNode* ref_child)
{
// 	ArvDomNodeClass *node_class;
// 
// 	if (ref_child == NULL)
// 		arv_dom_node_append_child(self, new_child);
// 
// 	g_return_val_if_fail(ARV_IS_DOM_NODE(new_child), NULL);
// 
// 	if (new_child->parent_node != NULL)
// 		arv_dom_node_remove_child(self, new_child);
// 
// 	if (!ARV_IS_DOM_NODE(self)) {
// 		g_critical("%s: self is not a ArvDomNode", G_STRFUNC);
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (!ARV_IS_DOM_NODE(ref_child)) {
// 		g_critical("%s: ref_child is not a ArvDomNode", G_STRFUNC);
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (ref_child->parent_node != self) {
// 		arv_debug_dom("[ArvDomNode::insert_before] Ref child '%s' doesn't belong to '%s'",
// 			arv_dom_node_get_node_name(ref_child),
// 			arv_dom_node_get_node_name(self));
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (!ARV_DOM_NODE_GET_CLASS(self)->can_append_child(self, new_child)) {
// 		arv_log_dom("[ArvDomNode::insert_before] Can't append '%s' to '%s'",
// 			arv_dom_node_get_node_name(new_child),
// 			arv_dom_node_get_node_name(self));
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	new_child->parent_node = self;
// 	new_child->next_sibling = ref_child;
// 	new_child->previous_sibling = ref_child->previous_sibling;
// 
// 	if (ref_child->previous_sibling == NULL)
// 		self->first_child = new_child;
// 	else
// 		ref_child->previous_sibling->next_sibling = new_child;
// 
// 	ref_child->previous_sibling = new_child;
// 
// 	node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	if (node_class->post_new_child)
// 		node_class->post_new_child(self, new_child);
// 
// 	arv_dom_node_changed(self);

	return new_child;
}

ArvDomNode * ArvDomNode::arv_dom_node_replace_child(ArvDomNode* new_child, ArvDomNode* old_child)
{
	ArvDomNode *next_sibling;
	ArvDomNode *node;

// 	if (new_child == NULL)
// 		return arv_dom_node_remove_child(self, old_child);
// 
// 	if (!ARV_IS_DOM_NODE(new_child)) {
// 		g_critical("%s: new_child is not a ArvDomNode", G_STRFUNC);
// 		if (ARV_IS_DOM_NODE(old_child))
// 			g_object_unref(old_child);
// 		return NULL;
// 	}
// 
// 	if (new_child->parent_node != NULL)
// 		arv_dom_node_remove_child(self, new_child);
// 
// 	if (old_child == NULL) {
// 		arv_debug_dom("[ArvDomNode::replace_child] old_child == NULL)");
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (!ARV_IS_DOM_NODE(old_child)) {
// 		g_critical("%s: old_child is not a ArvDomNode", G_STRFUNC);
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (!ARV_IS_DOM_NODE(self)) {
// 		g_critical("%s: self is not a ArvDomNode", G_STRFUNC);
// 		g_object_unref(new_child);
// 		g_object_unref(old_child);
// 		return NULL;
// 	}
// 
// 	if (old_child->parent_node != self) {
// 		g_object_unref(new_child);
// 		g_object_unref(old_child);
// 		return NULL;
// 	}
// 
// 	next_sibling = old_child->next_sibling;
// 
// 	node = arv_dom_node_remove_child(self, old_child);
// 	if (node != old_child) {
// 		g_object_unref(new_child);
// 		g_object_unref(old_child);
// 		return NULL;
// 	}
// 
// 	if (next_sibling == NULL)
// 		arv_dom_node_append_child(self, new_child);
// 	else
// 		arv_dom_node_insert_before(self, new_child, next_sibling);

	return old_child;
}

ArvDomNode * ArvDomNode::arv_dom_node_append_child(ArvDomNode* new_child)
{
// 	ArvDomNodeClass *node_class;
// 
// 	if (new_child == NULL)
// 		return NULL;
// 
// 	g_return_val_if_fail(ARV_IS_DOM_NODE(new_child), NULL);
// 
// 	if (!ARV_IS_DOM_NODE(self)) {
// 		g_critical("%s: self is not a ArvDomNode", G_STRFUNC);
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (new_child->parent_node != NULL)
// 		arv_dom_node_remove_child(self, new_child);
// 
// 	if (!ARV_DOM_NODE_GET_CLASS(self)->can_append_child(self, new_child)) {
// 		arv_log_dom("[ArvDomNode::append_child] Can't append '%s' to '%s'",
// 			arv_dom_node_get_node_name(new_child),
// 			arv_dom_node_get_node_name(self));
// 		g_object_unref(new_child);
// 		return NULL;
// 	}
// 
// 	if (self->first_child == NULL)
// 		self->first_child = new_child;
// 	if (self->last_child != NULL)
// 		self->last_child->next_sibling = new_child;
// 
// 	new_child->parent_node = self;
// 	new_child->next_sibling = NULL;
// 	new_child->previous_sibling = self->last_child;
// 	self->last_child = new_child;
// 
// 	node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	if (node_class->post_new_child)
// 		node_class->post_new_child(self, new_child);
// 
// 	arv_dom_node_changed(self);

	return new_child;
}

ArvDomNode * ArvDomNode::arv_dom_node_remove_child(ArvDomNode* old_child)
{
// 	ArvDomNode *node;
// 	ArvDomNodeClass *node_class;
// 
// 	g_return_val_if_fail(ARV_IS_DOM_NODE(self), NULL);
// 
// 	if (old_child == NULL)
// 		return NULL;
// 
// 	g_return_val_if_fail(ARV_IS_DOM_NODE(old_child), NULL);
// 
// 	for (node = self->first_child;
// 		node != NULL && node != old_child;
// 		node = node->next_sibling);
// 
// 	if (node == NULL)
// 		return NULL;
// 
// 	node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	if (node_class->pre_remove_child)
// 		node_class->pre_remove_child(self, old_child);
// 
// 	if (self->first_child == old_child)
// 		self->first_child = old_child->next_sibling;
// 	if (self->last_child == old_child)
// 		self->last_child = old_child->previous_sibling;
// 
// 	if (old_child->next_sibling != NULL)
// 		old_child->next_sibling->previous_sibling = old_child->previous_sibling;
// 	if (old_child->previous_sibling != NULL)
// 		old_child->previous_sibling->next_sibling = old_child->next_sibling;
// 
// 	old_child->parent_node = NULL;
// 	old_child->next_sibling = NULL;
// 	old_child->previous_sibling = NULL;
// 
// 	arv_dom_node_changed();

	return old_child;
}

bool ArvDomNode::arv_dom_node_has_child_nodes()
{
	//g_return_val_if_fail(ARV_IS_DOM_NODE(self), FALSE);

	return this->first_child != NULL;
}

void ArvDomNode::arv_dom_node_changed()
{
// 	ArvDomNode *parent_node;
// 	ArvDomNode *child_node;
// 	ArvDomNodeClass *node_class;
// 
// 	g_return_if_fail(ARV_IS_DOM_NODE(self));
// 
// 	node_class = ARV_DOM_NODE_GET_CLASS(self);
// 
// 	if (node_class->changed)
// 		node_class->changed(self);
// 
// 	child_node = self;
// 	for (parent_node = self->parent_node;
// 		parent_node != NULL;
// 		parent_node = parent_node->parent_node) {
// 		node_class = ARV_DOM_NODE_GET_CLASS(parent_node);
// 		if (node_class->child_changed == NULL ||
// 			!node_class->child_changed(parent_node, child_node))
// 			break;
// 		child_node = parent_node;
// 	}
}

ArvDomDocument * ArvDomNode::arv_dom_node_get_owner_document()
{
	ArvDomNode *parent;

// 	g_return_val_if_fail(ARV_IS_DOM_NODE(self), NULL);
// 
// 	for (parent = self;
// 		parent != NULL &&
// 		!ARV_IS_DOM_DOCUMENT(parent);
// 		parent = parent->parent_node);

	return (ArvDomDocument*)parent;
}

// ArvDomNode * ArvDomNode::arv_dom_node_child_list_get_item(ArvDomNodeList *list, unsigned int index)
// {
// 	ArvDomNodeChildList *child_list;// = ARV_DOM_NODE_CHILD_LIST(list);
// 	ArvDomNode *iter;
// 	unsigned int i = 0;
// 
// 	if (child_list->parent_node == NULL)
// 		return NULL;
// 
// 	for (iter = child_list->parent_node->first_child; iter != NULL; iter = iter->next_sibling) {
// 		if (i == index)
// 			return iter;
// 		i++;
// 	}
// 
// 	return NULL;
// }

// unsigned int ArvDomNode::arv_dom_node_child_list_get_length(ArvDomNodeList *list)
// {
// 	ArvDomNodeChildList *child_list;// = ARV_DOM_NODE_CHILD_LIST(list);
// 	ArvDomNode *iter;
// 	unsigned int length = 0;
// 
// 	if (child_list->parent_node == NULL)
// 		return 0;
// 
// 	for (iter = child_list->parent_node->first_child; iter != NULL; iter = iter->next_sibling)
// 		length++;
// 
// 	return length;
// }

// ArvDomNodeList * ArvDomNode::arv_dom_node_child_list_new(ArvDomNode *parent_node)
// {
// 	ArvDomNodeChildList *list;
// 
// 	//g_return_val_if_fail(ARV_IS_DOM_NODE(parent_node), NULL);
// 
// 	//list = g_object_new(ARV_TYPE_DOM_NODE_CHILD_LIST, NULL);
// 	list->parent_node = parent_node;
// 
// 	//g_object_weak_ref(G_OBJECT(parent_node), arv_dom_node_child_list_weak_notify_cb, list);
// 
// 	return (ArvDomNodeList*)(list);
// }
