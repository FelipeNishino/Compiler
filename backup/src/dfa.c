#include <stdio.h>
#include <stdlib.h>
#include "include/dfa.h"

void transition_insert(TransitionNode *list, Transition transition) {
    //create a link
    TransitionNode *link = (TransitionNode*) malloc(sizeof(TransitionNode));

//    link->key = key;
    link->transition = transition;

    //point it to old first node
    link->next = list;

    //point first to new first node
    list = link;
}

int transition_isEmpty(TransitionNode *list) {
   return list == NULL; 
}

int transition_isEqual(Transition t1, Transition t2) {
    return t1.source == t2.source && t1.read == t2.read && t1.target == t2.target;
}

TransitionNode* transition_find(TransitionNode *list, Transition transition) {
    //start from the first link
   TransitionNode* current = list;

   //if list is empty
   if(transition_isEmpty(list)) {
      return NULL;
   }

   //navigate through list
   while(!transition_isEqual(current->transition, transition)) {
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
    
   //if data found, return the current Link
   return current;
}

//insert link at the first location
DFA *dfa_insertState(DFA *dfa, State state) {
    //create a link
    DFA *link = (DFA*) malloc(sizeof(DFA));

//    link->key = key;
    link->state = state;

    //point it to old first node
    link->next = dfa;

    //point first to new first node
    dfa = link;
    return dfa;
}

//is list empty
int dfa_isEmpty(DFA *dfa) {
    return dfa == NULL;
}

//find a link with given key
DFA* dfa_find(DFA *dfa, int id) {

   //start from the first link
   DFA* current = dfa;

   //if list is empty
   if(dfa_isEmpty(dfa)) {
      return NULL;
   }

   //navigate through list
   while(current->state.id != id) {
    
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
    
   //if data found, return the current Link
   return current;
}

int dfa_length(DFA *dfa) {
    int length = 0;
    DFA *current;

    for(current = dfa; current != NULL; current = current->next) {
        length++;
    }

    return length;
}

void dfa_printList(DFA *dfa) {
    DFA *ptr = dfa;
    printf("\n[ ");

    //start from the beginning
    while(ptr != NULL) {
        printf("State; id {%d}, isTerminal {%d}\n", ptr->state.id, ptr->state.isTerminal);
        ptr = ptr->next;
    }

    printf(" ]\n");
}

DFA *dfa_createMachine() {
    int i;
    DFA *dfa = NULL;
    DFA *auxDFA;
    // printf("Creating machine from transitions defined in dfa.h\n");
    for(i = 0; i < N_TRANSITIONS; i++) {
        // printf("Checking if state for transition with src {%d} already exists\n", DEFAULT_TRANSITIONS[i].source);
        auxDFA = dfa_find(dfa, DEFAULT_TRANSITIONS[i].source);
        if(!auxDFA){
            // printf("State {%d} doesn't exist yet, creating and inserting...\n", DEFAULT_TRANSITIONS[i].source);
            dfa = dfa_insertState(dfa, (State) {
                .id = DEFAULT_TRANSITIONS[i].source,
                .isTerminal = DEFAULT_TRANSITIONS[i].source < 6 && DEFAULT_TRANSITIONS[i].source > 0,
                // .transitions = NULL}
                });
        }

        // printf("Checking if state for transition with target {%d} already exists\n", DEFAULT_TRANSITIONS[i].target);
        auxDFA = dfa_find(dfa, DEFAULT_TRANSITIONS[i].target);
        if(!auxDFA){
            // printf("State {%d} doesn't exist yet, creating and inserting...\n", DEFAULT_TRANSITIONS[i].target);
            dfa = dfa_insertState(dfa, (State) {
                    .id = DEFAULT_TRANSITIONS[i].target,
                    .isTerminal = DEFAULT_TRANSITIONS[i].target < 6 && DEFAULT_TRANSITIONS[i].target > 0,
                    // .transitions = NULL}
            });
        }
    }
    return dfa;
}
