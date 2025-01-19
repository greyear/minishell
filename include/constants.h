
#ifndef CONSTANTS_H
# define CONSTANTS_H

typedef enum e_err
{
	ARGS_NUM,
	INV_ARGS,
	MLLC
}	t_err;

typedef enum e_exit
{
	SUCCESS = 0, //??
	CMD_EXEC = 126,
	CMD_NF = 127,
	MALLOC_ERR,
}	t_exit;


/*POSIX ограничивает диапазон exit status от 0 до 255. 
При использовании значений выше 255 операционная система может 
"обрезать" коды до младших 8 бит, что вызовет некорректные 
результаты*/

#endif