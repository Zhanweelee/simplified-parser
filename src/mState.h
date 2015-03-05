#include <string>
#include <map>
using namespace std;

#ifndef M_STATE
#define M_STATE

// 定义state的类型
enum mSTATE_TYPE {
	READY, IGNORE, FINISH, ERROR,

	// 这里继续填入你的状态
	INTEGER, IDENTIFIER,
	M_AIN, MA_IN, MAI_N, MAIN,
	I_NCLUDE, IN_CLUDE, INC_LUDE, INCL_UDE, INCLU_DE, INCLUD_E, INCLUDE,
	I_NT, IN_T, INT, V_OID, VO_ID, VOI_D, VOID,
	D_OUBLE, DO_UBLE, DOU_BLE, DOUB_LE, DOUBL_E, DOUBLE,
	R_ETURN, RE_TURN, RET_URN, RETU_RN, RETUR_N, RETURN,
	COMMA, SEMICOLON, LEFT_BRACE, RIGHT_BRACE, ASSIGN, QUOTATION, POUND,
	LEFT_PAREM, RIGHT_PAREN, NEW_LINE,

	/* 保留状态READY, IGNORE, FINISH, ERROR */
	/* 最后的不修改，用于计数 */
	mSTATE_NUM
};

// 作为通配条件的转移标志
enum CONDITION { IS_LETTER, IS_DIGIT, IS_OPERATOR, IS_WHITESPACE, IS_DELIMITER, UNKNOWN };

struct mState {
    int stateID;
    bool isEnd;
    string currntText;
    multimap<CONDITION, mState*> generalTransfer;
    multimap<char, mState*> specifiedTransfer;
    mState& push(CONDITION condition, mState* state) { generalTransfer.insert(make_pair(condition, state)); return *this; }
    mState& push(char ch, mState* state) { specifiedTransfer.insert(make_pair(ch, state)); return *this; }

    mState (int id = -1, string text = "", bool end = false): stateID(id), currntText(text), isEnd(end) {};
};

#endif