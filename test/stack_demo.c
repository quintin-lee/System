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
/**
 * @file main.c
 * @brief 测试堆栈数据结构功能
 * @author Quintin
 * @version 1.0
 * @date 2017-11-11
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/stack.h"

/**
 * @brief 主函数，测试堆栈数据结构功能
 *
 * @param argc 参数个数
 * @param argv[] 参数
 *
 * @return 0: success
 *         1: failed
 */
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    smart_stack Stack s1 = stack_create();
    smart_stack Stack s2 = stack_create();
    char str[10][100] =
    {
        "data0",
        "data1",
        "data2",
        "data3",
        "data4",
        "data5",
        "data6",
        "data7",
        "data8",
        "data9"
    };
    //int a = 2;
    //int b = 9;
    //int c = 40;
    //push(s1, (void*)&a, sizeof(a));
    //push(s1, (void*)&b, sizeof(b));
    //push(s1, (void*)&c, sizeof(c));
    int i = 0;
    printf("push datas into s1...");
    for (i = 0; i < 10; i++) {
        push(s1, (void*)&i, sizeof(i));
    }
    printf("ok\r\n");

    printf("push datas into s2...");
    for (i = 0; i < 10; i++) {
        push(s2, (void*)str[i], sizeof(str[i]));
    }
    printf("ok\r\n");
    i = 0;
    printf("clear s1...");
    stack_make_empty(s1);
    printf("ok\r\n");
    printf("pop datas from s1,s2...\r\n");
    while(!stack_is_empty(s1) || !stack_is_empty(s2)) {
        int d=0;
        pop(s1, &d, sizeof(d));
        pop(s2, str[i], sizeof(str[i]));
        printf("s1 pop:%d   s2 pop:%s\n", d, str[i]);
        i++;
    }
    printf("ok\r\n");
    //getchar();
    //stack_destroy(s1);
    return 0;
}
