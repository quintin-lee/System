
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/list.h"

typedef struct _tag_test_struct {
    int       a;
    int       b;
    char      buf[1024];
    list_link item;
}test_struct;

LIST_HEAD(test_list_header);

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    plist_link li = NULL;
   	plist_link ly = NULL;
   	test_struct *ptests;
   	/* test_struct *ptest_seek; */

    ptests = (test_struct*)malloc(sizeof(test_struct));
    memset(ptests, 0, sizeof(test_struct));
    ptests->a = 10;
    ptests->b = 11;
    snprintf(ptests->buf, sizeof(ptests->buf), "%s", "aaa aaa");
    list_add(&ptests->item, &test_list_header);

    ptests = (test_struct*)malloc(sizeof(test_struct));
    memset(ptests, 0, sizeof(test_struct));
    ptests->a = 20;
    ptests->b = 21;
    snprintf(ptests->buf, sizeof(ptests->buf), "%s", "bbb aaa");
    list_add(&ptests->item, &test_list_header);

    list_for_each_entry_reverse(ptests , &test_list_header , test_struct , item){
        fprintf(stderr , "list_for_each_entry_reverse : %d , %s\n" , ptests->a , ptests->buf);
    }
    list_for_each(li , &test_list_header){
        ptests = list_entry( test_struct , item , li);
        fprintf(stderr , "list_for_each : %d , %s\n" , ptests->a , ptests->buf);
    }

    //removed list entry
    list_for_each_safe(li , ly , &test_list_header) {
        ptests = list_entry( test_struct , item , li);
        if (ptests) {
            free(ptests);
        }
    }

    // clean list
    list_empty(&test_list_header);
    return 0;
}
