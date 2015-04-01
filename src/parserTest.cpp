#include <iostream>
#include <string>

using namespace std;

#include "mLexer.h"
#include "mParser.h"

extern mLexer* mLexerInit();
extern mParser* mParserInit();

int main() {
	/*
	 * Lexer Test
	 */
	mLexer* lexer = mLexerInit();
	string str = "#include \"file\" int main(void) { int a = 123; double b = 456; a = b; return 0; }";
    lexer->process(str);
    cout <<str <<endl;
    lexer->printTokenStream();

	/* Parser Test
	 *
	 */
	mParser* parser = mParserInit();
	parser->process(lexer->getTokenStream());
	parser->printTree(parser->getASTree(), "");
    return 0;
}
