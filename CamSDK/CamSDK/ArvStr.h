#pragma once
class ArvStr
{
public:
	ArvStr();
	~ArvStr();

	char *	arv_str_strip();
	bool	arv_str_is_uri();
	char *	arv_str_to_uri();
	bool	arv_str_parse_double();
	unsigned int	arv_str_parse_double_list();
	void	arv_str_skip_char();
	void	arv_str_skip_colon_and_spaces();
	void	arv_str_skip_comma_and_spaces();
	void	arv_str_skip_semicolon_and_spaces();
	void	arv_str_skip_spaces();
};

