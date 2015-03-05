#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

#ifndef M_LEXER
#define M_LEXER

#include "lexerFuncions.h"

typedef bool (*charTypeCheck)(char);

// 词法分析器
class mLexer {
private:
    vector<mToken> tokenStream;
    string errorMsg;
    string input;
    mState *readyState;
	vector<mState*> currntStates;
    map<int, int> stateIDToTokenType;
    map<int, charTypeCheck> transferCondition;
    int position;
    mToken getNext ();
    void process ();
public:
    mLexer(mState* ready, map<int, int> stateIDToTokenType, map<int, charTypeCheck> transferCondition);
    void process (const string& input);
    vector<mToken> getTokenStream ();
    void printTokenStream();
};

#endif