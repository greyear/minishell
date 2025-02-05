
#include "../../include/minishell.h"

/*
envps:
✅ Allowed Characters in Environment Variable Names
Uppercase Letters: A-Z
Digits (0-9) – but NOT at the start
Underscore (_)
❌ Not Allowed
Lowercase letters (a-z) → Though technically allowed, they are usually not used by convention.
Special Characters (!@#$%^&*()-+=,.:;'"<>?[]{})
Spaces → MY VAR=hello is invalid.
Starting with a Number → 1HOME=/root is invalid.
*/

/*
✅ Работает (корректно подставляет значение переменной):

export VAR=test
echo $VAR        # Выведет: test
echo "$VAR"      # Выведет: test
echo '${VAR}'    # Выведет: ${VAR} (кавычки предотвращают подстановку)

❌ Ошибки (неверное использование кавычек после $):

echo $"VAR"      # Выведет: "VAR", так как $ перед кавычками ничего не делает
echo $'VAR'      # Выведет: VAR (в $'...' работает ANSI-экранирование, но не подстановка переменной)
echo $"VAR text" # Выведет: "VAR text" (не подставляет значение переменной)
*/

t_bool	is_envp_symbol(int c)
{
	if (ft_isalnum(c) || c == UNDERSC)
		return (true);
	return (false);
}
