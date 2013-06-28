/* luadec, based on luac */
#include "common.h"

#include "structs.h"

#include <assert.h>

void InitList(List * list) {
   list->head = NULL;
   list->tail = NULL;
   list->size = 0;
}

void AddToList(List * list, ListItem * item) {
   if (list==NULL || item==NULL)
	   return;
   if (list->tail)
      list->tail->next = item;
   if (!list->head)
      list->head = item;
   list->tail = item;
   list->size++;
}

void AddToListHead(List * list, ListItem * item) {
   if (list==NULL || item==NULL)
	   return;
   item->next = list->head;
   list->head = item;
   if (!list->tail)
	   list->tail = item;
   list->size++;
}

ListItem *LastItem(List * list) {
   return list->tail;
}

ListItem *PopFromList(List * list) {
   ListItem *walk, *prev;
   if (!list->tail)
      return NULL;
   prev = NULL;
   walk = list->head;
   while (walk) {
      if (walk == list->tail) {
         if (prev)
            prev->next = NULL;
         else
            list->head = NULL;
         list->tail = prev;
         list->size--;
         return walk;
      }
      prev = walk;
      walk = walk->next;
   }
   assert(0);
   return NULL;
}

void LoopList(List * list, ListItemFn fn, void* param) {
   ListItem *walk = list->head;
   while (walk) {
      ListItem *save = walk;
      walk = walk->next;
      fn(save, param);
   }
}

void DeleteList(List * list) {
   ListItem *walk = list->head;
   while (walk) {
      ListItem *save = walk;
      walk = walk->next;
      free(save);
   }
   list->head = NULL;
   list->tail = NULL;
   list->size = 0;
}

ListItem *FindInList(List * list, ListItemCmpFn cmp, const void *sample) {
   ListItem *walk = list->head;
   while (walk) {
      if (cmp(walk, sample))
         return walk;
      walk = walk->next;
   }
   return NULL;
}

ListItem *RemoveFindInList(List * list, ListItemCmpFn cmp, const void *sample) {
	ListItem *prev = NULL;
	ListItem *walk = list->head;
	while (walk) {
		if (cmp(walk, sample)){
			if (prev != NULL){
				prev->next = walk->next;
			}else{
				list->head = walk->next;
			}
			if ( list->tail == walk ){
				list->tail = prev;
			}
			list->size--;
			return walk;
		}
		prev = walk;
		walk = walk->next;
	}
	return NULL;
}

int AddToSet(IntSet * set, int a) {
   int i;
   if (! set->mayRepeat) {
      for (i = 0; i < set->ctr; i++) {
         if (set->values[i] == a)
            return 0;
      }
   }
   set->values[set->ctr] = a;
   set->ctr++;
   assert(set->ctr <= MAXARG_A);
   return 1;
}

int PeekSet(IntSet * set, int a) {
   int i;
   for (i = 0; i < set->ctr; i++) {
      if (set->values[i] == a) {
         return 1;
         break;
      }
   }
   return 0;
}

int PopSet(IntSet * set) {
   if (set->ctr == 0)
      return 0;
   set->ctr--;
   return set->values[set->ctr];
}

int RemoveFromSet(IntSet * set, int a) {
   int i;
   int at = -1;
   for (i = 0; i < set->ctr; i++) {
      if (set->values[i] == a) {
         at = i;
         break;
      }
   }
   if (at == -1)
      return 0;
   for (i = at; i < set->ctr; i++)
      set->values[i] = set->values[i + 1];
   set->ctr--;
   return 1;
}

void AddToVarStack(VarStack * stack, char* dest, char* src, int reg) {
   stack->dests[stack->ctr] = dest;
   stack->srcs[stack->ctr] = src;
   stack->regs[stack->ctr] = reg;
   stack->ctr++;
}

void ClearVarStatck(VarStack* stack){
	int i;
	for (i = 0; i < stack->ctr; i++) {
		if (stack->dests[i]){
			free(stack->dests[i]);
			stack->dests[i] = NULL;
		}
		if (stack->srcs[i]){
			free(stack->srcs[i]);
			stack->srcs[i] = NULL;
		}
	}
	stack->ctr = 0;
}

