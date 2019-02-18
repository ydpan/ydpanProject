#pragma once
#include "BaseDataDefine.h"
class ArvGcNode;
class ArvGcPropertyNode
{
public:
	ArvGcPropertyNode();
	~ArvGcPropertyNode();

	ArvGcNode *	arv_gc_property_node_new_access_mode();
	ArvGcNode *	arv_gc_property_node_new_address();
	ArvGcNode *	arv_gc_property_node_new_bit();
	ArvGcNode *	arv_gc_property_node_new_cachable();
	ArvGcNode *	arv_gc_property_node_new_command_value();
	ArvGcNode *	arv_gc_property_node_new_constant();
	ArvGcNode *	arv_gc_property_node_new_description();
	ArvGcNode *	arv_gc_property_node_new_display_name();
	ArvGcNode *	arv_gc_property_node_new_endianess();
	ArvGcNode *	arv_gc_property_node_new_expression();
	ArvGcNode *	arv_gc_property_node_new_formula();
	ArvGcNode *	arv_gc_property_node_new_formula_from();
	ArvGcNode *	arv_gc_property_node_new_formula_to();
	ArvGcNode *	arv_gc_property_node_new_increment();
	ArvGcNode *	arv_gc_property_node_new_length();
	ArvGcNode *	arv_gc_property_node_new_lsb();
	ArvGcNode *	arv_gc_property_node_new_maximum();
	ArvGcNode *	arv_gc_property_node_new_minimum();
	ArvGcNode *	arv_gc_property_node_new_msb();
	ArvGcNode *	arv_gc_property_node_new_off_value();
	ArvGcNode *	arv_gc_property_node_new_on_value();
	ArvGcNode *	arv_gc_property_node_new_chunk_id();
	ArvGcNode *	arv_gc_property_node_new_p_address();
	ArvGcNode *	arv_gc_property_node_new_p_command_value();
	ArvGcNode *	arv_gc_property_node_new_p_feature();
	ArvGcNode *	arv_gc_property_node_new_p_increment();
	ArvGcNode *	arv_gc_property_node_new_p_is_available();
	ArvGcNode *	arv_gc_property_node_new_p_is_implemented();
	ArvGcNode *	arv_gc_property_node_new_p_is_locked();
	ArvGcNode *	arv_gc_property_node_new_p_length();
	ArvGcNode *	arv_gc_property_node_new_p_maximum();
	ArvGcNode *	arv_gc_property_node_new_p_minimum();
	ArvGcNode *	arv_gc_property_node_new_p_port();
	ArvGcNode *	arv_gc_property_node_new_p_value();
	ArvGcNode *	arv_gc_property_node_new_p_variable();
	ArvGcNode *	arv_gc_property_node_new_polling_time();
	ArvGcNode *	arv_gc_property_node_new_sign();
	ArvGcNode *	arv_gc_property_node_new_tooltip();
	ArvGcNode *	arv_gc_property_node_new_unit();
	ArvGcNode *	arv_gc_property_node_new_p_value_default();
	ArvGcNode *	arv_gc_property_node_new_value_default();
	ArvGcNode *	arv_gc_property_node_new_value();
	const char *	arv_gc_property_node_get_name();
	//ArvGcPropertyNodeType	arv_gc_property_node_get_node_type();
	ArvGcNode *	arv_gc_property_node_get_linked_node();
	double	arv_gc_property_node_get_double();
	lp_int64	arv_gc_property_node_get_int64();
	const char *	arv_gc_property_node_get_string();
	void	arv_gc_property_node_set_double();
	void	arv_gc_property_node_set_int64();
	void	arv_gc_property_node_set_string();
};

class ArvGcIndexNode {
	ArvGcNode *	arv_gc_index_node_new();
	lp_int64	arv_gc_index_node_get_index();
};

class ArvGcValueIndexedNode
{
	ArvGcNode *	arv_gc_p_value_indexed_node_new();
	lp_int64	arv_gc_value_indexed_node_get_index();
	//GType	arv_gc_value_indexed_node_get_type();
	ArvGcNode *	arv_gc_value_indexed_node_new();
};

class ArvGcInvalidatorNode
{
	ArvGcNode *	arv_gc_invalidator_node_new();
	lp_int32	arv_gc_invalidator_node_get_modification_count();
	void	arv_gc_invalidator_node_set_modification_count();
};

class ArvGcFeatureNode {
	ArvGcFeatureNode *	arv_gc_feature_node_new();
	//GType	arv_gc_feature_node_get_value_type();
	const char *	arv_gc_feature_node_get_value_as_string();
	void	arv_gc_feature_node_set_value_from_string();
	const char *	arv_gc_feature_node_get_name();
	const char *	arv_gc_feature_node_get_display_name();
	const char *	arv_gc_feature_node_get_tooltip();
	const char *	arv_gc_feature_node_get_description();
	bool	arv_gc_feature_node_is_available();
	void	arv_gc_feature_node_inc_modification_count();
	lp_int32	arv_gc_feature_node_get_modification_count();
	bool	arv_gc_feature_node_is_implemented();
	bool	arv_gc_feature_node_is_locked();
};

class ArvGcRegisterDescriptionNode {
	ArvGcNode *	arv_gc_register_description_node_new();
	int	arv_gc_register_description_node_compare_schema_version();
	bool	arv_gc_register_description_node_check_schema_version();
	void	arv_gc_register_feature_node();
};

class ArvGcGroupNode {
	//GType	arv_gc_group_node_get_type();
	ArvGcNode *	arv_gc_group_node_new();
};

class ArvGcRegister {
	void	arv_gc_register_get();
	lp_uint64	arv_gc_register_get_address();
	lp_uint64	arv_gc_register_get_length();
	//GType	arv_gc_register_get_type();
	void	arv_gc_register_set();
};

class ArvGcInteger {
	lp_int64	arv_gc_integer_get_value();
	void	arv_gc_integer_set_value();
	lp_int64	arv_gc_integer_get_min();
	lp_int64	arv_gc_integer_get_max();
	lp_int64	arv_gc_integer_get_inc();
	const char *	arv_gc_integer_get_unit();
	void	arv_gc_integer_impose_min();
	void	arv_gc_integer_impose_max();
};
class ArvGcFloat {
	double	arv_gc_float_get_value();
	void	arv_gc_float_set_value();
	double	arv_gc_float_get_min();
	double	arv_gc_float_get_max();
	double	arv_gc_float_get_inc();
	const char *	arv_gc_float_get_unit();
	void	arv_gc_float_impose_min();
	void	arv_gc_float_impose_max();
};

class ArvGcString {
	const char *	arv_gc_string_get_value();
	void	arv_gc_string_set_value();
	lp_int64	arv_gc_string_get_max_length();
};

class ArvGcCategory
{
	ArvGcNode *	arv_gc_category_new();
	//const GSList *	arv_gc_category_get_features();
};

class ArvGcCommand {
	ArvGcNode *	arv_gc_command_new();
	void	arv_gc_command_execute();
};

class ArvGcBoolean {
	ArvGcNode *	arv_gc_boolean_new();
	bool	arv_gc_boolean_get_value();
	void	arv_gc_boolean_set_value();
};

class ArvGcIntegerNode
{
	ArvGcNode *	arv_gc_integer_node_new();
};
class ArvGcFloatNode
{
	ArvGcNode *	arv_gc_float_node_new();
};

enum ArvGcSign
{
	ARV_GC_SIGN_SIGNED,

	//signed integer

	ARV_GC_SIGN_UNSIGNED,

	//unsigned integer
};
enum ArvGcRegisterNodeType {
ARV_GC_REGISTER_NODE_TYPE_REGISTER,

	//Register node

	ARV_GC_REGISTER_NODE_TYPE_INTEGER,

	//IntReg node

	ARV_GC_REGISTER_NODE_TYPE_MASKED_INTEGER,

	//MaskedIntReg node

	ARV_GC_REGISTER_NODE_TYPE_FLOAT,

	//FloatReg node

	ARV_GC_REGISTER_NODE_TYPE_STRING,

	//StringReg node

	ARV_GC_REGISTER_NODE_TYPE_STRUCT_REGISTER,

	//StructReg node
};

	
class ArvGcRegisterNode
{
	ArvGcNode *	arv_gc_register_node_new();
	ArvGcNode *	arv_gc_register_node_new_integer();
	ArvGcNode *	arv_gc_register_node_new_masked_integer();
	ArvGcNode *	arv_gc_register_node_new_float();
	ArvGcNode *	arv_gc_register_node_new_string();
	ArvGcNode *	arv_gc_register_node_new_struct_register();
	lp_int64	arv_gc_register_node_get_masked_integer_value();
	void	arv_gc_register_node_set_masked_integer_value();
};

class ArvGcStructEntryNode {
	ArvGcNode *	arv_gc_struct_entry_node_new();
};
class ArvGcEnumeration {
	ArvGcNode *	arv_gc_enumeration_new();
	const char *	arv_gc_enumeration_get_string_value();
	void	arv_gc_enumeration_set_string_value();
	lp_int64	arv_gc_enumeration_get_int_value();
	void	arv_gc_enumeration_set_int_value();
	//const GSList *	arv_gc_enumeration_get_entries();
	lp_int64 *	arv_gc_enumeration_get_available_int_values();
	const char **	arv_gc_enumeration_get_available_string_values();
};
class ArvGcEnumEntry {
	ArvGcNode *	arv_gc_enum_entry_new();
	lp_int64	arv_gc_enum_entry_get_value();
};
class ArvGcConverter
{
	ArvGcNode *	arv_gc_converter_new();
	ArvGcNode *	arv_gc_converter_new_integer();
};
class ArvGcSwissKnife
{
	ArvGcNode *	arv_gc_swiss_knife_new();
	ArvGcNode *	arv_gc_swiss_knife_new_integer();
};
class ArvGcPort
{
	ArvGcNode *	arv_gc_port_new();
	void	arv_gc_port_read();
	void	arv_gc_port_write();
	void	arv_gc_set_default_node_data();
};