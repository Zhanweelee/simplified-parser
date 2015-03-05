#include <stdlib.h>
#include <stdio.h>
#include "mLexer.h"

extern void parseTextByType (mToken* token);
// extern enum mSTATE_TYPE;

// 构造函数: 初始化状态转移图和token类型映射字典
mLexer::mLexer(mState* ready, map<int, int> stateIDToTokenType, map<int, charTypeCheck> transferCondition) {
    this->position = 0;
    this->readyState = ready;
    this->currntStates.push_back(ready);
    this->stateIDToTokenType = stateIDToTokenType;
	this->transferCondition = transferCondition;
}

// 对input文本进行词法分析
void mLexer::process (const string& input) {
    this->tokenStream.clear();
    this->currntStates.clear();
    this->errorMsg.clear();
    this->currntStates.push_back(readyState);
    this->position = 0;
    this->input = input;
    process();
}

// 进行词法分析
void mLexer::process () {
    while (position < (int)input.size()) {
        mToken next = getNext();

        // 错误处理
        if (errorMsg != "") {
            printf("%s\n", errorMsg.c_str());
            return;
        }

        // 根据类型赋上token的值
        parseTextByType (&next);
        tokenStream.push_back(next);
    }
}

// 返回token流
vector<mToken> mLexer::getTokenStream () {
    return this->tokenStream;
}

// 输出token流
void mLexer::printTokenStream () {
    for (size_t i = 0; i < tokenStream.size(); ++i) {
        printf("Token: %d\ntype: %d\ntext: %s\nvalue: %lf\n\n", i, tokenStream[i].type, tokenStream[i].text.c_str(), tokenStream[i].value);
    }
}

// 产生文本流中下一个token
mToken mLexer::getNext() {
    vector<mToken> availableTokens;
    while (!currntStates.empty()) {
        if (position >= (int)input.size()) {
            int lastStateID = currntStates[0]->stateID;
            if (stateIDToTokenType.find(lastStateID) != stateIDToTokenType.end()) {
                return mToken (stateIDToTokenType[currntStates[0]->stateID], currntStates[0]->currntText);
            } else {
                errorMsg = "Error: at " + currntStates[0]->currntText + " can't transfer!\n";
                break;
            }
        }
        char nextChar = input[position++];
        vector<mState*> matchedStates;
        for (size_t count = 0; count < currntStates.size(); ++count) {
            multimap<CONDITION, mState*> generalAdjacents = currntStates[count]->generalTransfer;
            multimap<char, mState*> specifiedAdjacents = currntStates[count]->specifiedTransfer;

            vector<mState*> tempStates;

            for (multimap<CONDITION, mState*>::iterator itr = generalAdjacents.begin(); itr != generalAdjacents.end(); itr ++)
                if (transferCondition[itr->first](nextChar))
                    tempStates.push_back(itr->second);

            for (multimap<char, mState*>::iterator itr = specifiedAdjacents.begin(); itr != specifiedAdjacents.end(); itr ++)
                if (itr->first == nextChar)
                    tempStates.push_back(itr->second);

            for (size_t i = 0; i < tempStates.size(); i ++)
                if (tempStates.at(i)->stateID == FINISH) {
                    mState* save = currntStates[count];
                    availableTokens.push_back(mToken (stateIDToTokenType[save->stateID], save->currntText));
                } else if (tempStates.at(i)->stateID != IGNORE) {
                    mState* nextState = tempStates.at(i);
                    nextState->currntText = currntStates[count]->currntText;
                    nextState->currntText.push_back(nextChar);
                    matchedStates.push_back(nextState);
                }
        }
        currntStates = matchedStates;
    }
    currntStates.push_back(readyState);
    if (availableTokens.empty()) {
        return getNext();
    } else {
        position --;
        return availableTokens.back(); // 采用最长匹配策略
    }
}

