/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese <bostjan@a2o.si>
 *
 * Inspired by: https://github.com/zedshaw/liblcthw/blob/master/src/lcthw/list.h & dbh.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 * Type definitions
 */
struct listNode_t;

typedef struct listNode_t {
    struct listNode_t *next;
    struct listNode_t *prev;
    void *value;
} listNode_t;

typedef struct list_t {
    int count;
    listNode_t *first;
    listNode_t *last;
} list_t;



/*
 * List handling functions
 */
int             snoopy_util_list_push           (list_t * list, void * newNodeValue);
void *          snoopy_util_list_remove         (list_t * list, listNode_t * nodeToRemove);
listNode_t *    snoopy_util_list_fetchNextNode  (list_t * list, listNode_t * curNode);
