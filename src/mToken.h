#include <string>

using std::string;

#ifndef M_TOKEN
#define M_TOKEN

// 定义token的类型
enum mTOKEN_TYPE {
	// 这里继续填入你的状态
	NUM, ID, INC, IN, VO, DO, RET, LP, RP, LB, RB, AS, SE, QU, COM, PO,
	EPSILON, ENDL, UN, NL,

	// 最后的不修改，用于计数
	mTOKEN_NUM
};

// 定义token结构体
struct mToken {
    int type;
    string text;
    double value;
    mToken (int tp = -1, const string& t = "", double v = 0): type(tp), text(t), value(v) {};
};

#endif