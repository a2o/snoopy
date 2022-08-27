/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese <bostjan@a2o.si>
 *
 * Inspired by: https://github.com/zedshaw/liblcthw/blob/master/src/lcthw/list.c
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
 * Includes order: from local to global
 */
#include "util/list-snoopy.h"

#include "snoopy.h"
#include "error.h"

#include <stdlib.h>



/*
 * Push a value onto the doubly linked list, as a last node
 *
 * Params:
 *     list_t:          A list to work with
 *     void *:          Pointer to the new node's value
 *
 * Return:
 *     int:             SNOOPY_SUCCESS or SNOOPY_ERROR
 */
int snoopy_util_list_push (list_t * list, void * newNodeValue)
{
    listNode_t * newNode;

    // Create a new node
    newNode = calloc(1, sizeof(listNode_t));
    if (!newNode) {
        snoopy_error_handler("Unable to allocate memory for a new doubly linked list node");
        return SNOOPY_ERROR;
    }
    newNode->value = newNodeValue;

    // Place it at the end of the list
    if (list->last == NULL) {
        list->first = newNode;
        list->last = newNode;
        newNode->prev = NULL;
        newNode->next = NULL;

    } else {
        list->last->next = newNode;
        newNode->prev = list->last;
        newNode->next = NULL;
        list->last = newNode;
    }

    // Update the list count
    list->count++;

    return SNOOPY_SUCCESS;
}



/*
 * Remove a node from a doubly-linked list
 *
 * Params:
 *     list_t:          A list to work with
 *     listNode_t:      A node to remove
 *
 * Return:
 *     void *:          Value pointer of a removed node, or NULL on failure
 */
void * snoopy_util_list_remove (list_t * list, listNode_t * nodeToRemove)
{
    void * retVal = NULL;

    // Check the list and the node
    if (!list->first || !list->last) {
        snoopy_error_handler("The doubly linked list is empty");
        return NULL;
    }
    if (!nodeToRemove) {
        snoopy_error_handler("No node given, unable to remove NULL");
        return NULL;
    }

    // Remove the node from the list
    if (nodeToRemove == list->first && nodeToRemove == list->last) {
        list->first = NULL;
        list->last = NULL;

    } else if (nodeToRemove == list->first) {
        list->first = nodeToRemove->next;

    } else if (nodeToRemove == list->last) {
        list->last = nodeToRemove->prev;
        list->last->next = NULL;

    } else {
        listNode_t * nodeAfter = nodeToRemove->next;
        listNode_t * nodeBefore = nodeToRemove->prev;
        nodeAfter->prev = nodeBefore;
        nodeBefore->next = nodeAfter;
    }

    // Update the list count
    list->count--;

    // Prepare the return value
    retVal = nodeToRemove->value;

    // Cleanup
    free(nodeToRemove);

    return retVal;
}



/*
 * Fetch the next node on a list (or the first node if curNode == NULL)
 *
 * Params:
 *     list_t:          A list to work with
 *     listNode_t:      Node before the expected one (or NULL)
 *
 * Return:
 *     listNode_t *:    Next node, or NULL if list's end has been reached
 */
listNode_t * snoopy_util_list_fetchNextNode (list_t * list, listNode_t * curNode)
{
    // If the list is empty, there is nothing to fetch
    if (!list->first || !list->last) {
        return NULL;
    }

    // If curNode == NULL, return the first node
    if (!curNode) {
        return list->first;
    }

    // Else simply return the next node (curNode->next is NULL when curNode is the last node on the list)
    return curNode->next;
}
