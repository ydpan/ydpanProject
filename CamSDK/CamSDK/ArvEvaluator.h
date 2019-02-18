#pragma once
#include "BaseDataDefine.h"
//#include <hash_map>
#include <string>
typedef enum {
	ARV_EVALUATOR_STATUS_SUCCESS,
	ARV_EVALUATOR_STATUS_NOT_PARSED,
	ARV_EVALUATOR_STATUS_EMPTY_EXPRESSION,
	ARV_EVALUATOR_STATUS_PARENTHESES_MISMATCH,
	ARV_EVALUATOR_STATUS_SYNTAX_ERROR,
	ARV_EVALUATOR_STATUS_UNKNOWN_OPERATOR,
	ARV_EVALUATOR_STATUS_UNKNOWN_VARIABLE,
	ARV_EVALUATOR_STATUS_UNKNOWN_SUB_EXPRESSION,
	ARV_EVALUATOR_STATUS_UNKNOWN_CONSTANT,
	ARV_EVALUATOR_STATUS_MISSING_ARGUMENTS,
	ARV_EVALUATOR_STATUS_REMAINING_OPERANDS,
	ARV_EVALUATOR_STATUS_DIVISION_BY_ZERO,
	ARV_EVALUATOR_STATUS_STACK_OVERFLOW,
	ARV_EVALUATOR_STATUS_INVALID_DOUBLE_FUNCTION,
	ARV_EVALUATOR_STATUS_FORBIDDEN_RECUSRION
} ArvEvaluatorStatus;

static const char *arv_evaluator_status_strings[] = {
	"success",
	"not parsed",
	"empty expression",
	"parentheses mismatch",
	"syntax error",
	"unknown operator",
	"unknown variable",
	"unknown subexpression",
	"unknown constant",
	"missing arguments",
	"remaining operands",
	"division by zero",
	"stack overflow",
	"invalid double function",
	"forbidden recursion"
};
typedef enum {
	ARV_EVALUATOR_TOKEN_UNKNOWN,
	ARV_EVALUATOR_TOKEN_COMMA,
	ARV_EVALUATOR_TOKEN_TERNARY_QUESTION_MARK,
	ARV_EVALUATOR_TOKEN_TERNARY_COLON,
	ARV_EVALUATOR_TOKEN_LOGICAL_OR,
	ARV_EVALUATOR_TOKEN_LOGICAL_AND,
	ARV_EVALUATOR_TOKEN_BITWISE_OR,
	ARV_EVALUATOR_TOKEN_BITWISE_XOR,
	ARV_EVALUATOR_TOKEN_BITWISE_AND,
	ARV_EVALUATOR_TOKEN_EQUAL,
	ARV_EVALUATOR_TOKEN_NOT_EQUAL,
	ARV_EVALUATOR_TOKEN_LESS_OR_EQUAL,
	ARV_EVALUATOR_TOKEN_GREATER_OR_EQUAL,
	ARV_EVALUATOR_TOKEN_LESS,
	ARV_EVALUATOR_TOKEN_GREATER,
	ARV_EVALUATOR_TOKEN_SHIFT_RIGHT,
	ARV_EVALUATOR_TOKEN_SHIFT_LEFT,
	ARV_EVALUATOR_TOKEN_SUBSTRACTION,
	ARV_EVALUATOR_TOKEN_ADDITION,
	ARV_EVALUATOR_TOKEN_REMAINDER,
	ARV_EVALUATOR_TOKEN_DIVISION,
	ARV_EVALUATOR_TOKEN_MULTIPLICATION,
	ARV_EVALUATOR_TOKEN_POWER,
	ARV_EVALUATOR_TOKEN_MINUS,
	ARV_EVALUATOR_TOKEN_PLUS,
	ARV_EVALUATOR_TOKEN_BITWISE_NOT,
	ARV_EVALUATOR_TOKEN_FUNCTION_SIN,
	ARV_EVALUATOR_TOKEN_FUNCTION_COS,
	ARV_EVALUATOR_TOKEN_FUNCTION_SGN,
	ARV_EVALUATOR_TOKEN_FUNCTION_NEG,
	ARV_EVALUATOR_TOKEN_FUNCTION_ATAN,
	ARV_EVALUATOR_TOKEN_FUNCTION_TAN,
	ARV_EVALUATOR_TOKEN_FUNCTION_ABS,
	ARV_EVALUATOR_TOKEN_FUNCTION_EXP,
	ARV_EVALUATOR_TOKEN_FUNCTION_LN,
	ARV_EVALUATOR_TOKEN_FUNCTION_LG,
	ARV_EVALUATOR_TOKEN_FUNCTION_SQRT,
	ARV_EVALUATOR_TOKEN_FUNCTION_TRUNC,
	ARV_EVALUATOR_TOKEN_FUNCTION_ROUND,
	ARV_EVALUATOR_TOKEN_FUNCTION_FLOOR,
	ARV_EVALUATOR_TOKEN_FUNCTION_CEIL,
	ARV_EVALUATOR_TOKEN_FUNCTION_ASIN,
	ARV_EVALUATOR_TOKEN_FUNCTION_ACOS,
	ARV_EVALUATOR_TOKEN_RIGHT_PARENTHESIS,
	ARV_EVALUATOR_TOKEN_LEFT_PARENTHESIS,
	ARV_EVALUATOR_TOKEN_CONSTANT_INT64,
	ARV_EVALUATOR_TOKEN_CONSTANT_DOUBLE,
	ARV_EVALUATOR_TOKEN_VARIABLE
} ArvEvaluatorTokenId;
typedef enum {

	ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT,
	ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT
} ArvEvaluatorTokenAssociativity;

typedef struct {
	const char *		tag;
	int			precedence;
	bool		double_only;
	int			n_args;
	ArvEvaluatorTokenAssociativity	associativity;
} ArvEvaluatorTokenInfos;
static ArvEvaluatorTokenInfos arv_evaluator_token_infos[] = {
	{"",		0,	false,	1, (ArvEvaluatorTokenAssociativity)0}, /* UNKNOWN */
	{",",		0, 	false,	0, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* COMMA */
	{"?",		5,	false,	3, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT}, /* TERNARY_QUESTION_MARK */
	{":",		5,	false,	1, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT}, /* TERNARY_COLON */
	{"||",		10,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* LOGICAL_OR */
	{"&&",		20,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* LOGICAL_AND */
	{"|",		40,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* BITWISE_OR */
	{"^",		50,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* BITWISE_XOR */
	{"&",		60,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* BITWISE_AND */
	{"=",		70,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* EQUAL, */
	{"<>",		70,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* NOT_EQUAL */
	{"<=",		80,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* LESS_OR_EQUAL */
	{">=",		80,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* GREATER_OR_EQUAL */
	{"<",		80,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* LESS */
	{">",		80,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* GREATER */
	{">>",		90,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* SHIFT_RIGHT */
	{"<<",		90,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* SHIFT_LEFT */
	{"-",		100, 	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* SUBSTRACTION */
	{"+",		100, 	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* ADDITION */
	{"%",		110,	false,	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* REMAINDER */
	{"/",		110,	false, 	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* DIVISION */
	{"*",		110, 	false, 	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT}, /* MULTIPLICATION */
	{"**",		120,	false, 	2, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT}, /* POWER */
	{"minus",	130, 	false, 	1, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT}, /* MINUS */
	{"plus",	130, 	false, 	1, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT}, /* PLUS */
	{"~",		130,	false,	1, ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT}, /* BITWISE_NOT */
	{"sin",		200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_SIN */
	{"cos",		200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_COS */
	{"sgn",		200,	false,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_SGN */
	{"neg",		200,	false,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_NEG */
	{"atan",	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_ATAN */
	{"tan" ,	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_TAN */
	{"abs" ,	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_ABS */
	{"exp" ,	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_EXP */
	{"ln",  	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_LN */
	{"lg",  	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_LG */
	{"sqrt",	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_SQRT */
	{"trunc",	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_TRUNC */
	{"round",	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_ROUND */
	{"floor",	200,	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_FLOOR */
	{"ceil",	200, 	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_CEIL */
	{"asin",	200, 	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_ASIN */
	{"acos",	200, 	true,	1, (ArvEvaluatorTokenAssociativity)0}, /* FUNCTION_ACOS */
	{")",		990, 	false,	0, (ArvEvaluatorTokenAssociativity)0}, /* RIGHT_PARENTHESIS */
	{"(",		-1, 	false,	0, (ArvEvaluatorTokenAssociativity)0}, /* LEFT_PARENTHESIS */
	{"int64",	200,	false,	0, (ArvEvaluatorTokenAssociativity)0}, /* CONSTANT_INT64 */
	{"double",	200,	false,	0, (ArvEvaluatorTokenAssociativity)0}, /* CONSTANT_DOUBLE */
	{"var",		200,	false,	0, (ArvEvaluatorTokenAssociativity)0}, /* VARIABLE */
};
typedef struct {
	ArvEvaluatorTokenId	token_id;
	lp_int32 parenthesis_level;
	union {
		double		v_double;
		lp_int64		v_int64;
		char * 		name;
	} data;
} ArvEvaluatorToken;
struct ArvValue {
	//GType type;
	union {
		lp_int64 v_int64;
		double v_double;
	} data;
};
typedef struct {
	lp_int32 parenthesis_level;
	ArvValue value;
} ArvEvaluatorValuesStackItem;

typedef struct {
	int count;
// 	GSList *token_stack;
// 	GSList *operator_stack;
// 	GSList *garbage_stack;
	bool in_sub_expression;
	bool previous_token_was_operand;
	bool previous_token_was_right_parenthesis;
} ArvEvaluatorParserState;

class ArvEvaluator
{
public:
	ArvEvaluator();
	~ArvEvaluator();
	void init();
	ArvEvaluator *	arv_evaluator_new(const char *name);
	void	arv_evaluator_set_expression(const char *expression);
	const char *	arv_evaluator_get_expression();
	const char *	arv_evaluator_get_constant(const char *name);
	const char *	arv_evaluator_get_sub_expression(const char *name);
	void	arv_evaluator_set_constant(const char *name, const char *constant);
	void	arv_evaluator_set_sub_expression(const char *name,const char *expression);
	double	arv_evaluator_evaluate_as_double();
	lp_int64	arv_evaluator_evaluate_as_int64();
	void	arv_evaluator_set_double_variable(const char *name, double v_double);
	void	arv_evaluator_set_int64_variable(const char *name, lp_int64 v_int64);
private:
	static ArvEvaluatorToken *arv_evaluator_token_new(ArvEvaluatorTokenId token_id)
	{
		ArvEvaluatorToken *token = new ArvEvaluatorToken;
		token->token_id = token_id;
		return token;
	}
	static ArvEvaluatorToken *arv_evaluator_token_new_double(double v_double)
	{
		ArvEvaluatorToken *token = arv_evaluator_token_new(ARV_EVALUATOR_TOKEN_CONSTANT_DOUBLE);
		token->data.v_double = v_double;
		return token;
	}
	static ArvEvaluatorToken *arv_evaluator_token_new_int64(double v_int64)
	{
		ArvEvaluatorToken *token = arv_evaluator_token_new(ARV_EVALUATOR_TOKEN_CONSTANT_INT64);
		token->data.v_int64 = v_int64;
		return token;
	}
	static ArvEvaluatorToken *
		arv_evaluator_token_new_variable(const char *name)
	{
		ArvEvaluatorToken *token = arv_evaluator_token_new(ARV_EVALUATOR_TOKEN_VARIABLE);
		//token->data.name = memcpy(name);

		return token;
	}
	static void arv_evaluator_token_free(ArvEvaluatorToken *token)
	{
		if (token == NULL)
			return;

		if (token->token_id == ARV_EVALUATOR_TOKEN_VARIABLE)
			free(token->data.name);
		free(token);
	}
	static bool arv_evaluator_token_is_variable(ArvEvaluatorToken *token)
	{
		return (token != NULL &&
			token->token_id == ARV_EVALUATOR_TOKEN_VARIABLE);
	}
	static bool arv_evaluator_token_is_operand(ArvEvaluatorToken *token)
	{
		return (token != NULL &&
			token->token_id > ARV_EVALUATOR_TOKEN_LEFT_PARENTHESIS);
	}
	static bool arv_evaluator_token_is_operator(ArvEvaluatorToken *token)
	{
		return (token != NULL &&
			token->token_id > ARV_EVALUATOR_TOKEN_UNKNOWN &&
			token->token_id < ARV_EVALUATOR_TOKEN_RIGHT_PARENTHESIS);
	}
	static bool arv_evaluator_token_is_comma(ArvEvaluatorToken *token)
	{
		return (token != NULL &&
			token->token_id == ARV_EVALUATOR_TOKEN_COMMA);
	}
	static bool arv_evaluator_token_is_left_parenthesis(ArvEvaluatorToken *token)
	{
		return (token != NULL &&
			token->token_id == ARV_EVALUATOR_TOKEN_LEFT_PARENTHESIS);
	}
	static bool arv_evaluator_token_is_right_parenthesis(ArvEvaluatorToken *token)
	{
		return (token != NULL &&
			token->token_id == ARV_EVALUATOR_TOKEN_RIGHT_PARENTHESIS);
	}
	static bool arv_evaluator_token_compare_precedence(ArvEvaluatorToken *a, ArvEvaluatorToken *b)
	{
		int a_precedence;
		int b_precedence;
		ArvEvaluatorTokenAssociativity a_associativity;

// 		if (a == NULL || b == NULL ||
// 			a->token_id >= G_N_ELEMENTS(arv_evaluator_token_infos) ||
// 			b->token_id >= G_N_ELEMENTS(arv_evaluator_token_infos))
// 			return false;

		a_precedence = arv_evaluator_token_infos[a->token_id].precedence;
		b_precedence = arv_evaluator_token_infos[b->token_id].precedence;
		a_associativity = arv_evaluator_token_infos[a->token_id].associativity;

		return (((a_precedence <= b_precedence) &&
			a_associativity == ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_LEFT_TO_RIGHT) ||
			((a_precedence < b_precedence) &&
				a_associativity == ARV_EVALUATOR_TOKEN_ASSOCIATIVITY_RIGHT_TO_LEFT));
	}

	static ArvEvaluatorToken *arv_get_next_token(char **expression, bool previous_token_was_operand, bool previous_token_was_right_parenthesis)
	{
		ArvEvaluatorToken *token = NULL;
		ArvEvaluatorTokenId token_id = ARV_EVALUATOR_TOKEN_UNKNOWN;

		//g_return_val_if_fail(expression != NULL && *expression != NULL, NULL);
		//arv_str_skip_spaces(expression);

		if (**expression == '\0')
			return NULL;

// 		//if (g_ascii_isdigit(**expression)) 
// 		{
// 			char *end;
// 			lp_int64 v_int64;
// 			double v_double;
// 			ptrdiff_t length_int64;
// 			ptrdiff_t length_double;
// 
// 			//v_int64 = g_ascii_strtoll(*expression, &end, 0);
// 			length_int64 = end - *expression;
// 
// 			end = *expression;
// 			//arv_str_parse_double(&end, &v_double);
// 			length_double = end - *expression;
// 
// 			if (length_double > 0 || length_int64 > 0) {
// 				if (length_double > length_int64) {
// 					token = arv_evaluator_token_new_double(v_double);
// 					*expression += length_double;
// 				}
// 				else {
// 					token = arv_evaluator_token_new_int64(v_int64);
// 					*expression += length_int64;
// 				}
// 			}
// 		}
// 		else if (g_ascii_isalpha(**expression) || **expression == '_') {
// 			char *end = *expression;
// 			ptrdiff_t token_length;
// 
// 			while (g_ascii_isalnum(*end) || *end == '_')
// 				end++;
// 
// 			token_length = end - *expression;
// 
// 			if (token_length == 2) {
// 				if (g_ascii_strncasecmp("ln", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_LN;
// 				else if (g_ascii_strncasecmp("lg", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_LG;
// 			}
// 			else if (token_length == 3) {
// 				if (g_ascii_strncasecmp("sin", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_SIN;
// 				else if (g_ascii_strncasecmp("cos", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_COS;
// 				else if (g_ascii_strncasecmp("sgn", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_SGN;
// 				else if (g_ascii_strncasecmp("neg", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_NEG;
// 				else if (g_ascii_strncasecmp("tan", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_TAN;
// 				else if (g_ascii_strncasecmp("abs", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_ABS;
// 				else if (g_ascii_strncasecmp("exp", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_EXP;
// 			}
// 			else if (token_length == 4) {
// 				if (g_ascii_strncasecmp("atan", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_ATAN;
// 				else if (g_ascii_strncasecmp("sqrt", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_SQRT;
// 				else if (g_ascii_strncasecmp("ceil", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_CEIL;
// 				else if (g_ascii_strncasecmp("asin", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_ASIN;
// 				else if (g_ascii_strncasecmp("acos", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_ACOS;
// 			}
// 			else if (token_length == 5) {
// 				if (g_ascii_strncasecmp("trunc", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_TRUNC;
// 				else if (g_ascii_strncasecmp("round", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_ROUND;
// 				else if (g_ascii_strncasecmp("floor", *expression, token_length) == 0)
// 					token_id = ARV_EVALUATOR_TOKEN_FUNCTION_FLOOR;
// 			}
// 
// 			if (token_id != ARV_EVALUATOR_TOKEN_UNKNOWN)
// 				token = arv_evaluator_token_new(token_id);
// 			else {
// 				char *name = g_strndup(*expression, token_length);
// 				token = arv_evaluator_token_new_variable(name);
// 				g_free(name);
// 			}
// 
// 			*expression = end;
// 		}
// 		else {
// 			switch (**expression) {
// 			case '(': token_id = ARV_EVALUATOR_TOKEN_LEFT_PARENTHESIS; break;
// 			case ')': token_id = ARV_EVALUATOR_TOKEN_RIGHT_PARENTHESIS; break;
// 			case ',': token_id = ARV_EVALUATOR_TOKEN_COMMA; break;
// 			case '?': token_id = ARV_EVALUATOR_TOKEN_TERNARY_QUESTION_MARK; break;
// 			case ':': token_id = ARV_EVALUATOR_TOKEN_TERNARY_COLON; break;
// 			case '+': if (previous_token_was_operand ||
// 				previous_token_was_right_parenthesis)
// 				token_id = ARV_EVALUATOR_TOKEN_ADDITION;
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_PLUS;
// 				break;
// 			case '-': if (previous_token_was_operand ||
// 				previous_token_was_right_parenthesis)
// 				token_id = ARV_EVALUATOR_TOKEN_SUBSTRACTION;
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_MINUS;
// 				break;
// 			case '*': if ((*expression)[1] == '*') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_POWER;
// 			}
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_MULTIPLICATION;
// 				break;
// 			case '/': token_id = ARV_EVALUATOR_TOKEN_DIVISION; break;
// 			case '%': token_id = ARV_EVALUATOR_TOKEN_REMAINDER; break;
// 			case '&': if ((*expression)[1] == '&') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_LOGICAL_AND;
// 			}
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_BITWISE_AND;
// 				break;
// 			case '|': if ((*expression)[1] == '|') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_LOGICAL_OR;
// 			}
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_BITWISE_OR;
// 				break;
// 			case '^': token_id = ARV_EVALUATOR_TOKEN_BITWISE_XOR; break;
// 			case '~': token_id = ARV_EVALUATOR_TOKEN_BITWISE_NOT; break;
// 			case '<': if ((*expression)[1] == '>') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_NOT_EQUAL;
// 			}
// 					  else if ((*expression)[1] == '<') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_SHIFT_LEFT;
// 			}
// 					  else if ((*expression)[1] == '=') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_LESS_OR_EQUAL;
// 			}
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_LESS;
// 				break;
// 			case '>': if ((*expression)[1] == '>') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_SHIFT_RIGHT;
// 			}
// 					  else if ((*expression)[1] == '=') {
// 				(*expression)++;
// 				token_id = ARV_EVALUATOR_TOKEN_GREATER_OR_EQUAL;
// 			}
// 					  else
// 				token_id = ARV_EVALUATOR_TOKEN_GREATER;
// 				break;
// 			case '=': token_id = ARV_EVALUATOR_TOKEN_EQUAL; break;
// 			}
// 
// 			if (token_id != ARV_EVALUATOR_TOKEN_UNKNOWN) {
// 				(*expression)++;
// 				token = arv_evaluator_token_new(token_id);
// 			}
// 		}

		return token;
	}
	static double round_with_precision(double x, lp_int64 precision)
	{
		double pow_multiplier = pow(10, precision);
		return (round(x*pow_multiplier)) / pow_multiplier;
	}

	static int get_arguments_count(ArvEvaluatorValuesStackItem * stack, int current_index)
	{
		int arguments_count = 0;
		int index = current_index;
		int parenthesis_level = stack[index].parenthesis_level;
		while ((index >= 0) && (stack[index].parenthesis_level >= parenthesis_level)) {
			index--;
			arguments_count++;
		}
		return arguments_count;
	}
	/*static ArvEvaluatorStatus evaluate(GSList *token_stack, GHashTable *variables, gint64 *v_int64, double *v_double)
	{
		ArvEvaluatorToken *token;
		ArvEvaluatorStatus status;
		GSList *iter;
		ArvEvaluatorValuesStackItem stack[ARV_EVALUATOR_STACK_SIZE];
		ArvValue *value;
		int index = -1;
		gboolean integer_mode;

		g_assert(v_int64 != NULL || v_double != NULL);

		integer_mode = v_int64 != NULL;

		for (iter = token_stack; iter != NULL; iter = iter->next) {
			token = iter->data;

			if (index < (arv_evaluator_token_infos[token->token_id].n_args - 1)) {
				status = ARV_EVALUATOR_STATUS_MISSING_ARGUMENTS;
				goto CLEANUP;
			}

			if (arv_evaluator_token_infos[token->token_id].double_only && integer_mode) {
				status = ARV_EVALUATOR_STATUS_INVALID_DOUBLE_FUNCTION;
				goto CLEANUP;
			}

			if (index >= ARV_EVALUATOR_STACK_SIZE - 1) {
				status = ARV_EVALUATOR_STATUS_STACK_OVERFLOW;
				goto CLEANUP;
			}

			arv_evaluator_token_debug(token, variables);

			int actual_arguments_count = arv_evaluator_token_infos[token->token_id].n_args;

			switch (token->token_id) {
			case ARV_EVALUATOR_TOKEN_LOGICAL_AND:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) &&
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_LOGICAL_OR:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) ||
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_BITWISE_NOT:
				arv_value_set_int64(&stack[index].value,
					~arv_value_get_int64(&stack[index].value));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_BITWISE_AND:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) &
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_BITWISE_OR:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) |
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_BITWISE_XOR:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) ^
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_EQUAL:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) ==
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) ==
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_NOT_EQUAL:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) !=
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) !=
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_LESS_OR_EQUAL:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) <=
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) <=
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_GREATER_OR_EQUAL:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) >=
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) >=
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_LESS:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) <
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) <
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_GREATER:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) >
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) >
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_SHIFT_RIGHT:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) >>
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_SHIFT_LEFT:
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) <<
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_SUBSTRACTION:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) -
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_double(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) -
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_ADDITION:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) +
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_double(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) +
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_REMAINDER:
				if (arv_value_get_int64(&stack[index].value) == 0) {
					status = ARV_EVALUATOR_STATUS_DIVISION_BY_ZERO;
					goto CLEANUP;
				}
				arv_value_set_int64(&stack[index - 1].value,
					arv_value_get_int64(&stack[index - 1].value) %
					arv_value_get_int64(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_DIVISION:
				if (integer_mode) {
					if (arv_value_get_int64(&stack[index].value) == 0) {
						status = ARV_EVALUATOR_STATUS_DIVISION_BY_ZERO;
						goto CLEANUP;
					}
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) /
						arv_value_get_int64(&stack[index].value));
				}
				else {
					if (arv_value_get_double(&stack[index].value) == 0.0) {
						status = ARV_EVALUATOR_STATUS_DIVISION_BY_ZERO;
						goto CLEANUP;
					}
					arv_value_set_double(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) /
						arv_value_get_double(&stack[index].value));
				}
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_MULTIPLICATION:
				if (integer_mode ||
					(arv_value_holds_int64(&stack[index - 1].value) &&
						arv_value_holds_int64(&stack[index].value)))
					arv_value_set_int64(&stack[index - 1].value,
						arv_value_get_int64(&stack[index - 1].value) *
						arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_double(&stack[index - 1].value,
						arv_value_get_double(&stack[index - 1].value) *
						arv_value_get_double(&stack[index].value));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_POWER:
				if (integer_mode)
					arv_value_set_int64(&stack[index - 1].value,
						pow(arv_value_get_int64(&stack[index - 1].value),
							arv_value_get_int64(&stack[index].value)));
				else
					arv_value_set_double(&stack[index - 1].value,
						pow(arv_value_get_double(&stack[index - 1].value),
							arv_value_get_double(&stack[index].value)));
				stack[index - 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_MINUS:
				if (integer_mode || arv_value_holds_int64(&stack[index].value))
					arv_value_set_int64(&stack[index].value,
						-arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_double(&stack[index].value,
						-arv_value_get_double(&stack[index].value));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_PLUS:
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_SIN:
				arv_value_set_double(&stack[index].value, sin(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_COS:
				arv_value_set_double(&stack[index].value, cos(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_SGN:
				if (integer_mode || arv_value_holds_int64(&stack[index].value)) {
					gint64 int_value = arv_value_get_int64(&stack[index].value);
					if (int_value < 0)
						arv_value_set_int64(&stack[index].value, -1);
					else if (int_value > 0)
						arv_value_set_int64(&stack[index].value, 1);
					else
						arv_value_set_int64(&stack[index].value, 0);
				}
				else {
					double dbl_value = arv_value_get_double(&stack[index].value);
					if (dbl_value < 0.0)
						arv_value_set_int64(&stack[index].value, -1);
					else if (dbl_value > 0.0)
						arv_value_set_int64(&stack[index].value, 1);
					else
						arv_value_set_int64(&stack[index].value, 0);
				}
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_NEG:
				if (integer_mode || arv_value_holds_int64(&stack[index].value))
					arv_value_set_int64(&stack[index].value,
						-arv_value_get_int64(&stack[index].value));
				else
					arv_value_set_double(&stack[index].value,
						-arv_value_get_double(&stack[index].value));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_ATAN:
				arv_value_set_double(&stack[index].value, atan(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_TAN:
				arv_value_set_double(&stack[index].value, tan(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_ABS:
				if (arv_value_holds_double(&stack[index].value))
					arv_value_set_double(&stack[index].value,
						fabs(arv_value_get_double(&stack[index].value)));
				else
					arv_value_set_int64(&stack[index].value,
						llabs(arv_value_get_int64(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_EXP:
				arv_value_set_double(&stack[index].value, exp(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_LN:
				arv_value_set_double(&stack[index].value, log(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_LG:
				arv_value_set_double(&stack[index].value, log10(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_SQRT:
				arv_value_set_double(&stack[index].value, sqrt(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_TRUNC:
				if (arv_value_get_double(&stack[index].value) > 0.0)
					arv_value_set_double(&stack[index].value,
						floor(arv_value_get_double(&stack[index].value)));
				else
					arv_value_set_double(&stack[index].value,
						ceil(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_ROUND:
				actual_arguments_count = get_arguments_count(stack, index);
				if (actual_arguments_count == 1) {
					arv_value_set_double(&stack[index].value, round(arv_value_get_double(&stack[index].value)));
					stack[index].parenthesis_level = token->parenthesis_level;
				}
				else if (actual_arguments_count == 2) {
					arv_value_set_double(&stack[index - 1].value,
						round_with_precision(arv_value_get_double(&stack[index - 1].value),
							arv_value_get_int64(&stack[index].value)));
					stack[index - 1].parenthesis_level = token->parenthesis_level;
				}
				else {
					if (actual_arguments_count < 1) {
						status = ARV_EVALUATOR_STATUS_MISSING_ARGUMENTS;
					}
					else {
						status = ARV_EVALUATOR_STATUS_REMAINING_OPERANDS;
					}
					goto CLEANUP;
				}

				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_FLOOR:
				arv_value_set_double(&stack[index].value, floor(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_CEIL:
				arv_value_set_double(&stack[index].value, ceil(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_ASIN:
				arv_value_set_double(&stack[index].value, asin(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_FUNCTION_ACOS:
				arv_value_set_double(&stack[index].value, acos(arv_value_get_double(&stack[index].value)));
				stack[index].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_CONSTANT_INT64:
				arv_value_set_int64(&stack[index + 1].value, token->data.v_int64);
				stack[index + 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_CONSTANT_DOUBLE:
				if (integer_mode)
					arv_value_set_int64(&stack[index + 1].value, token->data.v_double);
				else
					arv_value_set_double(&stack[index + 1].value, token->data.v_double);
				stack[index + 1].parenthesis_level = token->parenthesis_level;
				break;
			case ARV_EVALUATOR_TOKEN_VARIABLE:
				value = g_hash_table_lookup(variables, token->data.name);
				if (value != NULL) {
					arv_value_copy(&stack[index + 1].value, value);
					stack[index + 1].parenthesis_level = token->parenthesis_level;
				}
				else {
					status = ARV_EVALUATOR_STATUS_UNKNOWN_VARIABLE;
					goto CLEANUP;
				}
				break;
			case ARV_EVALUATOR_TOKEN_TERNARY_COLON:
				break;
			case ARV_EVALUATOR_TOKEN_TERNARY_QUESTION_MARK:
				if (arv_value_get_int64(&stack[index - 2].value) != 0) {
					arv_value_copy(&stack[index - 2].value, &stack[index - 1].value);
					stack[index - 2].parenthesis_level = stack[index - 1].parenthesis_level;
				}
				else {
					arv_value_copy(&stack[index - 2].value, &stack[index].value);
					stack[index - 2].parenthesis_level = stack[index].parenthesis_level;
				}
				break;
			default:
				status = ARV_EVALUATOR_STATUS_UNKNOWN_OPERATOR;
				goto CLEANUP;
				break;
			}
			index = index - actual_arguments_count + 1;
		}

		if (index != 0) {
			status = ARV_EVALUATOR_STATUS_REMAINING_OPERANDS;
			goto CLEANUP;
		}

		if (v_double != NULL)
			*v_double = arv_value_get_double(&stack[0].value);

		if (v_int64 != NULL)
			*v_int64 = arv_value_get_int64(&stack[0].value);

		if (arv_value_holds_int64(&stack[0].value))
			arv_log_evaluator("[Evaluator::evaluate] Result = (int64) %Ld", arv_value_get_int64(&stack[0].value));
		else
			arv_log_evaluator("[Evaluator::evaluate] Result = (double) %g", arv_value_get_double(&stack[0].value));

		return ARV_EVALUATOR_STATUS_SUCCESS;
	CLEANUP:
		if (v_double != NULL)
			*v_double = 0.0;

		if (v_int64 != NULL)
			*v_int64 = 0.0;

		return status;
	}*/
/*
static ArvEvaluatorStatus
parse_to_stacks(ArvEvaluator *evaluator, char *expression, ArvEvaluatorParserState *state)
{
	ArvEvaluatorToken *token;
	ArvEvaluatorStatus status;
	gboolean token_found;
	gint32 current_parenthesis_level = 0;
	if (expression == NULL)
		return ARV_EVALUATOR_STATUS_EMPTY_EXPRESSION;


	do {
		token = arv_get_next_token(&expression, state->previous_token_was_operand, state->previous_token_was_right_parenthesis);

		if (token != NULL) {
			token_found = TRUE;
			token->parenthesis_level = current_parenthesis_level;
			state->previous_token_was_operand = arv_evaluator_token_is_operand(token);
			state->previous_token_was_right_parenthesis = arv_evaluator_token_is_right_parenthesis(token);

			if (arv_evaluator_token_is_variable(token)) {
				if (g_hash_table_lookup_extended(evaluator->priv->constants, token->data.name, NULL, NULL)) {
					const char *constant;

					constant = g_hash_table_lookup(evaluator->priv->constants, token->data.name);

					if (constant != NULL) {
						arv_evaluator_token_free(token);
						token = arv_get_next_token((char **)&constant, FALSE, FALSE);
						if (token != NULL)
							state->token_stack = g_slist_prepend(state->token_stack, token);
					}
					else {
						status = ARV_EVALUATOR_STATUS_UNKNOWN_CONSTANT;
						goto CLEANUP;
					}
				}
				else if (g_hash_table_lookup_extended(evaluator->priv->sub_expressions, token->data.name, NULL, NULL)) {
					const char *sub_expression;

					sub_expression = g_hash_table_lookup(evaluator->priv->sub_expressions, token->data.name);

					if (sub_expression != NULL) {
						char *string;

						if (state->in_sub_expression) {
							status = ARV_EVALUATOR_STATUS_FORBIDDEN_RECUSRION;
							goto CLEANUP;
						}

						string = g_strdup_printf("(%s)", sub_expression);
						state->in_sub_expression = TRUE;
						status = parse_to_stacks(evaluator, string, state);
						state->in_sub_expression = FALSE;
						g_free(string);

						if (status != ARV_EVALUATOR_STATUS_SUCCESS) {
							goto CLEANUP;
						}

						arv_evaluator_token_free(token);
						token = NULL;
					}
					else {
						status = ARV_EVALUATOR_STATUS_UNKNOWN_SUB_EXPRESSION;
						goto CLEANUP;
					}
				}
				else {
					state->token_stack = g_slist_prepend(state->token_stack, token);
				}
			}
			else if (arv_evaluator_token_is_operand(token)) {
				state->token_stack = g_slist_prepend(state->token_stack, token);
			}
			else if (arv_evaluator_token_is_comma(token)) {
				while (state->operator_stack != NULL &&
					!arv_evaluator_token_is_left_parenthesis(state->operator_stack->data)) {
					state->token_stack = g_slist_prepend(state->token_stack, state->operator_stack->data);
					state->operator_stack = g_slist_delete_link(state->operator_stack, state->operator_stack);
				}
				if (state->operator_stack == NULL ||
					!arv_evaluator_token_is_left_parenthesis(state->operator_stack->data)) {
					status = ARV_EVALUATOR_STATUS_PARENTHESES_MISMATCH;
					goto CLEANUP;
				}
				state->garbage_stack = g_slist_prepend(state->garbage_stack, token);
			}
			else if (arv_evaluator_token_is_operator(token)) {
				while (state->operator_stack != NULL &&
					arv_evaluator_token_compare_precedence(token, state->operator_stack->data)) {
					state->token_stack = g_slist_prepend(state->token_stack, state->operator_stack->data);
					state->operator_stack = g_slist_delete_link(state->operator_stack, state->operator_stack);
				}
				state->operator_stack = g_slist_prepend(state->operator_stack, token);
			}
			else if (arv_evaluator_token_is_left_parenthesis(token)) {
				current_parenthesis_level += 1;
				state->operator_stack = g_slist_prepend(state->operator_stack, token);
			}
			else if (arv_evaluator_token_is_right_parenthesis(token)) {
				current_parenthesis_level -= 1;
				while (state->operator_stack != NULL &&
					!arv_evaluator_token_is_left_parenthesis(state->operator_stack->data)) {
					state->token_stack = g_slist_prepend(state->token_stack, state->operator_stack->data);
					state->operator_stack = g_slist_delete_link(state->operator_stack, state->operator_stack);
				}
				if (state->operator_stack == NULL) {
					status = ARV_EVALUATOR_STATUS_PARENTHESES_MISMATCH;
					goto CLEANUP;
				}
				state->garbage_stack = g_slist_prepend(state->garbage_stack, token);
				state->garbage_stack = g_slist_prepend(state->garbage_stack, state->operator_stack->data);
				state->operator_stack = g_slist_delete_link(state->operator_stack, state->operator_stack);
			}
			else {
				status = ARV_EVALUATOR_STATUS_SYNTAX_ERROR;
				goto CLEANUP;
			}
			(state->count)++;
		}
		else if (*expression != '\0') {
			status = ARV_EVALUATOR_STATUS_SYNTAX_ERROR;
			goto CLEANUP;
		}
		else {
			token_found = FALSE;
		}
	} while (token_found);

	return ARV_EVALUATOR_STATUS_SUCCESS;

CLEANUP:
	if (token != NULL)
		arv_evaluator_token_free(token);

	return status;
}*/
/*
static void
free_rpn_stack(ArvEvaluator *evaluator)
{
	GSList *iter;

	for (iter = evaluator->priv->rpn_stack; iter != NULL; iter = iter->next)
		arv_evaluator_token_free(iter->data);
	g_slist_free(evaluator->priv->rpn_stack);
	evaluator->priv->rpn_stack = NULL;
}*/
/*

static ArvEvaluatorStatus
parse_expression (ArvEvaluator *evaluator)
{
	ArvEvaluatorParserState state;
	ArvEvaluatorStatus status;
	GSList *iter;
	int count;

	state.count  =0;
	state.previous_token_was_operand = FALSE;
	state.previous_token_was_right_parenthesis = FALSE;
	state.token_stack = NULL;
	state.operator_stack = NULL;
	state.garbage_stack = NULL;
	state.in_sub_expression = FALSE;

	free_rpn_stack (evaluator);

	arv_log_evaluator ("[Evaluator::parse_expression] %s", evaluator->priv->expression);

	status = parse_to_stacks (evaluator, evaluator->priv->expression, &state);

	if (status != ARV_EVALUATOR_STATUS_SUCCESS)
		goto CLEANUP;

	arv_log_evaluator ("[Evaluator::parse_expression] Found %d items in expression", state.count);

	while (state.operator_stack != NULL) {
		if (arv_evaluator_token_is_left_parenthesis (state.operator_stack->data)) {
			status = ARV_EVALUATOR_STATUS_PARENTHESES_MISMATCH;
			goto CLEANUP;
		}
		state.token_stack = g_slist_prepend (state.token_stack, state.operator_stack->data);
		state.operator_stack = g_slist_delete_link (state.operator_stack, state.operator_stack);
	}

	evaluator->priv->rpn_stack = g_slist_reverse (state.token_stack);

	for (iter = state.garbage_stack, count = 0; iter != NULL; iter = iter->next, count++)
		arv_evaluator_token_free (iter->data);
	g_slist_free (state.garbage_stack);

	arv_log_evaluator ("[Evaluator::parse_expression] %d items in garbage list", count);
	arv_log_evaluator ("[Evaluator::parse_expression] %d items in token list", g_slist_length (evaluator->priv->rpn_stack));

	return evaluator->priv->rpn_stack == NULL ? ARV_EVALUATOR_STATUS_EMPTY_EXPRESSION : ARV_EVALUATOR_STATUS_SUCCESS;

CLEANUP:
	for (iter = state.garbage_stack; iter != NULL; iter = iter->next)
		arv_evaluator_token_free (iter->data);
	g_slist_free (state.garbage_stack);
	for (iter = state.token_stack; iter != NULL; iter = iter->next)
		arv_evaluator_token_free (iter->data);
	g_slist_free (state.token_stack);
	for (iter = state.operator_stack; iter != NULL; iter = iter->next)
		arv_evaluator_token_free (iter->data);
	g_slist_free (state.operator_stack);

	return status;
}
*/
private:
 	char *expression;
// 	std::vector<int> *rpn_stack;
 	ArvEvaluatorStatus parsing_status;
// 	std::hash_map<int,std::string> *variables;
// 	std::hash_map<int, std::string> *sub_expressions;
// 	std::hash_map<int, std::string> *constants;
};

