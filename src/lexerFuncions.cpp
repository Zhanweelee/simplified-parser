#include "lexerFuncions.h"
#include <stdlib.h>

bool isLetter (char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    return false;
}

bool isDigit (char ch) {
    if (ch >= '0' && ch <= '9')
        return true;
    return false;
}

bool isOperator (char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=')
        return true;
    return false;
}

bool isWhitespace (char ch) {
    if (ch == ' ' || ch == '\t' || ch == '\n')
        return true;
    return false;
}

bool isDelimiter (char ch) {
    if (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '\n')
        return true;
    return false;
}

// 根据token的类型，对文本进行解析，得到Value
void parseTextByType (mToken* token) {
    if (token->type == NUM) {
        char buffer[256];
        for (size_t i = 0; i < token->text.size(); i ++)
            buffer[i] = token->text[i];
        token->value = atoi(buffer);
    } /* else if (token->type == DOUBLE) {

	} */
}