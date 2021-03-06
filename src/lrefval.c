/*
 * Copyright (C) 2016  Bastien Penavayre
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>

/*-----------------------linked list-------------------------------*/

struct		s_node
{
  void		*data;
  void		(*dtor)(void *);
  int	       	level;
  struct s_node	*next;
  void		*id;
};

static __thread
struct
{
  int			level;
  void			(*return_dtor)(void *);
  struct s_node		*front;
  void			*new_id;
  struct s_node		*to_update;
  struct s_node		*to_update_next;
}			__stack_list = {0, 0, 0, 0, 0, 0};

void		__except_clean_exit_func(int);

static
void		__enter_new_func()
{
  __stack_list.level++;
}

int		__get_current_level()
{
  return __stack_list.level;
}

void		__protect_kill_stack(void *id)
{
  __stack_list.new_id = id;
  for (struct s_node *node = __stack_list.front; node != 0 && node->level == __stack_list.level; node = node->next)
    if (node->id == id)
      {
	__stack_list.to_update = node;
	__stack_list.to_update_next = node->next;
	node->dtor(node->data);
	break ;
      }
}

void		*__push_var(struct s_node *new_node)
{
  if (new_node == __stack_list.to_update)
    {
      new_node->next = __stack_list.to_update_next;
      __stack_list.to_update = 0;
      __stack_list.to_update_next = 0;
    }
  else
    {
      new_node->next = __stack_list.front;
      __stack_list.front = new_node;
    }
  new_node->id = __stack_list.new_id;
  return new_node->data;
}

void		*__get_front_var_list()
{
  if (__stack_list.front == 0)
    return 0;
  return __stack_list.front->data;
}

void		__exit_end_func(int level)
{
  struct s_node	*front = __stack_list.front;
  struct s_node *node = 0;

  for (node = __stack_list.front; node != 0 && node->level >= level; node = node->next)
    __stack_list.front = node->next;
  for (node = front; node != 0 && node != __stack_list.front; node = node->next)
    if (node->dtor != 0)
      node->dtor(node->data);
  __except_clean_exit_func(level);
  __stack_list.level = level - 1;
}

void		__delayed_level_encrementation()
{
  if (__stack_list.front == 0 || __stack_list.level == 0)
    return ;
  __stack_list.front->level++;
}

void		*__prevent_clean_up_var(void *arg)
{
  struct
    s_node	*node;

  for (node = __stack_list.front;
       node != 0 && node->data > arg; node = node->next);
  if (node != 0 && node->data == arg)
    {
      __stack_list.return_dtor = node->dtor;
      node->dtor = 0;
    }
  return arg;
}

void		(*__get_return_dtor())(void *)
{
  return __stack_list.return_dtor;
}

__attribute__((destructor))
static void    	__clean_for_exit()
{
  __exit_end_func(-1);
}

/*-----------------------instrument functions-------------------------------*/

void	__cyg_profile_func_enter(void *func, void *from)
{
  (void)func, (void)from;
  __enter_new_func();
}

void	__cyg_profile_func_exit(void *func, void *from)
{
  (void)func, (void)from;
  __exit_end_func(__stack_list.level);
}
