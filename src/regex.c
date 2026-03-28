#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parseregex.h"
#include "pattern_match.h"
#include "nfa.h"

bool match_text(char* pattern, char* text) {
    if (pattern == NULL || text == NULL) {
        return false;
    }
    re_nfa_t* nfa = re2nfa(pattern);
    bool result = match_re(nfa, text);
    return result;
}
