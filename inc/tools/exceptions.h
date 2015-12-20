/*
** exception.h for  in /home/penava_b/perso/test
** 
** Made by penava_b
** Login   <penava_b@epitech.net>
** 
** Started on  Fri Nov 27 02:11:31 2015 penava_b
** Last update Fri Dec 18 18:52:35 2015 penava_b
*/

#pragma once

#include <setjmp.h>
#include "tools/new_delete.h"

jmp_buf		*__except_get_front();
void	       	__except_initializer(void *);
int	       	__except_dispatcher(const char *, const char *, int);
void	       	__except_throw_func(const void *, void *, const char *,
				    const char *, int);
int	       	__except_catch_func(const void *);
void		*__except_get_data();
char		__except_get_catch_tool();

struct	       	__false_except_list
{
  void		*_1;
  int	       	_2;
  void	       	*_3;
  void   	*_4;
  void	 	*_5;
  jmp_buf      	_6;
  void    	*_7;
  char		_8;
};


#define try								\
  for (__except_initializer((struct __false_except_list[1]){});		\
       __except_dispatcher(__FILE__, __FUNCTION__, __LINE__) ;)		\
    if (setjmp(*__except_get_front()) == 0)

#define catch(type, name)						\
  else if (__except_catch_func(type ## _type_instance))				\
    for (type *name __attribute__((unused)) = __except_get_data(); __except_get_catch_tool(); )

#define throw(type, ctor, ...)						\
  __except_throw_func(type ## _type_instance, new(type, ctor, ##__VA_ARGS__), \
		      __FILE__, __FUNCTION__, __LINE__)
