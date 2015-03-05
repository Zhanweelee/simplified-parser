#include <vector>
using namespace std;

#include "mToken.h"

#ifndef M_ASTNODE
#define M_ASTNODE

enum NODE_TYPE {
	PROGRAM, STATEMENT_LIST, STATEMENT, FUNC_DEF, INCL_DEF, RET_TYPE, PARAM_LIST, VALUE,
	DEF_STMT, ASSIGN_STMT, RET_STMT, G_ASSIGN, G_POUND, G_VOID,
	G_EPSILON, G_INCLUDE, G_QUOTATION, G_SEMICOLON, G_ID, G_INT, G_DOUBLE, G_LEFT_PARAN, G_RIGHT_PARAN, G_RETURN,
	G_LEFT_BRACE, G_RIGHT_BRACE, G_COMMA, G_NUM, G_NEW_LINE
};

struct mASTNode {
    NODE_TYPE type;
    bool isToken;
    mToken* token;
    vector<mASTNode*> childs;
    mASTNode (NODE_TYPE type, bool istoken = false, mToken* token = NULL) : type(type), isToken(istoken), token(token) {};
};

#endif