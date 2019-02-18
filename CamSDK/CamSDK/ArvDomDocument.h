#pragma once
#include "ArvDomElement.h"
#include "ArvDomText.h"
#include "ArvDomNode.h"
#include "ICamDevice.h"
class ArvDomElement;
//class ArvDomNode;
class ArvDomDocument:public IDomDocument
{
public:
	ArvDomDocument();
	~ArvDomDocument();
	virtual void Init(const void *buffer, int size);

	ArvDomDocument *	(*ArvDomDocumentCreateFunction) ();
	ArvDomDocument *	arv_dom_implementation_create_document();
	void	arv_dom_implementation_add_document_type();
	void	arv_dom_implementation_cleanup();
	void	arv_dom_document_append_from_memory();
	ArvDomDocument *	arv_dom_document_new_from_memory();
	ArvDomDocument *	arv_dom_document_new_from_path();
	ArvDomDocument *	arv_dom_document_new_from_url();
	void	arv_dom_document_save_to_stream();
	void	arv_dom_document_save_to_memory();
	void	arv_dom_document_save_to_path();
	void	arv_dom_document_save_to_url();
	ArvDomElement *	arv_dom_document_get_document_element();
	ArvDomElement *	arv_dom_document_create_element();
	ArvDomText *	arv_dom_document_create_text_node();
	const char *	arv_dom_document_get_url();
	void	arv_dom_document_set_url();
	void	arv_dom_document_set_path();
	void *	arv_dom_document_get_href_data();

public:
	//ArvDomNode node;
	char *url;
};

class ArvDomDocumentFragment {
	ArvDomDocumentFragment *	arv_dom_document_fragment_new();
};
