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

typedef struct queue_type* Queue;

/* 智能队列指针实现 */
#define smart_Queue __attribute__((cleanup(smart_queue_free)))

#ifdef __cplusplus
extern "C"
{
#endif

Queue queue_create(void);
int queue_destroy(Queue s);
int queue_make_empty(Queue s);
int queue_is_empty(const Queue s);
int queue_length(Queue s, unsigned int *len);
int queue_enqueue(Queue s, const void* p, int len);
int queue_dequeue(Queue s, void* p, int len);

/**
 * @brief 为实现队列智能指针重新封装队列销毁函数
 *
 * @param s 指向队列指针的指针
 */
__attribute__ ((always_inline))
inline void smart_queue_free(Queue* s)
{
    queue_destroy(*s);
}

#ifdef __cplusplus
}
#endif

#endif

