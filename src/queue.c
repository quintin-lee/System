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
 * @file queue.c
 * @brief 对队列数据结构进行封装，实现了队列的创建、销毁、入队、出队、清空操作。
 * @author quintin
 * @version 1.0
 * @date 2017-11-12
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "list.h"
#include "queue.h"

/**
 * @brief 队列数据结构
 */
struct queue_type {
    unsigned int cnt; // 队列数据计数,方便计算队列长度
    void *data;       // 队列数据
    list_link item;   // 队列节点
};

/**
 * @brief 创建队列
 *
 * @return 队列指针
 */
Queue queue_create(void)
{
    struct queue_type *s = NULL;
    s = (struct queue_type*)malloc(sizeof(struct queue_type));
    if (!s)
        return NULL;
    memset(s, 0, sizeof(struct queue_type));
    INIT_LIST_HEAD(&(s->item));
    return s;
}

/**
 * @brief 判断队列是否为空
 *
 * @param s[in] 队列指针
 *
 * @return 1, 队列为空
 *         0, 队列非空
 */
int queue_is_empty(const Queue s)
{
    return (s->data == NULL);
}

/**
 * @brief 入队操作
 *
 * @param s[in]   队列指针
 * @param dat[in] 入队数据
 * @param len[in] 数据长度
 *
 * @return 1, 失败
 *         0, 成功
 */
int queue_enqueue(Queue s, const void* dat, int len)
{
    char *data  = (char*)dat;
    if (!s)
        return 1;

    if (queue_is_empty(s)) {
        s->data = malloc(sizeof(char)*len);
        if (!s->data)
            return 1;
        memset(s->data, 0, sizeof(char)*len);
        memcpy(s->data, data, len);
        s->cnt++;
        return 0;
    }

    struct queue_type *temp = (struct queue_type*)malloc(sizeof(struct queue_type));
    if (!temp)
        return 1;
    memset(temp, 0, sizeof(struct queue_type));

    temp->data = malloc(sizeof(char)*len);
    if (!temp->data)
        return 1;
    memset(temp->data, 0, sizeof(char)*len);

    memcpy(temp->data, data, len);

    struct queue_type *tail = list_entry(struct queue_type, item, s->item.l);
    temp->cnt = tail->cnt+1;
    list_add(&temp->item, &s->item);
    return 0;
}

/**
 * @brief 出队操作
 *
 * @param s[in]    队列指针
 * @param dat[out] 出队数据
 * @param len[in]  数据长度
 *
 * @return 1, 失败
 *         0, 成功
 */
int queue_dequeue(struct queue_type* s, void* dat, int len)
{
    if (queue_is_empty(s))
        return 1;

    /* 将对首数据取出 */
    memcpy(dat, s->data, len);
    memset(s->data, 0, len);

    /* 将后面的数据前移 */
    plist_link li = NULL;
    struct queue_type *temp = list_entry(struct queue_type, item, s->item.r);

    if (temp->data && (temp != s)) {
        memcpy(s->data, temp->data, len);
        free(temp->data);
        temp->data = NULL;
        s->cnt = temp->cnt;
        list_del(&temp->item);
    } else {
        if (s->data) {
            free(s->data);
            s->data = NULL;
        }
        s->cnt = 0;
    }

    if (temp != s && temp) {
        free(temp);
        temp = NULL;
    }
    return 0;
}

/**
 * @brief 获取队列长度
 *
 * @param s[in]    队列指针
 * @param len[out] 队列长度
 *
 * @return 1, 失败
 *         0, 成功
 */
int queue_length(struct queue_type* s, unsigned int *len)
{
    struct queue_type *tail = list_entry(struct queue_type, item, s->item.l);
    if (len) {
        if (queue_is_empty(s))
            *len = 0;
        else if (tail->cnt < s->cnt)
            *len = 65536 - s->cnt + tail->cnt;
        else
            *len = tail->cnt - s->cnt + 1;
        return 0;
    } else
        return 1;
}

/**
 * @brief 清空队列
 *
 * @param s[in] 队列指针
 *
 * @return 0, 成功
 */
int queue_make_empty(struct queue_type* s)
{
    plist_link li = NULL;
   	plist_link ly = NULL;
    struct queue_type *ptests = NULL;
    list_for_each_safe(li , ly , &s->item) {
        ptests = list_entry(struct queue_type , item , li);
        if (ptests) {
            if (ptests->data)
                free(ptests->data);

            free(ptests);
            ptests = NULL;
        }
    }
    if (s->data) {
        free(s->data);
        s->data = NULL;
    }
    s->item.l = &s->item;
    s->item.r = &s->item;
    s->cnt = 0;
    return 0;
}

/**
 * @brief 销毁队列
 *
 * @param s[in] 队列指针
 *
 * @return 0, 成功
 */
int queue_destroy(struct queue_type* s)
{
    plist_link li = NULL;
   	plist_link ly = NULL;
    struct queue_type *ptests = NULL;
    list_for_each_safe(li , ly , &s->item) {
        ptests = list_entry(struct queue_type , item , li);
        if (ptests) {
            if (ptests->data)
                free(ptests->data);

            free(ptests);
            ptests = NULL;
        }
    }

    if (s) {
        free(s);
        s = NULL;
    }
    return 0;
}

/**
 * @brief 为实现队列智能指针重新封装队列销毁函数
 *
 * @param s 指向队列指针的指针
 */
/*
void smart_queue_free(struct queue_type** s)
{
    queue_destroy(*s);
}
*/
