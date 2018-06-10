/* Copyright (C) 
 * 2017 - Quintin
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details._gst_reserved* 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>

typedef struct stack_type* Stack;

/* 实现智能指针 */
#define smart_stack __attribute__((cleanup(smartfree_stack)))

#ifdef __cplusplus
extern "C"
{
#endif

Stack stack_create(void);
int stack_destroy(Stack s);
int stack_make_empty(Stack s);
int stack_is_empty(const Stack s);
int push(Stack s, const void* p, int len);
int pop(Stack s, void* p, int len);

/**
 * @brief 重新封装堆栈销毁函数，以便用于智能指针
 *
 * @param ptr 指向堆栈指针的指针
 */
__attribute__ ((always_inline))
inline void smartfree_stack(Stack* ptr) {
    stack_destroy(*ptr);
}

#ifdef __cplusplus
}
#endif

#endif

