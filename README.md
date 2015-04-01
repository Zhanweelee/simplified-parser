# 简易编译器
本编译器由词法分析(**mLexer**)、语法分析(**mParser**)和语义解析三部分组成。其中库中提供lexer和parser部分的功能实现，可以直接通过包含头文件，进行实例化构造不同需求的lexer和parser，而语义解析部分没有具体的代码提供，但是有相应的例子(**Simplified-SQL**)提供参考。
项目地址(simplified-compiler)：https://github.com/Zhanweelee/simplified-compiler

# 简易语法分析器

@[mParser|语法分析器|自顶向下方法|LL(1)|编译器]

本项目是simplified-compiler的子项目，是编译器的第二部分语法分析过程，所使用的类名为mParser。

**mParser**是语法分析器的类名，使用者可以自行进行继承和封装，内部采用自顶向下LL(1)的方法对token流进行语法分析，输出语法树。

**注意** ~~本项目暂时无法用GCC编译，项目测试在Visual Studio 2012项目下可以正常编译和运行。~~ 本项目可以使用GCC、VC11等编译，如有问题请提交到issues


### 使用步骤
可以概括为三个步骤：
1. 包含头文件`"mLexer.h"`,`"mParser.h"`和`"mGrammar.h"`
2. 在`mParserInit.cpp`中定义语法规则
3. 实例化mLexer，调用process方法对token流进行处理
4. 调用`parser->printTree(parser->getASTree(), "");`输出语法树

### 例子
使用**mLexer**构造一个C程序代码的词法分析，可以解析一下代码并且得到相应的token。
```
#include "file"
int main(void) {
    int a = 123;
	printf("Hello world");
	return 0;
}
```

### 进入语法分析前的准备
- 如果您没有在**词法分析**部分得到token流，请移步  [simplified-lexer](https://github.com/Zhanweelee/simplified-lexer) 查看readme

- 如果您通过继承或者其他方法继承mLexer，得到符合规定的token流，请通过 ```lexer->getTokenStream(); ``` 返回值传递给mParser的实例方法process，进行处理。


## 包含头文件，编写main文件
```

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
```

main函数的编写只需要在lexer处理文本之后，传递获取到的tokenStream传递给parser继续处理。我们先编写测试main函数是为了让读者更加清晰我们这一步骤的目的，也符合了测试优先的原则。

## 定义语法规则
在定义文法之前您可能需要一定的基础知识做铺垫。这里做个声明，mParser采用自顶向下的分析方法，采用LL(1)分析获得语法树。

为了符合LL(1)的性质，您的文法需要满足一下性质
- 不包含左递归，如果包含请手动消除左递归，如`S -> Sb | a`可以转化为`S -> aS', S' -> a S' | b `
- 不存在可提取的左因式, 如` S -> a | aBc` 可以提取因子a
- 等等

这个例子的文法可以规定为
```
program -> epsilon | program_ program
program_ -> func_def | incl_def

param_list -> t_void | ret_type id param_list_
param_list_ -> epsilon | , ret_type id param_list_

statement_list -> statement statement_list | epsilon
statement -> def_stmt | assign_stmt | ret_stmt | epsilon
def_stmt -> ret_type id = value;
assign_stmt -> id = value;
value -> num | id
ret_stmt -> return value;

ret_type -> int | double

value-> value_1 | value_2
value_1 -> t_num
value_2 -> t_id

func_def -> ret_type id (param_list) { statement_list }

incl_def -> t_pound t_include t_left_param t_id t_right_param
```

## 实例化mParser
执行main函数即可

## 输出语法树
可以查看到本例子的结果
```
┌0
    ┌0
    ├──4
    ├      ┌Token ('#')
    ├      ├Token ('include')
    ├      ├Token ('"')
    ├      ├Token ('file')
    ├      └Token ('"')
    └0
        └0
            ┌0
            ├──3
            ├      ┌5
            ├      ├──Token ('int')
            ├      ├Token ('main')
            ├      ├Token ('(')
            ├      ├6
            ├      ├──Token ('void')
            ├      ├Token (')')
            ├      ├Token ('{')
            ├      ├1
            ├      ├  ├1
            ├      ├      ┌2
            ├      ├      ├  ┌8
            ├      ├      ├      ┌5
            ├      ├      ├      ├──Token ('int')
            ├      ├      ├      ├Token ('a')
            ├      ├      ├      ├Token ('=')
            ├      ├      ├      ├7
            ├      ├      ├      ├──7
            ├      ├      ├      ├──────Token ('123')
            ├      ├      ├      └Token (';')
            ├      ├      └1
            ├      ├          └1
            ├      ├              ┌2
            ├      ├              ├──8
            ├      ├              ├      ┌5
            ├      ├              ├      ├──Token ('double')
            ├      ├              ├      ├Token ('b')
            ├      ├              ├      ├Token ('=')
            ├      ├              ├      ├7
            ├      ├              ├      ├──7
            ├      ├              ├      ├──────Token ('456')
            ├      ├              ├      └Token (';')
            ├      ├              └1
            ├      ├                  └1
            ├      ├                      ┌2
            ├      ├                      ├──9
            ├      ├                      ├      ┌Token ('a')
            ├      ├                      ├      ├Token ('=')
            ├      ├                      ├      ├7
            ├      ├                      ├      ├──7
            ├      ├                      ├      ├──────Token ('b')
            ├      ├                      ├      └Token (';')
            ├      ├                      └1
            ├      ├                          └1
            ├      ├                              ┌2
            ├      ├                              ├──10
            ├      ├                              ├      ┌Token ('return')
            ├      ├                              ├      ├7
            ├      ├                              ├      ├──7
            ├      ├                              ├      ├────Token ('0')
            ├      ├                              ├      └Token (';')
            ├      ├                              └1
            ├      └Token ('}')
            └0
```
