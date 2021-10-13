#include "compiler.h"

#include <stdio.h>

#include "common.h"
#include "scanner.h"

void compile(const char* source) {
    initScanner(source);
    int line = -1;
    while (true) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%-20s '%.*s'\n", getTokenTypeName(token.type), token.length, token.start);

        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) break;
    }
}
