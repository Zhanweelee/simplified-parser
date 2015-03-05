#include <string>
#include <map>
using namespace std;

#include "mLexer.h"

mLexer* mLexerInit() {
    mState* states = new mState[mSTATE_NUM];
    for (int i = 0; i < mSTATE_NUM; i ++)
		states[i].stateID = i;
	
	/*** -------------------------------------------- ***/
	/* 构造状态转移图 */
	// 状态READY
    states[READY].push(IS_DIGIT, &states[INTEGER])
		.push(IS_LETTER, &states[IDENTIFIER])
		.push('v', &states[V_OID])
        .push('i', &states[I_NCLUDE])
		.push('i', &states[I_NT])
		.push('d', &states[D_OUBLE])
		.push('r', &states[R_ETURN])
        .push('=', &states[ASSIGN])
		.push(',', &states[COMMA])
		.push('"', &states[QUOTATION])
		.push(';', &states[SEMICOLON])
        .push('{', &states[LEFT_BRACE])
		.push('}', &states[RIGHT_BRACE])
        .push('(', &states[LEFT_PAREM])
		.push(')', &states[RIGHT_PAREN])
		.push('#', &states[POUND])
		.push('\n', &states[NEW_LINE])
		.push(IS_WHITESPACE, &states[IGNORE]);
	
    states[INTEGER].push(IS_LETTER, &states[ERROR])
		.push(IS_DIGIT, &states[INTEGER])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[IDENTIFIER].push(IS_LETTER, &states[IDENTIFIER])
		.push(IS_DIGIT, &states[IDENTIFIER])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH])
		.push('\"', &states[FINISH])
		;

    states[I_NCLUDE].push('n', &states[IN_CLUDE]);
    states[IN_CLUDE].push('c', &states[INC_LUDE]);
    states[INC_LUDE].push('l', &states[INCL_UDE]);
    states[INCL_UDE].push('u', &states[INCLU_DE]);
    states[INCLU_DE].push('d', &states[INCLUD_E]);
    states[INCLUD_E].push('e', &states[INCLUDE]);
	states[INCLUDE].push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[I_NT].push('n', &states[IN_T]);
    states[IN_T].push('t', &states[INT]);
	states[INT].push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[V_OID].push('o', &states[VO_ID]);
    states[VO_ID].push('i', &states[VOI_D]);
    states[VOI_D].push('d', &states[VOID]);
	states[VOID].push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[D_OUBLE].push('o', &states[DO_UBLE]);
	states[DO_UBLE].push('u', &states[DOU_BLE]);
	states[DOU_BLE].push('b', &states[DOUB_LE]);
	states[DOUB_LE].push('l', &states[DOUBL_E]);
	states[DOUBL_E].push('e', &states[DOUBLE]);
	states[DOUBLE].push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

    states[R_ETURN].push('e', &states[RE_TURN]);
    states[RE_TURN].push('t', &states[RET_URN]);
    states[RET_URN].push('u', &states[RETU_RN]);
    states[RETU_RN].push('r', &states[RETUR_N]);
    states[RETUR_N].push('n', &states[RETURN]);
	states[RETURN].push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[ASSIGN].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	
	states[LEFT_BRACE].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[RIGHT_BRACE].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	states[LEFT_PAREM].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	states[RIGHT_PAREN].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[COMMA].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	states[SEMICOLON].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[QUOTATION].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);
	
	states[POUND].push(IS_LETTER, &states[FINISH])
		.push(IS_DIGIT, &states[FINISH])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	states[NEW_LINE].push(IS_LETTER, &states[INTEGER])
		.push(IS_DIGIT, &states[INTEGER])
		.push(IS_OPERATOR, &states[FINISH])
		.push(IS_WHITESPACE, &states[FINISH])
		.push(IS_DELIMITER, &states[FINISH]);

	/*** -------------------------------------------- ***/
	/* 状态和token之间对应关系 */
	map<int, int> stateIDToTokenType;
	stateIDToTokenType[INTEGER] = NUM;
	stateIDToTokenType[IDENTIFIER] = ID;

	stateIDToTokenType[INCLUDE] = INC;
	stateIDToTokenType[INT] = IN;
	stateIDToTokenType[DOUBLE] = DO;
	stateIDToTokenType[VOID] = VO;
	stateIDToTokenType[RETURN] = RET;
	
    stateIDToTokenType[LEFT_BRACE] = LB;
    stateIDToTokenType[RIGHT_BRACE] = RB;
    stateIDToTokenType[LEFT_PAREM] = LP;
    stateIDToTokenType[RIGHT_PAREN] = RP;
	stateIDToTokenType[QUOTATION] = QU;
	stateIDToTokenType[ASSIGN] = AS;
	stateIDToTokenType[COMMA] = COM;
	stateIDToTokenType[SEMICOLON] = SE;
	stateIDToTokenType[POUND] = PO;
	stateIDToTokenType[NEW_LINE] = NL;

	
	/*** -------------------------------------------- ***/
	/* 通配转移条件 */
	map<int, charTypeCheck> transferCondition;
	transferCondition[IS_LETTER]= isLetter;
	transferCondition[IS_DIGIT]= isDigit;
	transferCondition[IS_OPERATOR]= isOperator;
	transferCondition[IS_WHITESPACE]= isWhitespace;
	transferCondition[IS_DELIMITER]= isDelimiter;

	mLexer* lexer = new mLexer(states, stateIDToTokenType, transferCondition);
    return lexer;
}
