all: run

run: lexer-c
	./lexerTest

lexer-c: mToken.h mState.h mLexer.h mLexer.cpp lexerFuncions.h lexerFuncions.cpp mLexerInit.cpp lexerTest.cpp
	g++ *.h *.cpp -o lexerTest