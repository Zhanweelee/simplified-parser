#include <string>
#include <map>
using namespace std;

#include "mLexer.h"
#include "mParser.h"
#include "mGrammar.h"

mParser* mParserInit() {
	mGrammar* program = new mGrammar(PROGRAM);
	mGrammar* program_ = new mGrammar(PROGRAM);
	mGrammar* program_1 = new mGrammar(PROGRAM);

	mGrammar* statement_list = new mGrammar(STATEMENT_LIST);
	mGrammar* statement_list_ = new mGrammar(STATEMENT_LIST);
	mGrammar* statement_list_1 = new mGrammar(STATEMENT_LIST);
	mGrammar* statement_list_2 = new mGrammar(STATEMENT_LIST);
	mGrammar* statement = new mGrammar(STATEMENT);

	mGrammar* value = new mGrammar(VALUE);
	mGrammar* def_stmt = new mGrammar(DEF_STMT);
	mGrammar* assign_stmt = new mGrammar(ASSIGN_STMT);
	mGrammar* ret_stmt = new mGrammar(RET_STMT);
	mGrammar* func_def = new mGrammar(FUNC_DEF);
	mGrammar* incl_def = new mGrammar(INCL_DEF);

	mGrammar* ret_type = new mGrammar(RET_TYPE);
	mGrammar* param_list = new mGrammar(PARAM_LIST);

	mGrammar* t_pound = new mGrammar(G_POUND, true, new mToken(PO));
	mGrammar* t_double = new mGrammar(G_DOUBLE, true, new mToken(DO));
	mGrammar* t_int = new mGrammar(G_INT, true, new mToken(IN));
	mGrammar* t_include = new mGrammar(G_INCLUDE, true, new mToken(INC));
	mGrammar* t_quotation = new mGrammar(G_QUOTATION, true, new mToken(QU));
	mGrammar* t_semicolon = new mGrammar(G_SEMICOLON, true, new mToken(SE));
	mGrammar* t_id = new mGrammar(G_ID, true, new mToken(ID));
	mGrammar* t_void = new mGrammar(G_VOID, true, new mToken(VO));
	mGrammar* t_num = new mGrammar(G_NUM, true, new mToken(NUM));
	mGrammar* t_assign = new mGrammar(G_ASSIGN, true, new mToken(AS));
	mGrammar* t_left_paran = new mGrammar(G_LEFT_PARAN, true, new mToken(LP));
	mGrammar* t_right_paran = new mGrammar(G_RIGHT_PARAN, true, new mToken(RP));
	mGrammar* t_left_brace = new mGrammar(G_LEFT_BRACE, true, new mToken(LB));
	mGrammar* t_right_brace = new mGrammar(G_RIGHT_BRACE, true, new mToken(RB));
	mGrammar* t_comma = new mGrammar(G_COMMA, true, new mToken(COM));
	mGrammar* t_new_line = new mGrammar(G_NEW_LINE, true, new mToken(NL));
	mGrammar* t_epsilon = new mGrammar(G_EPSILON, true, new mToken(EPSILON));
	mGrammar* t_return = new mGrammar(G_RETURN, true, new mToken(RET));

	/*
	 * program -> epsilon | program_ program
	 * program_ -> func_def | incl_def
	 */
	program->isRaw = false;
	program->subGrammars.push_back(t_epsilon);
	program->subGrammars.push_back(program_);

	program_->subGrammars.push_back(program_1);
	program_->subGrammars.push_back(program);

	program_1->isRaw = false;
	program_1->subGrammars.push_back(incl_def);
	program_1->subGrammars.push_back(func_def);

	/*
	 * ret_type -> int | double
	 */
	ret_type->isRaw = false;
	ret_type->subGrammars.push_back(t_int);
	ret_type->subGrammars.push_back(t_double);
	ret_type->subGrammars.push_back(t_void);


	/*
	 * param_list -> t_void | ret_type id param_list_
	 * param_list_ -> epsilon | , ret_type id param_list_
	 */
	param_list->subGrammars.push_back(t_void);


	/*
	 * statement_list -> statement statement_list_
	 * statement_list_ -> statement statement_list_ | epsilon
	 * 
	 * statement -> def_stmt | assign_stmt | ret_stmt | epsilon
	 * def_stmt -> ret_type id = num;
	 * assign_stmt -> id = value;
	 * value -> num | id
	 * ret_stmt -> return value;
	 */
	statement_list->subGrammars.push_back(statement);
	statement_list->subGrammars.push_back(statement_list_);

	statement_list_->isRaw = false;
	statement_list_->subGrammars.push_back(statement_list_1);
	statement_list_->subGrammars.push_back(statement_list_2);

	statement_list_1->subGrammars.push_back(statement);
	statement_list_1->subGrammars.push_back(statement_list_);

	statement_list_2->subGrammars.push_back(t_epsilon);

	statement->isRaw = false;
	statement->subGrammars.push_back(def_stmt);
	statement->subGrammars.push_back(assign_stmt);
	statement->subGrammars.push_back(ret_stmt);
	statement->subGrammars.push_back(t_epsilon);

	def_stmt->subGrammars.push_back(ret_type);
	def_stmt->subGrammars.push_back(t_id);
	def_stmt->subGrammars.push_back(t_assign);
	def_stmt->subGrammars.push_back(t_num);
	def_stmt->subGrammars.push_back(t_semicolon);

	assign_stmt->subGrammars.push_back(t_id);
	assign_stmt->subGrammars.push_back(t_assign);
	assign_stmt->subGrammars.push_back(value);
	assign_stmt->subGrammars.push_back(t_semicolon);

	ret_stmt->subGrammars.push_back(t_return);
	ret_stmt->subGrammars.push_back(value);
	ret_stmt->subGrammars.push_back(t_semicolon);

	/*
	* value-> int | double
	*/
	value->isRaw = false;
	value->subGrammars.push_back(t_int);
	value->subGrammars.push_back(t_double);


	/*
	 * func_def -> ret_type id (param_list) { statement_list }
	 */
	func_def->subGrammars.push_back(ret_type);
	func_def->subGrammars.push_back(t_id);
	func_def->subGrammars.push_back(t_left_paran);
	func_def->subGrammars.push_back(param_list);
	func_def->subGrammars.push_back(t_right_paran);
	func_def->subGrammars.push_back(t_left_brace);
	func_def->subGrammars.push_back(statement_list);
	func_def->subGrammars.push_back(t_right_brace);

	/*
	* incl_def -> #include "t_id"
	*/
	incl_def->subGrammars.push_back(t_pound);
	incl_def->subGrammars.push_back(t_include);
	incl_def->subGrammars.push_back(t_quotation);
	incl_def->subGrammars.push_back(t_id);
	incl_def->subGrammars.push_back(t_quotation);


    // 传递文法到Parser中
    mParser* myParser = new mParser(program);
    return myParser;
}