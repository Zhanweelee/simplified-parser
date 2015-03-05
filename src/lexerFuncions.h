
#ifndef LEXER_FUNCTIONS
#define LEXER_FUNCTIONS

/* ���������� */
extern bool isLetter (char ch);
extern bool isDigit (char ch);
extern bool isOperator (char ch);
extern bool isWhitespace (char ch);
extern bool isDelimiter (char ch);

#endif

#include "mToken.h"
#include "mState.h"
