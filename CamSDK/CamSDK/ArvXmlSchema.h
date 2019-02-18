#pragma once
class ArvXmlSchema
{
public:
	ArvXmlSchema();
	~ArvXmlSchema();

	ArvXmlSchema *	arv_xml_schema_new_from_file();
	ArvXmlSchema *	arv_xml_schema_new_from_path();
	bool	arv_xml_schema_validate();
};

