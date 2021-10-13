#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
        case TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_SLASH: return "TOKEN_SLASH";
        case TOKEN_STAR: return "TOKEN_STAR";
        case TOKEN_BANG: return "TOKEN_BANG";
        case TOKEN_BANG_EQUAL: return "TOKEN_BANG_EQUAL";
        case TOKEN_EQUAL: return "TOKEN_EQUAL";
        case TOKEN_EQUAL_EQUAL: return "TOKEN_EQUAL_EQUAL";
        case TOKEN_GREATER: return "TOKEN_GREATER";
        case TOKEN_GREATER_EQUAL: return "TOKEN_GREATER_EQUAL";
        case TOKEN_LESS: return "TOKEN_LESS";
        case TOKEN_LESS_EQUAL: return "TOKEN_LESS_EQUAL";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_AND: return "TOKEN_AND";
        case TOKEN_CLASS: return "TOKEN_CLASS";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_FALSE: return "TOKEN_FALSE";
        case TOKEN_FOR: return "TOKEN_FOR";
        case TOKEN_FUN: return "TOKEN_FUN";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_NIL: return "TOKEN_NIL";
        case TOKEN_OR: return "TOKEN_OR";
        case TOKEN_PRINT: return "TOKEN_PRINT";
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_SUPER: return "TOKEN_SUPER";
        case TOKEN_THIS: return "TOKEN_THIS";
        case TOKEN_TRUE: return "TOKEN_TRUE";
        case TOKEN_VAR: return "TOKEN_VAR";
        case TOKEN_WHILE: return "TOKEN_WHILE";
        case TOKEN_ERROR: return "TOKEN_ERROR";
        case TOKEN_EOF: return "TOKEN_EOF";
    }
}

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type) {
    return (Token){
        .type = type,
        .start = scanner.start,
        .length = (int)(scanner.current - scanner.start),
        .line = scanner.line,
    };
}

static Token errorToken(const char* errMsg) {
    return (Token){
        .type = TOKEN_ERROR,
        .start = errMsg,
        .length = (int)strnlen(errMsg, 256),
        .line = scanner.line,
    };
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char match(char expected) {
    if (isAtEnd()) return false;

    if (*scanner.current != expected) return false;

    scanner.current++;
    return true;
}

static inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool isAlphabetic(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static inline bool isAlphaNumeric(char c) {
    return isDigit(c) || isAlphabetic(c);
}

static void skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ': advance(); break;
            case '\r': advance(); break;
            case '\t': advance(); break;

            case '\n':
                scanner.line++;
                advance();
                break;

            // skip line comments here too
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;

            default: return;
        }
    }
}

static Token string() {
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("unterminated string");

    // consume closing quote
    advance();
    return makeToken(TOKEN_STRING);
}

static Token number() {
    // TODO handle decimals without leading 0, i.e. .123

    // don't need to check isAtEnd because peek() will return '\0'
    while (isDigit(peek())) advance();

    // look for fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        // consume the '.'
        advance();

        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static bool checkKeyword(const char* keyword, int length) {
    int keywordLength = (int)(scanner.current - scanner.start);
    if (keywordLength != length + 1) return false;

    for (int i = 0; i < length; i++) {
        if (keyword[i] != scanner.start[1 + i]) return false;
    }
    return true;
}

// Mini state machine for quickly checking whether an identifier is a keyword.
// TODO: automate generating this from a list of keywords
static TokenType checkIdentType() {
    switch (*scanner.start) {
        case 'a':
            if (checkKeyword("nd", 2)) return TOKEN_AND;
            break;
        case 'c':
            if (checkKeyword("lass", 4)) return TOKEN_CLASS;
            break;
        case 'e':
            if (checkKeyword("lse", 3)) return TOKEN_ELSE;
            break;
        case 'f':
            if (checkKeyword("alse", 4)) return TOKEN_FALSE;
            else if (checkKeyword("or", 2)) return TOKEN_FOR;
            else if (checkKeyword("un", 2)) return TOKEN_FUN;
            break;
        case 'i':
            if (checkKeyword("f", 1)) return TOKEN_IF;
            break;
        case 'n':
            if (checkKeyword("il", 2)) return TOKEN_NIL;
            break;
        case 'o':
            if (checkKeyword("r", 2)) return TOKEN_OR;
            break;
        case 'p':
            if (checkKeyword("rint", 4)) return TOKEN_PRINT;
            break;
        case 'r':
            if (checkKeyword("eturn", 5)) return TOKEN_RETURN;
            break;
        case 's':
            if (checkKeyword("uper", 4)) return TOKEN_SUPER;
            break;
        case 't':
            if (checkKeyword("his", 3)) return TOKEN_THIS;
            else if (checkKeyword("rue", 3)) return TOKEN_TRUE;
            break;
        case 'v':
            if (checkKeyword("ar", 2)) return TOKEN_VAR;
            break;
        case 'w':
            if (checkKeyword("hile", 4)) return TOKEN_WHILE;
            break;
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isAlphaNumeric(peek())) advance();
    return makeToken(checkIdentType());
}

Token scanToken() {
    skipWhitespace();

    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();

    if (isDigit(c)) return number();
    if (isAlphabetic(c)) return identifier();

    switch (c) {
        // Single-character tokens
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);

        // One or two characters
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        // String literal
        case '"': return string();
    }

    return errorToken("unexpected character");
}
