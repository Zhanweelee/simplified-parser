#include <vector>
#include <map>
using std::vector;
using std::map;

#include "mToken.h"
#include "mASTNode.h"

#ifndef M_GRAMMAR
#define M_GRAMMAR

struct mGrammar {
    NODE_TYPE type;
    bool isTerminal;
    mToken* token;
    bool isRaw;
    vector<mGrammar* > subGrammars;

    // ��¼��mToken Type���ķ���ӳ��
    map<int, vector<mGrammar*> > parserTable;
	
	mGrammar(NODE_TYPE type, bool terminal = false, mToken* token = NULL): type(type), isTerminal(terminal), token(token) { isRaw = true; };
};

#endif