# libmgcregex
library for pattern matching with extended POSIX regular expressions.

Utilizes Thompsons Construction to implement a Linked Digraph representation
of a non-deterministic finite automaton described by the supplied regular expression.

Linear-time non-backtracking pattern matching using Power Set construction algorithm.

RE -> AST -> NFA

## build
```
      make
      sudo make install
      sudo ldconfig
      make clean
```


## use

for one-off uses, match_text() is the simplest way to method.
requiring two parameters: the pattern and the string to be searched.
```
    #include <stdio.h>
    #include <mgc/regex/regex.h>

    int main(int argc, char* argv[]) {
        char* pattern = "(a*b|ac)d";
        char* text = "aaaaabd";
        if (match_text(pattern, text)) {
            printf("Match Found!\n);
        }

        return 0;
    }
```

Because the above method handles the parsing of the regular expression and construction of the NFA
it should _not_ be used for searching multiple strings, as it will re-parse and re-build the NFA each time! 

Should you need to match multiple strings against a pattern, it is better to use re2nfa() and match_re():
```
    #include <stdio.h>
    #include <mgc/regex/regex.h>

    int main(int argc, char* argv[]) {
        char* pattern = "(a*b|ac)d";
        char* text[] = {"abd", "aaaaabd", "acd", "zzyzx"};
        re_nfa_t* nfa = re2nfa(pattern);
        for (int i = 0; i < 4; i++) {
            if (match_re(nfa, text[i])) {
                printf("Match Found!\n);
            }
        }
        return 0;
    }
```

## compile

```
    gcc prog.c -o prog -lmgcregex
```