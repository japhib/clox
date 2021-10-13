#include "scanner.h"

#include <stdbool.h>
#include <string.h>

void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isAtEnd() { return *scanner.current == '\0'; }

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

static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

static char match(char expected) {
  if (isAtEnd()) {
    return false;
  }

  if (*scanner.current != expected) {
    return false;
  }

  scanner.current++;
  return true;
}

Token scanToken() {
  scanner.start = scanner.current;

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();

  switch (c) {
    // Single-character tokens
    case '(':
      return makeToken(TOKEN_LEFT_PAREN);
    case ')':
      return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
      return makeToken(TOKEN_LEFT_BRACE);
    case '}':
      return makeToken(TOKEN_RIGHT_BRACE);
    case ';':
      return makeToken(TOKEN_SEMICOLON);
    case ',':
      return makeToken(TOKEN_COMMA);
    case '.':
      return makeToken(TOKEN_DOT);
    case '-':
      return makeToken(TOKEN_MINUS);
    case '+':
      return makeToken(TOKEN_PLUS);
    case '/':
      return makeToken(TOKEN_SLASH);
    case '*':
      return makeToken(TOKEN_STAR);

    // One or two characters
    case '!':
      return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  }

  return errorToken("unexpected character");
}
