#pragma once
class ArvStatistic
{
public:
	ArvStatistic();
	~ArvStatistic();

	ArvStatistic *	arv_statistic_new();
	void	arv_statistic_free();
	void	arv_statistic_reset();
	bool	arv_statistic_fill();
	void	arv_statistic_set_name();
	char *	arv_statistic_to_string();
};

