#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <list>

using namespace std;

#include "mParser.h"

// ��ʼ���﷨����
mParser::mParser(mGrammar* exprSyntax) {
    this->mainGrammar = exprSyntax;
    this->root = NULL;
    this->debug = false;
	this->syntaxError = "Error: You have an error in your syntax.";
}

// ����debugģʽ
void mParser::setDebug (bool d) {
    this->debug = d;
}

bool mParser::isDebug () {
    return this->debug;
}

// ��token�������﷨����
bool mParser::process (vector<mToken> tokens) {
    this->root = NULL;
	this->lineCount = 0;
    this->tokenNum = tokens.size();
    this->tokenStream = new mToken[tokenNum+1];
    for (size_t i = 0; i < tokenNum; i ++)
        tokenStream[i] = tokens[i];
    nextToken = tokenStream;
    process ();
    if (errPos != -1) {
        cout <<syntaxError <<" '" <<errMsg <<"' at line " <<lineCount;
		int columnCount = 0;
		mToken* p = tokenStream;
        for (p; p < nextToken; ++ p) {
			if (p->type == NL)
				columnCount = 0;
			else
                columnCount += p->text.size();
		}
		cout << ", at column " <<columnCount <<"." <<endl;
		return false;
    }
	return true;
}
    
// �﷨������������Ҫͬ�������ĳ�ʼ���ٵ���
void mParser::process () {
    errPos = -1;
    stack<mGrammar*> pStack;
    pStack.push(mainGrammar);
    list<mASTNode*> nList;
    this->root = new mASTNode((NODE_TYPE)0);
    nList.push_back(root);
    while (nextToken < tokenStream + tokenNum) {
        int tokenType = nextToken->type;
		
		if (tokenType == NL) {
			++ lineCount;
			++ nextToken;
			continue;
		}

        mASTNode* currnt = nList.front();
        nList.pop_front();
        mGrammar* top = pStack.top();
        pStack.pop();

        if (top->isTerminal == false) {
            if (top->isRaw == false) {
				bool isFoundGrammar = false, hasEpsilon = false;
                for (size_t i = 0; i < top->subGrammars.size(); i ++) {
                    mGrammar* sub = top->subGrammars.at(i);
                    if (First.find(sub) == First.end())
                        computeFirst(sub);
                    if (find(First[sub].begin(), First[sub].end(), tokenType) != First[sub].end()) {
                        pStack.push(sub);
                        mASTNode* node = new mASTNode (sub->type, sub->isTerminal, sub->token);
                        currnt->childs.insert(currnt->childs.begin(), node);;
                        nList.push_front(node);
                        isFoundGrammar = true;
						break;
                    }

					if (find(First[sub].begin(), First[sub].end(), EPSILON) != First[sub].end()) {
						hasEpsilon = true;
					}
                }

				if (isFoundGrammar == false && hasEpsilon == false) {
					errMsg = nextToken->text;
					errPos = nextToken - tokenStream;
					break;
				}
            } else {
                if (First.find(top) == First.end())
                    computeFirst(top);
                if (find(First[top].begin(), First[top].end(), tokenType) != First[top].end()) {
                    for (int i = top->subGrammars.size()-1; i >= 0; i --) {
                        mGrammar* sub = top->subGrammars.at(i);
                        pStack.push(sub);
                        mASTNode* node = new mASTNode (sub->type, sub->isTerminal, sub->token);
                        currnt->childs.insert(currnt->childs.begin(), node);
                        nList.push_front(node);
                    }
                } else {
                    errMsg = nextToken->text;
                    errPos = nextToken - tokenStream;
                    break;
                }
            }
        } else {
            if (top->token->type == tokenType) {
                if (this->debug) {
                    cout <<"Token: " <<tokenType <<endl <<"Position: " <<(int)(nextToken - tokenStream) <<endl <<endl;
                }
                currnt->token = nextToken;
                currnt->isToken = true;
                nextToken ++;
            } else {
                errMsg = nextToken->text;
                errPos = nextToken - tokenStream;
                break;
            }
        }
    }

    if (!pStack.empty()) {
        errPos = nextToken - tokenStream;
    } else if (this->debug) {
        cout <<"Success at generating Parser tree!" <<endl;
    }
}

//// ��ʼ��Sentence�ķ������Ƶ�ʽ��First����Follw��
//void mParser::computeParserTable (mGrammar* expr) {
//    for (size_t i = 0; i < mTOKEN_NUM; i ++) {
//
//    }
//}


// �����ķ���First��
void mParser::computeFirst (mGrammar* expr) {
    if (expr->isTerminal) {
        First[expr].push_back (expr->token->type);
        return;
    }

        for (size_t j = 0; j < expr->subGrammars.size(); j ++) {
		    mGrammar* sub = expr->subGrammars.at(j);
			if (expr->isRaw == false && expr->type != sub->type) {
                computeFirst (sub);
			    for (size_t k = 0; k < First[sub].size(); k ++)
                    if (find(First[expr].begin(), First[expr].end(), First[sub].at(k)) == First[expr].end())
                        First[expr].push_back(First[sub].at(k));
                continue;
            }

		    if (sub->isTerminal) {
                int tokenType = sub->token->type;
                if (find(First[expr].begin(), First[expr].end(), tokenType) == First[expr].end())
                    First[expr].push_back(tokenType);
            } else {
			    computeFirst (sub);
			    for (size_t k = 0; k < First[sub].size(); k ++)
                    if (find(First[expr].begin(), First[expr].end(), First[sub].at(k)) == First[expr].end())
                        First[expr].push_back(First[sub].at(k));
		    }

            // ����X=Y1Y2Y3����Yk������Yi��First�а���epsilon�����Yi+1Yi+2����Yk��First����X��First��
            bool hasEpsilon = false;
            for (size_t k = 0; k < First[sub].size(); k ++) {
                if (First[sub].at(k) == EPSILON) {
                    hasEpsilon = true;
                    break;
                }
            }

            // �����һ���Ƶ�ʽ��First��û��epsilon�����Ȳ���������������
            if (hasEpsilon == false)
                break;
        }
}

// ��ȡAST���ڵ�
mASTNode* mParser::getASTree() {
    return this->root;
}


void mParser::printTree (mASTNode* root, string prefix) {
    for (size_t i = 0; i < root->childs.size(); i ++) {
        mASTNode* node = root->childs[i];
        int size = root->childs.size();
         if (i + 1 >= size) {
            if (node->isToken == false) {
                cout <<prefix <<"��" <<node->type <<endl;
                printTree(node, prefix + "    ");
            } else {
                cout <<prefix <<"��" <<"Token ('" <<node->token->text <<"')" <<endl;
            }
        } else if (i == 0) {
            if (node->isToken == false) {
                cout <<prefix <<"��" <<node->type <<endl;
                printTree(node, prefix + "��  ");
            } else {
                cout <<prefix <<"��" <<"Token ('" <<node->token->text <<"')" <<endl;
            }
        } else {
            if (node->isToken == false) {
                cout <<prefix <<"��" <<node->type <<endl;
                printTree(node, prefix + "��  ");
            } else {
                cout <<prefix <<"��" <<"Token ('" <<node->token->text <<"')" <<endl;
            }
        }
            
    }

}