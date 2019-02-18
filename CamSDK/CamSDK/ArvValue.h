#pragma once
#include "BaseDataDefine.h"
class ArvValue
{
public:
	ArvValue();
	~ArvValue();

	ArvValue *	arv_value_new_double();
	ArvValue *	arv_value_new_int64();
	void	arv_value_free();
	void	arv_value_copy();
	void	arv_value_set_int64();
	void	arv_value_set_double();
	lp_int64	arv_value_get_int64();
	double	arv_value_get_double();
	bool	arv_value_holds_int64();
	double	arv_value_holds_double();
};

