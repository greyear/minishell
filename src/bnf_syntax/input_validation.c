
#include "../../include/minishell.h"

//- `<v_input>	::= <pipeline> <newline>`
int	validate_input(char *str)
{
	char	*next;
	int		err_flag;

	//что я делаю, если вводят \n?
	err_flag = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == NULL_TERM)
		return (0); //empty line is handled as success
	next = validate_pipeline(str, &err_flag);
	if (err_flag == 1 || *next != NULL_TERM) //line DOESN'T end with \0
	{
		//error message
		return (1); // code?
	}
	return (0);
}
