#include <vector>

using std::vector;

#include "mGrammar.h"
#include "mASTNode.h"

#ifndef M_PARSER
#define M_PARSER

class mParser {
private:
    // token������һ����ƥ���tokenλ�á�token��Ŀ
    mToken* tokenStream;
    mToken* nextToken;
    int tokenNum;

    // ������ʾ��Ϣ
    string syntaxError;
    string errMsg;
    int errPos;
	int lineCount;

    // ��¼Abstrate Syntax Tree�ĸ��ڵ�
    mASTNode* root;

    map<mGrammar*, vector<int> > First;

    // �﷨����
    mGrammar* mainGrammar;

    // ��¼��ǰƥ�䵽���ķ�
    mGrammar* next;

    // �﷨������������Ҫͬ�������ĳ�ʼ���ٵ���
    void process();

    // �Ƿ���debugģʽ
    bool debug;

public:
	// �����ķ���First��
	void computeFirst (mGrammar* expr);
	// ����debugģʽ
	void setDebug (bool);
    // ����debug״̬
    bool isDebug ();
public:
    // ��ʼ���﷨����
    mParser(mGrammar* grammar);
    // ��token�������﷨����
    bool process (vector<mToken> token_stream);
    // ��ȡAST���ڵ�
    mASTNode* getASTree();
    void printTree (mASTNode* root, string prefix);
    void printError();
};

#endif