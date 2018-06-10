/* Copyright (C) 
 * 2017 - quintin
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
/**
 * @file stack.c
 * @brief 对堆栈数据结构进行封装，实现了堆栈的创建、删除、清空、入栈、出栈操作。
 * @author quintin
 * @version 1.0
 * @date 2017-11-12
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "list.h"
#include "stack.h"

/**
 * @brief  堆栈数据结构
 */
struct stack_type {
    unsigned int cnt;    // 堆栈数据计数
    void *data;          // 堆栈中的数据
    list_link item;      // 堆栈链表指针
};

/**
 * @brief 创建堆栈
 *
 * @return 堆栈指针
 */
Stack stack_create(void)
{
    struct stack_type *s = NULL;
    s = (struct stack_type*)malloc(sizeof(struct stack_type));
    if (!s)
        return NULL;
    memset(s, 0, sizeof(struct stack_type));
    INIT_LIST_HEAD(&(s->item));
    return s;
}

/**
 * @brief 判断堆栈是否为空
 *
 * @param s[in] 堆栈指针
 *
 * @return 1, 堆栈为空
 *         0, 堆栈非空
 */
int stack_is_empty(const Stack s)
{
    return (s->data == NULL);
}

/**
 * @brief 入栈操作
 *
 * @param s[in]   堆栈指针
 * @param dat[in] 入栈数据
 * @param len[in] 入栈数据长度
 *
 * @return 1, 失败
 *         0, 成功
 */
int push(Stack s, const void* dat, int len)
{
    char *data  = (char*)dat;

    if (!s)
        return 1;

    if (stack_is_empty(s)) {
        s->data = malloc(sizeof(char)*len);
        if (!s->data)
            return 1;
        memset(s->data, 0, sizeof(char)*len);
        memcpy(s->data, data, len);
        s->cnt++;
        return 0;
    }

    struct stack_type *temp = (struct stack_type*)malloc(sizeof(struct stack_type));
    if (!temp)
        return 1;
    memset(temp, 0, sizeof(struct stack_type));

    temp->data = malloc(sizeof(char)*len);
    if (!temp->data)
        return 1;
    memset(temp->data, 0, sizeof(char)*len);

    memcpy(temp->data, data, len);

    /* 读取堆栈顶指针，以便获取当前堆栈深度 */
    struct stack_type *tail = list_entry(struct stack_type, item, s->item.l);
    temp->cnt = tail->cnt+1;
    list_add(&temp->item, &s->item);
    return 0;
}

/**
 * @brief 出栈操作
 *
 * @param s[in]    堆栈指针
 * @param dat[out] 出栈数据
 * @param len[in]  出栈数据长度
 *
 * @return 1, 失败
 *         0, 成功
 */
int pop(struct stack_type* s, void* dat, int len)
{
    if (stack_is_empty(s))
        return 1;

    /* 定位链表尾，取出数据，并删除该节点 */
    struct stack_type *temp = list_entry(struct stack_type, item, s->item.l);

    memcpy(dat, temp->data, len);
    free(temp->data);
    temp->data = NULL;

    /* 如果到栈底只需清空数据 */
    if (1 == temp->cnt) {
        temp->cnt = 0;
        return 0;
    }

    list_del(&temp->item);

    free(temp);
    temp = NULL;
    return 0;
}

/**
 * @brief 清空堆栈
 *
 * @param s[in] 堆栈指针
 *
 * @return 0, 成功
 */
int stack_make_empty(struct stack_type* s)
{
    plist_link li = NULL;
   	plist_link ly = NULL;
    struct stack_type *ptests = NULL;

    /* 遍历整个链表，并释放内存 */
    list_for_each_safe(li , ly , &s->item) {
        ptests = list_entry(struct stack_type , item , li);
        if (ptests) {
            if (ptests->data) {
                free(ptests->data);
                ptests->data = NULL;
            }
            free(ptests);
            ptests = NULL;
        }
    }
    s->cnt = 0;
    free(s->data);
    s->data = NULL;
    s->item.l = &s->item;
    s->item.r = &s->item;
    return 0;
}

/**
 * @brief 销毁堆栈
 *
 * @param s[in] 堆栈指针
 *
 * @return 0, 成功
 */
int stack_destroy(struct stack_type* s)
{
    plist_link li = NULL;
    plist_link ly = NULL;
    struct stack_type *ptests = NULL;
    /* 在销毁堆栈前先遍历整个链表，并释放内存 */
    list_for_each_safe(li , ly , &s->item) {
        ptests = list_entry(struct stack_type , item , li);
        if (ptests) {
            if (ptests->data)
                free(ptests->data);

            free(ptests);
            ptests = NULL;
        }
    }
    /* 释放堆栈指针，销毁整个堆栈 */
    if (s) {
        free(s);
        s = NULL;
    }
    return 0;
}

/**
 * @brief 重新封装堆栈销毁函数，以便用于智能指针
 *
 * @param ptr 指向堆栈指针的指针
 */
/*
inline void smartfree_stack(Stack* ptr) {
    stack_destroy(*ptr);
}
*/
