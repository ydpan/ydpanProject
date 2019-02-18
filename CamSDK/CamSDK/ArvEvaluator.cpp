#include "ArvEvaluator.h"



ArvEvaluator::ArvEvaluator()
{
}


ArvEvaluator::~ArvEvaluator()
{
}

void ArvEvaluator::init()
{
// 	evaluator->priv = G_TYPE_INSTANCE_GET_PRIVATE(evaluator, ARV_TYPE_EVALUATOR, ArvEvaluatorPrivate);
// 
// 	evaluator->priv->expression = NULL;
// 	evaluator->priv->rpn_stack = NULL;
// 	evaluator->priv->variables = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)arv_value_free);
// 	evaluator->priv->sub_expressions = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
// 	evaluator->priv->constants = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
// 
// 	arv_evaluator_set_double_variable(evaluator, "PI", M_PI);
// 	arv_evaluator_set_double_variable(evaluator, "E", M_E);
}

ArvEvaluator * ArvEvaluator::arv_evaluator_new(const char *pexpression)
{
	ArvEvaluator *evaluator;

	//evaluator = g_object_new(ARV_TYPE_EVALUATOR, NULL);

	arv_evaluator_set_expression(expression);

	return evaluator;
}

void ArvEvaluator::arv_evaluator_set_expression(const char *pexpression)
{
	//g_return_if_fail(ARV_IS_EVALUATOR(evaluator));

	if (strcmp(pexpression, expression) == 0)
		return;

	free(expression);
	expression = NULL;

	if (expression == NULL) {
		parsing_status = ARV_EVALUATOR_STATUS_EMPTY_EXPRESSION;
		return;
	}

	parsing_status = ARV_EVALUATOR_STATUS_NOT_PARSED;
	//expression = g_strdup(expression);
}

const char * ArvEvaluator::arv_evaluator_get_expression()
{
	return expression;
}

const char * ArvEvaluator::arv_evaluator_get_constant(const char *name)
{
// 	g_return_val_if_fail(ARV_IS_EVALUATOR(evaluator), NULL);
// 
// 	if (name == NULL)
		return NULL;

//	return g_hash_table_lookup(evaluator->priv->constants, name);
}

const char * ArvEvaluator::arv_evaluator_get_sub_expression(const char *name)
{
	//g_return_val_if_fail(ARV_IS_EVALUATOR(evaluator), NULL);

	//if (name == NULL)
		return NULL;

	//return g_hash_table_lookup(evaluator->priv->sub_expressions, name);
}

void ArvEvaluator::arv_evaluator_set_constant(const char *name, const char *constant)
{
// 	const char *old_constant;
// 
// 	g_return_if_fail(ARV_IS_EVALUATOR(evaluator));
// 
// 	if (name == NULL)
// 		return;
// 
// 	old_constant = g_hash_table_lookup(evaluator->priv->constants, name);
// 	if (old_constant != NULL && g_strcmp0(old_constant, constant) == 0)
// 		return;
// 
// 	if (constant != NULL)
// 		g_hash_table_replace(evaluator->priv->constants, g_strdup(name), g_strdup(constant));
// 	else
// 		g_hash_table_remove(evaluator->priv->constants, name);
// 
// 	evaluator->priv->parsing_status = ARV_EVALUATOR_STATUS_NOT_PARSED;
// 
// 	arv_log_evaluator("[Evaluator::set_constant] %s = %s", name, constant);
}

void ArvEvaluator::arv_evaluator_set_sub_expression(const char *name, const char *expression)
{
// 	const char *old_expression;
// 
// 	g_return_if_fail(ARV_IS_EVALUATOR(evaluator));
// 
// 	if (name == NULL)
// 		return;
// 
// 	old_expression = g_hash_table_lookup(evaluator->priv->sub_expressions, name);
// 	if (old_expression != NULL && g_strcmp0(old_expression, expression) == 0)
// 		return;
// 
// 	if (expression != NULL)
// 		g_hash_table_replace(evaluator->priv->sub_expressions, g_strdup(name), g_strdup(expression));
// 	else
// 		g_hash_table_remove(evaluator->priv->sub_expressions, name);
// 
// 	evaluator->priv->parsing_status = ARV_EVALUATOR_STATUS_NOT_PARSED;
// 
// 	arv_log_evaluator("[Evaluator::set_sub_expression] %s = %s", name, expression);
}

double ArvEvaluator::arv_evaluator_evaluate_as_double()
{
	ArvEvaluatorStatus status;
	double value;

// 	g_return_val_if_fail(ARV_IS_EVALUATOR(evaluator), 0.0);
// 
// 	arv_log_evaluator("[Evaluator::evaluate_as_double] Expression = '%s'",
// 		evaluator->priv->expression);
// 
// 	if (evaluator->priv->parsing_status == ARV_EVALUATOR_STATUS_NOT_PARSED) {
// 		evaluator->priv->parsing_status = parse_expression(evaluator);
// 		arv_log_evaluator("[Evaluator::evaluate_as_double] Parsing status = %d",
// 			evaluator->priv->parsing_status);
// 	}
// 
// 	if (evaluator->priv->parsing_status != ARV_EVALUATOR_STATUS_SUCCESS) {
// 		arv_evaluator_set_error(error, evaluator->priv->parsing_status);
// 		return 0.0;
// 	}
// 
// 	status = evaluate(evaluator->priv->rpn_stack, evaluator->priv->variables, NULL, &value);
// 
// 	if (status != ARV_EVALUATOR_STATUS_SUCCESS) {
// 		arv_evaluator_set_error(error, status);
// 		return 0.0;
// 	}

	return value;
}

lp_int64 ArvEvaluator::arv_evaluator_evaluate_as_int64()
{
	ArvEvaluatorStatus status;
 	lp_int64 value;
// 
// 	g_return_val_if_fail(ARV_IS_EVALUATOR(evaluator), 0.0);
// 
// 	arv_log_evaluator("[Evaluator::evaluate_as_int64] Expression = '%s'",
// 		evaluator->priv->expression);
// 
// 	if (evaluator->priv->parsing_status == ARV_EVALUATOR_STATUS_NOT_PARSED) {
// 		evaluator->priv->parsing_status = parse_expression(evaluator);
// 		arv_log_evaluator("[Evaluator::evaluate_as_int64] Parsing status = %d",
// 			evaluator->priv->parsing_status);
// 	}
// 
// 	if (evaluator->priv->parsing_status != ARV_EVALUATOR_STATUS_SUCCESS) {
// 		arv_evaluator_set_error(error, evaluator->priv->parsing_status);
// 		return 0.0;
// 	}
// 
// 	status = evaluate(evaluator->priv->rpn_stack, evaluator->priv->variables, &value, NULL);
// 
// 	if (status != ARV_EVALUATOR_STATUS_SUCCESS) {
// 
// 		arv_evaluator_set_error(error, status);
// 		return 0.0;
// 	}

	return value;
}

void ArvEvaluator::arv_evaluator_set_double_variable(const char *name, double v_double)
{
// 	ArvValue *old_value;
// 
// 	g_return_if_fail(ARV_IS_EVALUATOR(evaluator));
// 	g_return_if_fail(name != NULL);
// 
// 	old_value = g_hash_table_lookup(evaluator->priv->variables, name);
// 	if (old_value != NULL && (arv_value_get_double(old_value) == v_double))
// 		return;
// 
// 	g_hash_table_insert(evaluator->priv->variables,
// 		g_strdup(name),
// 		arv_value_new_double(v_double));
// 
// 	arv_log_evaluator("[Evaluator::set_double_variable] %s = %g",
// 		name, v_double);
}

void ArvEvaluator::arv_evaluator_set_int64_variable(const char *name, lp_int64 v_int64)
{
// 	ArvValue *old_value;
// 
// 	g_return_if_fail(ARV_IS_EVALUATOR(evaluator));
// 	g_return_if_fail(name != NULL);
// 
// 	old_value = g_hash_table_lookup(evaluator->priv->variables, name);
// 	if (old_value != NULL && (arv_value_get_int64(old_value) == v_int64))
// 		return;
// 
// 	g_hash_table_insert(evaluator->priv->variables,
// 		g_strdup(name),
// 		arv_value_new_int64(v_int64));
// 
// 	arv_log_evaluator("[Evaluator::set_int64_variable] %s = %Ld", name, v_int64);
}
