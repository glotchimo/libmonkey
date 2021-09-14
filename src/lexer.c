#include "lexer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

Lexer *lexer_new(char *input) {
    Lexer *l = (Lexer *)malloc(sizeof(Lexer));
    l->input = input;
    lexer_read_char(l);
    return l;
}

Token lexer_next_token(Lexer *l) {
    Token t;

    lexer_skip_whitespace(l);

    switch (l->ch) {
    case '=':
        if (lexer_peek_char(l) == '=') {
            char ch = l->ch;
            lexer_read_char(l);
            char *literal = (char *)malloc(2);
            literal[0] = ch;
            literal[1] = l->ch;
            t.type = EQ;
            t.literal = literal;
        } else {
            set_token(&t, ASSIGN, l->ch);
        }
        break;

    case '+':
        set_token(&t, PLUS, l->ch);
        break;

    case '-':
        t.type = MINUS;
        t.literal = &l->ch;
        break;

    case '!':
        if (lexer_peek_char(l) == '=') {
            char ch = l->ch;
            lexer_read_char(l);
            char *literal = (char *)malloc(2);
            literal[0] = ch;
            literal[1] = l->ch;
            t.type = NOT_EQ;
            t.literal = literal;
        } else {
            set_token(&t, BANG, l->ch);
        }
        break;

    case '/':
        set_token(&t, SLASH, l->ch);
        break;

    case '*':
        set_token(&t, ASTERISK, l->ch);
        break;

    case '<':
        set_token(&t, LT, l->ch);
        break;

    case '>':
        set_token(&t, GT, l->ch);
        break;

    case ';':
        set_token(&t, SEMICOLON, l->ch);
        break;

    case ',':
        set_token(&t, COMMA, l->ch);
        break;

    case '{':
        set_token(&t, LBRACE, l->ch);
        break;

    case '}':
        set_token(&t, RBRACE, l->ch);
        break;

    case '(':
        set_token(&t, LPAREN, l->ch);
        break;

    case ')':
        set_token(&t, RPAREN, l->ch);
        break;

    case 0:
        t.type = EoF;
        t.literal = "";
        break;

    default:
        if (is_letter(l->ch)) {
            char *literal = lexer_read_ident(l);
            t.type = lookup_ident(literal);
            t.literal = literal;
            return t;
        } else if (is_digit(l->ch)) {
            t.type = INT;
            t.literal = lexer_read_num(l);
            return t;
        } else {
            set_token(&t, ILLEGAL, l->ch);
        }
    }

    lexer_read_char(l);

    return t;
}

void lexer_read_char(Lexer *l) {
    if (l->readPosition >= strlen(l->input)) {
        l->ch = 0;
    } else {
        l->ch = l->input[l->readPosition];
    }

    l->position = l->readPosition;
    l->readPosition = l->readPosition + 1;
}

char *lexer_read_ident(Lexer *l) {
    int position = l->position;

    while (is_letter(l->ch)) {
        lexer_read_char(l);
    }

    char *slice = (char *)malloc(sizeof(l->input) + 1);
    for (int i = position; i < l->position; i++) {
        slice[i - position] = l->input[i];
    }
    slice[sizeof(l->input)] = '\0';

    return slice;
}

char *lexer_read_num(Lexer *l) {
    int position = l->position;

    while (is_digit(l->ch)) {
        lexer_read_char(l);
    }

    char *slice = (char *)malloc(sizeof(l->input));
    for (int i = position; i < l->position; i++) {
        slice[i - position] = l->input[i];
    }

    return slice;
}

char lexer_peek_char(Lexer *l) {
    if (l->readPosition >= sizeof(l->input)) {
        return 0;
    } else {
        return l->input[l->readPosition];
    }
}

void lexer_skip_whitespace(Lexer *l) {
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
        lexer_read_char(l);
    }
}

bool is_letter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

void set_token(Token *token, TokenType type, char ch) {
    char *literal = (char *)malloc(1);
    literal[0] = ch;
    token->type = type;
    token->literal = literal;
}