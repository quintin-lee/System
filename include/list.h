/* Copyright (C)
 * 2017
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
#ifndef __UTILS_LIST_HEADER__
#define __UTILS_LIST_HEADER__

#include <stddef.h>

#define prefetch(x) (x = x)


#if defined(WIN32) && !defined(__cplusplus)
#undef inline
#define inline __inline
#endif

#ifndef structof
#define structof(type,field,ptr) ((type *)(((char *)(ptr))-offsetof(type, field)))
#endif

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the l/r entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

typedef struct tag_list_link {
  struct tag_list_link *l, *r;
}list_link, *plist_link;


#ifdef __cplusplus
extern "C"
{
#endif

static inline void INIT_LIST_HEAD(plist_link li)
{
  li->l = li;
  li->r = li;
}

static inline void __list_add(plist_link li,
            plist_link r,
            plist_link l)
{
  l->r = li;
  li->l = l;
  li->r = r;
  r->l = li;
}

static inline void list_add(plist_link li, plist_link lh)
{
  __list_add(li, lh, lh->l);
}

static inline void list_add_tail(plist_link li, plist_link lh)
{
  __list_add(li, lh->r, lh);
}

static inline void __list_del(plist_link r, plist_link l)
{
  l->r = r;
  r->l = l;
}

static inline void list_del(plist_link li)
{
  __list_del(li->r, li->l);
  li->l = li;
  li->r = li;
}

static inline void list_del_init(plist_link li)
{
  __list_del(li->r, li->l);
  INIT_LIST_HEAD(li);
}

static inline int list_is_empty(const plist_link lh)
{
  return lh->l == lh;
}

static inline void list_empty(plist_link lh)
{
	lh->l = lh;
	lh->r = lh;
}

static inline void __list_splice(plist_link li,
         plist_link lh)
{
  plist_link first = li->l;
  plist_link last = li->r;
  plist_link at = lh->l;

  first->r = lh;
  lh->l = first;

  last->l = at;
  at->r = last;
}

static inline void list_splice(plist_link li, plist_link lh)
{
  if (!list_is_empty(li))
    __list_splice(li, lh);
}

static inline void list_splice_init(plist_link li,
            plist_link lh)
{
  if (!list_is_empty(li)) {
    __list_splice(li, lh);
    INIT_LIST_HEAD(li);
  }
}

#ifdef __cplusplus
}
#endif

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
  list_link name = LIST_HEAD_INIT(name)

#define list_entry(s, m, ptr) \
  structof(s, m, ptr)

#define list_for_each(li, lh) \
  for (li = (lh)->l; prefetch(li->l), li != (lh); \
          li = li->l)

#define __list_for_each(li, lh) \
  for (li = (lh)->l; li != (lh); li = li->l)

#define list_for_each_reverse(li, lh) \
  for (li = (lh)->r; prefetch(li->r), li != (lh); \
          li = li->r)

#define list_for_each_safe(li, t, lh) \
  for (li = (lh)->l, t = li->l; li != (lh); \
    li = t, t = li->l)

#define list_for_each_entry(li, lh, type, field)        \
  for (li = list_entry(type, field, (lh)->l);  \
       prefetch(li->field.l), &li->field != (lh);  \
       li = list_entry(type, field, li->field.l))

#define list_for_each_entry_reverse(li, lh, type, field)      \
  for (li = list_entry(type, field, (lh)->r);  \
       prefetch(li->field.r), &li->field != (lh);  \
       li = list_entry(type, field, li->field.r))

#define list_exchange(lx, ly , lt)		\
	{	\
		lx->r->l = ly;	\
		ly->l->r = lx;	\
		lt = lx->l;		\
		lx->l = ly->l;	\
		ly->l = lt;		\
		lt = lx->r;		\
		lx->r = ly->r;	\
		ly->r = lt;		\
	}

/************************************************************************/
/* ------------------------ sample code --------------------------------
	typedef struct _tag_test_struct {
	int a;
	int b;
	char buf[1024];
	list_link item;
	}test_struct;
	LIST_HEAD(test_list_header);
	plist_link li = NULL;
	plist_link ly = NULL;
	test_struct *ptests;
	test_struct *ptest_seek;

	// add first item
	ptests = (test_struct *)malloc(sizeof(test_struct));
	memset(ptests , 0 , sizeof(test_struct));
	ptests->a = 10;
	ptests->b = 11;
	_snprintf(ptests->buf , sizeof(ptests->buf) , "%s" , "aaaa aaaa");
	list_add(&ptests->item , &test_list_header);
  
	// add second item
	ptests = (test_struct *)malloc(sizeof(test_struct));
	memset(ptests , 0 , sizeof(test_struct));
	ptests->a = 21;
	ptests->b = 22;
	_snprintf(ptests->buf , sizeof(ptests->buf) , "%s" , "bbbb bbbb");
	list_add(&ptests->item , &test_list_header);
	ptest_seek = ptests;

	// add third item
	ptests = (test_struct *)malloc(sizeof(test_struct));
	memset(ptests , 0 , sizeof(test_struct));
	ptests->a = 31;
	ptests->b = 32;
	_snprintf(ptests->buf , sizeof(ptests->buf) , "%s" , "cccc cccc");
	list_add(&ptests->item , &test_list_header);
	ptest_seek = ptests;

	// add forth item
	ptests = (test_struct *)malloc(sizeof(test_struct));
	memset(ptests , 0 , sizeof(test_struct));
	ptests->a = 41;
	ptests->b = 42;
	_snprintf(ptests->buf , sizeof(ptests->buf) , "%s" , "ddd dddd");
	list_add(&ptests->item , &test_list_header);

	// del the second item
	list_del(&ptest_seek->item);
	free(ptest_seek);

	// loop for get and display item
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
	*/
/************************************************************************/
#endif	//__UTILS_LIST_HEADER__

