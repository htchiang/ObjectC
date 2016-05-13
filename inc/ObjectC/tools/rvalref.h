/*
** lrefval.h for  in /home/penava_b/perso/Obj/Obj4/Future/LeftValueRef
** 
** Made by penava_b
** Login   <penava_b@epitech.net>
** 
** Started on  Fri Jan  1 10:12:30 2016 penava_b
** Last update Sun May  8 14:26:40 2016 penava_b
*/

#pragma once

struct		s_right_value_node
{
  void		*data;
  void		(*dtor)(void *);
  char		toclean;
  int		level;
  struct s_right_value_node	*next;
  void		*id;
};

int		__get_current_level();
void		*__push_var(struct s_right_value_node *);
void		*__get_front_var_list();
void		__exit_end_func(int);
void		__prevent_clean_up();
void		__reset_clean_up();
void		__delayed_level_encrementation();
void		*__prevent_clean_up_var(void *);
void		(*__get_return_dtor())(void *);
long		__get_front_node_diff(const char *);
void		__push_back_on_stack(void *, int);
void		__protect_kill_stack(void *);

#define rvalue(type, ctor, ...)                                         \
  (__protect_kill_stack((char[1]){0}),					\
   type ## _ ## ctor							\
   (__pre_ctor_ ## type							\
    (__push_var((struct s_right_value_node[1])                          \
		{{ (type[1]){},						\
                            (void *)type ## _dtor,                      \
                                42,                                     \
                                __get_current_level(),                  \
                                (void *)0, (void *)0                    \
                                }})), ##__VA_ARGS__),                   \
   __delayed_level_encrementation(),					\
   (type *)__get_front_var_list())

#define function_rvalue(type, ctor, ...)                \
    (__protect_kill_stack((char[1]){0}),                \
         type ## _ ## ctor                              \
         (__pre_ctor_ ## type                           \
          (__push_var((struct s_right_value_node[1])    \
          {{ (type[1]){},                               \
                 (void *)type ## _dtor,                 \
                 42,                                    \
                 __get_current_level(),                 \
                 (void *)0, (void *)0                   \
                 }})), ##__VA_ARGS__),                  \
         (type *)__get_front_var_list())

#define stdmove(x) (*(__typeof__(x))__prevent_clean_up_var(x))

#define retvalue(x)						\
  (__protect_kill_stack((char[1]){0}),				\
   (__typeof__(x) *)						\
   __push_var((struct s_right_value_node[1])                    \
	      {{						\
		  (__typeof__(x)[1]){x},			\
		    __get_return_dtor(),			\
		      42,					\
		      __get_current_level() + 1,		\
		      (void *)0, (void *)0			\
		      }})					\
   )