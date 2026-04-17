#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pattern_match.h"
#include "set.h"

bool checkRange(char check, char low, char high) {
    return check >= low && check <= high;
}

bool match_ccl(char ch, re_nfa_transition_t* trans) {
    bool is_compliment = trans->data[0] == '^';
    for (int i = 0; trans->data[i]; i++) {
        if (trans->data[i] == '-' && trans->data[i+1] != '\0') {
            if (checkRange(ch, trans->data[i-1], trans->data[i+1]))
                return true;
        }
        if (trans->data[i] == ch) 
            return true;
    }
    return false;
}

OrderedSet* move(char ch, OrderedSet* states) {
    OrderedSet* next = malloc(sizeof(OrderedSet));
    init_set(next, states->cmpfunc);
    RBIterator it;
    rb_iter_init(&it, states);
    for (; !rb_iter_done(&it); rb_iter_next(&it)) {
        for (int i = 0; i < 2; i++) {
            re_nfa_transition_t* trans = ((re_nfa_state_t*)rb_iter_get(&it)->value)->trans[i];
            if (trans != NULL && trans->is_epsilon == false) {
                if (trans->is_ccl) {
                    if (match_ccl(ch, trans)) {
                        setAdd(next, trans->dest);
                    }
                } else if (trans->data[0] == ch || trans->data[0] == '.') {
                    setAdd(next, trans->dest);
                }
            }
        }
    }
    return next;
}


OrderedSet* e_closure(OrderedSet* states) {
    OrderedSet* next = states;
    re_nfa_state_t* ss[255];
    int st = 0;
    RBIterator it;
    rb_iter_init(&it, states);
    for (; !rb_iter_done(&it); rb_iter_next(&it)) {
        ss[st++] = ((re_nfa_state_t*)rb_iter_get(&it)->value);
    }
    while (st > 0) {
        re_nfa_state_t* curr = ss[--st];
        for (int i = 0; i < 2; i++) {
            re_nfa_transition_t* trans = curr->trans[i];
            if (trans != NULL && trans->is_epsilon) {
                setAdd(next, trans->dest);
                ss[st++] = trans->dest;
            }
        }
    }
    return next;
}

int cmp_states(void* l, void* r) {
    re_nfa_state_t* a = (re_nfa_state_t*)l;
    re_nfa_state_t* b = (re_nfa_state_t*)r;
    if (a->label < b->label) return -1;
    if (a->label > b->label) return 1;
    return 0;
}

bool match_re(re_nfa_t* nfa, char* text) {
    OrderedSet *states = malloc(sizeof(OrderedSet));
    init_set(states, &cmp_states);
    setAdd(states, nfa->start);
    states = e_closure(states);
    bool did_find = false;
    int match_from = 0;
    int match_len = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        states = move(text[i], states);
        states = e_closure(states);
        if (setEmpty(states))
            return false;
        if (setContains(states, nfa->accept)) {
            did_find = true;
            match_len = i;
        }
    }
    return did_find;
}