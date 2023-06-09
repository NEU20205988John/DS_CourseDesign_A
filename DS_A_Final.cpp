#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
//#include <conio.h>
#include <cstring>
#include "easyx.h"
#include <Windows.h>
#include <time.h>
#include "ArrayStack.h"
#include "ArrayDeque.h"
using namespace std;

class Complex {
private:
	double _real;
	double _image;
public:
	Complex(double r, double i);
	Complex(const Complex& u) {//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!const
		_real = u._real;
		_image = u._image;
	}
	Complex operator+(const Complex c);
	Complex operator-(const Complex& c);
	Complex operator*(const Complex& c);
	Complex operator/(const Complex& c);
	Complex& operator+=(const Complex c);
	Complex& operator-=(const Complex c);
	bool operator ==(const Complex& c);
	Complex operator ++();
	Complex operator ++(int);
	Complex operator --();
	Complex operator --(int);
	friend ostream& operator << (ostream& os, Complex p) {
		if (p._real != 0) {
			if (p._image == 1)
				return os << p._real << "+i";
			else if (p._image == -1)
				return os << p._real << "-i";
			else if (p._image == 0)
				return os << p._real;
			else if (p._image < 0)
				return os << p._real << p._image << "i";
			else
				return os << p._real << "+" << p._image << "i";
		}
		else {
			if (p._image == 1)
				return os << "i";
			else if (p._image == -1)
				return os << "-i";
			else if (p._image == 0)
				return os << p._real;
			else
				return os << p._image << "i";
		}
	};
};

Complex::Complex(double r = 0.0, double i = 0.0) {
	this->_real = r;
	this->_image = i;
}

Complex Complex::operator+(const Complex c) {
	Complex tmp;
	tmp._real = this->_real + c._real;
	tmp._image = this->_image + c._image;
	return tmp;
}

Complex Complex::operator-(const Complex& c) {
	Complex tmp;
	tmp._real = this->_real - c._real;
	tmp._image = this->_image - c._image;
	return tmp;
}

Complex Complex::operator*(const Complex& c) {
	Complex tmp;
	tmp._real = this->_real * c._real - _image * c._image;
	tmp._image = this->_image * c._real + this->_real * c._image;
	return tmp;
}

Complex Complex::operator/(const Complex& c) {//(a+bi)(c-di) / (c+di)(c-di)
	//(ac+bd)/(c^2+d^2)+(bc-ad)/(c^2+d^2) i
	Complex tmp;
	double deno = c._real * c._real + c._image * c._image;
	tmp._real = (this->_real * c._real + this->_image * c._image) / deno;
	tmp._image = (this->_image * c._real + this->_real * c._image) / deno;
	return tmp;
}

Complex& Complex::operator+=(const Complex c) {
	this->_real += c._real;
	this->_image += c._image;
	return *this;
}

Complex& Complex::operator-=(const Complex c) {
	this->_real -= c._real;
	this->_image -= c._image;
	return *this;
}

bool Complex::operator==(const Complex& c) {
	return (_real == c._real && _image == c._image);
}

Complex Complex::operator++() {//前置++
	this->_real += 1.0;
	this->_image += 1.0;
	return *this;
}

Complex Complex::operator++(int) {
	Complex tmp(*this);
	this->_real++;
	this->_image++;
	return tmp;
}

Complex Complex::operator--() {
	this->_real -= 1.0;
	this->_image -= 1.0;
	return (*this);
}

Complex Complex::operator--(int) {
	Complex tmp(*this);
	this->_real--;
	this->_image--;
	return tmp;
}

enum expnodetype {
	OPERATOR, OPERAND, STR
};

class expnode {
public:
	expnode(Complex u) {
		Type = OPERAND;
		i = u;
		ch = '\0';//12.08
	}
	expnode(double r, double i) : i(r,i) {//Default Constructor
		Type = OPERAND;
		ch = '\0';//12.08
	}
	expnode(char u = 0) {//Default Constructor
		Type = OPERATOR;
		ch = u;
	}
	expnode(string u) {
		Type = STR;
		e = u;
	}
	expnode& operator = (Complex u) {
		Type = OPERAND;
		i = u;
		return *this;
	}
	expnode& operator = (char u) {
		Type = OPERATOR;
		ch = u;
		return *this;
	}
	expnode& operator = (string u) {
		Type = STR;
		e = u;
		return *this;
	}
	void operator += (expnode u) {//expnode &: ambiguous overload          Complex char
		if (u.Type == OPERATOR) {
			if (Type == OPERATOR)
				ch += u.ch;
			else if (Type == OPERAND)
				ch += u.ch;
			else//Type == STR
				e += u.ch;
		}
		else if (u.Type == OPERAND) {//Complex
			if (Type == STR) {
				ostringstream st;
				st << u.i;
				e += st.str();
			}
		}
		else if (u.Type == STR)
			e += u.e;//".e" must be specified, or ambiguous overload
	}
	operator Complex() {
		if (Type == OPERAND)
			return i;
		else
			return Complex();//12.08
		/*else if (Type == OPERATOR)
			return ch;
		else//Type == STR
			return 0;*/
	}
	operator char() {
		if (Type == OPERATOR)
			return ch;
		else
			return '\0';//12.08
		/*else if (type == operand)
			return i;
		else//type == str
			return '\0';*/
	}
	operator string () {
		if (Type == STR)
			return e;
		else {
			ostringstream st;
			if (Type == OPERAND)
				st << i;
			else//Type == OPERATOR
				st << ch;
			return st.str();
		}
	}
	friend ostream& operator << (ostream& os, expnode p) {//expnode &   problem???
		if (p.Type == OPERAND)
			return os << p.i;
		else if (p.Type == OPERATOR)
			return os << p.ch;
		else
			return os << p.e;
	}
	expnodetype type() {
		return Type;
	}
protected:
	enum expnodetype Type;//0:char  1:Complex  2:string
	char ch;
	Complex i;
	string e;
};

typedef struct expnodeTree {
	expnode data;
	expnodeTree* lchild, * rchild;
} expTree, * expTreePtr;

bool isoperator(char c) {//12.09
	return (c == '+' || c == '-' || c == '*' || c == '/') ? true : false;
}

expTreePtr expnodeBuildTree(string s) {
	//cout << "s = " << s << endl;
	/*操作数，作为叶子结点，可以看作是左右儿子都为NULL的父结点，因此也可以看作一棵二叉树。
	因此算法是，建立一个二叉树栈，扫描后缀表达式：
	1.若遇到操作数，把其作为左右儿子都为NULL的二叉树，根结点入栈。
	2.若遇到操作符，弹出两个栈中结点，作为此操作符的左、右儿子（先弹出的是右儿子，后弹出的是左儿子），然后把操作符作为根结点入栈。
	最后栈中剩余的唯一结点就是整个表达式树的根结点，表达式树构建完毕。*/
	stringstream ss;
	stringstream ss1;
	ArrayStack<expTreePtr> stk;//二叉树结点指针栈
	ss.str(s);
	ss1.str(s);
	//while (!ss.eof()) {
	//	string s1;
	//	ss >> s1;
	//	if (s1[s1.length() - 1] >= '0' && s1[s1.length() - 1] <= '9') {//real number
	//		expTreePtr root;
	//		root = (expTreePtr)malloc(sizeof(expTree));
	//		double num;
	//		ss1 >> num;
	//		Complex c(num, 0);
	//		root->lchild = NULL;
	//		root->rchild = NULL;
	//		root->data = c;
	//		stk.push(root);
	//	}
	//	else if(s1[s1.length() - 1] == 'i') {//image number -123-456i 
	//		string temp;
	//		ss1 >> temp;
	//		expTreePtr root;
	//		s1 = s1.substr(0, s1.length() - 1);
	//		string res = "";
	//		for (int i = 0; i < s1.length(); i++) {
	//			if (isdigit(s1[i]) && isoperator(s1[i + 1])) {
	//				res += s1[i];
	//				res += " ";
	//			}
	//			else if(isdigit(s1[i])||s1[i]=='.') {
	//				res += s1[i];
	//			}
	//			else if(isoperator(s1[i])) {
	//				res += s1[i];
	//			}
	//		}
	//		double r, i;
	//		stringstream complexs(res);
	//		complexs >> r >> i;
	//		//cout << "r = " << r << "i = " << i << endl;
	//		Complex c(r, i);
	//		root = (expTreePtr)malloc(sizeof(expTree));
	//		root->lchild = NULL;
	//		root->rchild = NULL;
	//		root->data = c;
	//		stk.push(root);
	//	}
	//	else {
	//		/*2.若遇到操作符，弹出两个栈中结点，作为此操作符的左、
	//		右儿子（先弹出的是右儿子，后弹出的是左儿子），
	//		然后把操作符作为根结点入栈。*/
	//		char op;
	//		ss1 >> op;
	//		//cout << "op = " << op << endl;
	//		//expTree right,left;
	//		//right=stk.pop();
	//		//left=stk.pop();
	//		expTreePtr optrPtr = (expTreePtr)malloc(sizeof(expTree));
	//		expTreePtr l, r;
	//		l = (expTreePtr)malloc(sizeof(expTree));
	//		r = (expTreePtr)malloc(sizeof(expTree));
	//		r = stk.pop();
	//		l = stk.pop();
	//		//l->lchild=left.lchild;
	//		//l->rchild=left.rchild;
	//		//l->data=left.data;
	//		optrPtr->lchild = l;
	//		optrPtr->rchild = r;
	//		optrPtr->data = op;
	//		stk.push(optrPtr);
	//	}
	//}
	while (!ss.eof()) {
		string s1;
		ss >> s1;
		if (s1[s1.length() - 1] >= '0' && s1[s1.length() - 1] <= '9') {//real number
			double num;
			ss1 >> num;
			expTreePtr root = (expTreePtr)malloc(sizeof(expTree));
			root->lchild = NULL;
			root->rchild = NULL;
			root->data = Complex(num, 0);
			stk.push(root);
		}
		else if (s1[s1.length() - 1] == 'i') {//image number -123-456i
			//这里的nex_exploader和expnodeBuildTree使用了同样的字符串处理思路，可以处理复数类
			string temp;
			ss1 >> temp;
			temp.pop_back();//删除最后的i
			int k;
			for (k = 0; k < temp.length(); k++) {
				if (isdigit(temp[k]) && isoperator(temp[k + 1])) {//溢出？？
					temp.insert(k + 1, " ");//在下标k+1对应的位置之前插入
					break;
				}
			}
			double r, i;
			if (k == temp.length()) {//无实部
				r = 0;
				if (temp == "" || temp == "+")//"i"
					i = 1;
				else if (temp == "-")//"-i"
					i = -1;
				else {
					stringstream complexs(temp);
					complexs >> i;
				}
			}
			else {
				stringstream complexs(temp);
				complexs >> r >> i;
				//cout << "r = " << r << " i = " << i << endl;
			}
			expTreePtr root = (expTreePtr)malloc(sizeof(expTree));
			root->lchild = NULL;
			root->rchild = NULL;
			root->data = Complex(r, i);
			stk.push(root);
		}
		else {//OPERATOR
			/*2.若遇到操作符，弹出两个栈中结点，作为此操作符的左、
			右儿子（先弹出的是右儿子，后弹出的是左儿子），
			然后把操作符作为根结点入栈。*/
			char op;
			ss1 >> op;
			expTreePtr optrPtr = (expTreePtr)malloc(sizeof(expTree));
			/*expTreePtr l, r;
			l = (expTreePtr)malloc(sizeof(expTree));
			r = (expTreePtr)malloc(sizeof(expTree));
			r = stk.pop();
			l = stk.pop();
			optrPtr->lchild = l;
			optrPtr->rchild = r;*/
			optrPtr->rchild = stk.pop();
			optrPtr->lchild = stk.pop();
			optrPtr->data = op;
			stk.push(optrPtr);
		}
	}
	return stk.pop();
}

void PostOrderTraverse(ArrayStack<Complex>& p, expTreePtr root) { //
	if (root->lchild == NULL && root->rchild == NULL) {
		p.push((Complex)(root->data));
		cout << "根结点的数据为：" << (Complex)(root->data) << endl;
		return;//操作数在叶子节点,递归结束
	}
	else {
		if (root) {
			PostOrderTraverse(p, root->lchild);
			PostOrderTraverse(p, root->rchild); //int 全都需要改成COMPLEX！！！
			cout << "optr : " << (char)(root->data) << endl;
			Complex num1, num2;
			if (p.length()) {
				num2 = p.pop();
				num1 = p.pop();
			}
			//cout << num1 << " " << num2 << endl;

			if ((char)(root->data) == '+') {
				Complex res = num1 + num2;
				p.push(res);
			}
			else if ((char)(root->data) == '-') {
				Complex res = num1 - num2;
				p.push(res);
			}
			else if ((char)(root->data) == '*') {
				Complex res = num1 * num2;
				p.push(res);
			}
			else if ((char)(root->data) == '/') {
				Complex res = num1 / num2;
				p.push(res);
			}
		}
	}
}

void exploader(ArrayDeque<expnode>& u) {
	char pre = 0, c = 0;
	int nnum = 0;
	c = getchar();
	while (c != 10 && c != EOF) {
		if (c >= '0' && c <= '9') {
			if (pre >= '0' && pre <= '9')
				nnum = nnum * 10 + (c - '0');
			else
				nnum = (c - '0');
		}
		else {
			if (pre >= '0' && pre <= '9')
				u.push_back(nnum);
			nnum = 0;
			u.push_back(c);
		}
		pre = c;
		c = getchar();
	}
	if (pre >= '0' && pre <= '9')
		u.push_back(nnum);
	else
		u.push_back(c);//结尾有括号
}

void pop_to_strnode(ArrayStack<expnode>& opnd, ArrayStack<expnode>& optr, char a, char b) {
	ArrayStack<expnode> etemp;
	while (char(optr.top()) == a || char(optr.top()) == b) {
		etemp.push(optr.pop());
		etemp.push(opnd.pop());
	}
	etemp.push(opnd.pop());//negative???
	expnode et("");
	while (etemp.length() - 1) {
		et += etemp.pop();
		et += ' ';
	}
	et += etemp.pop();
	opnd.push(et);
}

void new_exploader(ArrayDeque<expnode>& u, string s) {
	stringstream ss;
	stringstream ss1;
	ss.str(s);
	ss1.str(s);
	while (!ss.eof()) {
		string s1;
		ss >> s1;
		if (s1[s1.length()-1] >= '0' && s1[s1.length() - 1] <= '9') {//real number
			double num;
			ss1 >> num;
			u.push_back(Complex(num, 0));
			//cout << "num = " << num << endl;
		}
		else if(s1[s1.length() - 1]=='i') {//image number -123-456i 
			//这里的nex_exploader和expnodeBuildTree使用了同样的字符串处理思路，可以处理复数类
			string temp;
			ss1 >> temp;
			temp.pop_back();//删除最后的i
			//s1 = s1.substr(0, s1.length() - 1);
			//string res = "";
			/*for (int i = 0; i < s1.length(); i++) {
				if (isdigit(s1[i]) && isoperator(s1[i + 1])) {
					res += s1[i];
					res += " ";
				}
				else if (isdigit(s1[i]) || s1[i] == '.') {
					res += s1[i];
				}
				else if (isoperator(s1[i])) {
					res += s1[i];
				}
			}*/
			int k;
			for (k = 0; k < temp.length(); k++) {
				if (isdigit(temp[k]) && isoperator(temp[k + 1])) {//溢出？？
					temp.insert(k + 1, " ");//在下标k+1对应的位置之前插入
					break;
				}
			}
			double r, i;
			if (k == temp.length()) {//无实部
				r = 0;
				if (temp == "" || temp == "+")//"i"
					i = 1;
				else if (temp == "-")//"-i"
					i = -1;
				else {
					stringstream complexs(temp);
					complexs >> i;
				}
			}
			else {
				stringstream complexs(temp);
				complexs >> r >> i;
				//cout << "r = " << r << " i = " << i << endl;
			}
			u.push_back(Complex(r, i));
		}
		else {//OPERATOR
			char op;
			ss1 >> op;
			u.push_back(op);
			//cout << "op = " << op << endl;
		}
	}
}

void calcexp(ArrayStack<expnode>& opnd, ArrayStack<expnode>& optr, char a, char b) {
	char thisoptr = '\0', lastoptr = '\0';//12.08
	Complex thisopnd, lastopnd;
	if (a == '*' && b == '/') {
		lastoptr = '*';
		lastopnd = 1;
	}
	else if (a == '+' && b == '-') {
		lastoptr = '+';
		lastopnd = 0;
	}
	while (char(optr.top()) == a || char(optr.top()) == b) {
		thisoptr = optr.pop();
		thisopnd = opnd.pop();
		if (lastoptr == '*') {
			lastopnd = thisopnd * lastopnd;
		}
		else if (lastoptr == '/') {
			lastopnd = thisopnd / lastopnd;
		}
		else if (lastoptr == '+') {
			lastopnd = thisopnd + lastopnd;
		}
		else if (lastoptr == '-') {
			lastopnd = thisopnd - lastopnd;
		}
		lastoptr = thisoptr;
	}
	thisopnd = opnd.pop();
	if (lastoptr == '*') {
		lastopnd = thisopnd * lastopnd;
	}
	else if (lastoptr == '/') {
		lastopnd = thisopnd / lastopnd;
	}
	else if (lastoptr == '+') {
		lastopnd = thisopnd + lastopnd;
	}
	else if (lastoptr == '-') {
		lastopnd = thisopnd - lastopnd;
	}
	opnd.push(lastopnd);
}

void RPNresult(ArrayDeque<expnode> y, string &RPN, Complex &result) {//Output: RPN, result 12.08
	ArrayStack<expnode> optr(y.capacity() / 2);
	ArrayStack<expnode> opnd(y.capacity() / 2);
	ArrayStack<expnode> optrcalc = optr;
	ArrayStack<expnode> opndcalc = opnd;
	expnode t;
	while (y.length()) {
		t = y.pop_front();
		if (t.type() == OPERAND) {
			opnd.push(t);
			opndcalc.push(t);
		}
		else {//OPERATOR
			if (char(t) == ')') {
				if (char(optr.top()) == '*' || char(optr.top()) == '/') {
					pop_to_strnode(opnd, optr, '*', '/');
					calcexp(opndcalc, optrcalc, '*', '/');
				}
				pop_to_strnode(opnd, optr, '+', '-');
				calcexp(opndcalc, optrcalc, '+', '-');
				optr.pop();//pop '('
				optrcalc.pop();
			}
			else if (char(t) == '+' || char(t) == '-') {
				if (char(optr.top()) == '*' || char(optr.top()) == '/') {
					pop_to_strnode(opnd, optr, '*', '/');
					calcexp(opndcalc, optrcalc, '*', '/');
				}
				optr.push(t);
				optrcalc.push(t);
			}
			else if (char(t) == '*' || char(t) == '/' || char(t) == '(') {
				optr.push(t);
				optrcalc.push(t);
			}
		}
	}
	if (char(optr.top()) == '*' || char(optr.top()) == '/') {
		pop_to_strnode(opnd, optr, '*', '/');
		calcexp(opndcalc, optrcalc, '*', '/');
	}
	pop_to_strnode(opnd, optr, '+', '-');
	calcexp(opndcalc, optrcalc, '+', '-');
	RPN = (string)opnd.pop();
	result = (Complex)opndcalc.pop();
}

void string2TCHAR(string& str, TCHAR* tchar) {//12.08
	int iLength = MultiByteToWideChar(CP_ACP, 0, str.c_str(), strlen(str.c_str()) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), strlen(str.c_str()) + 1, tchar, iLength);
}

void TCHAR2string(TCHAR* tchar, string& str) {//12.08
	int length = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	char *ch = new char[length];
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, ch, length, NULL, NULL);
	str = string(ch);
	delete[]ch;
}

bool illegalchar(const string &y) {//有非法字符时返回true 12.08
	int ly = y.length();
	stringstream z(y);
	char t;
	for (int i = 0; i < ly; i++) {
		z >> t;
		if (!(isdigit(t) || isoperator(t) || t == 'i' || t == '(' || t == ')' || t == ' ' || t == '.'))
			return true;
	}
	return false;
}

bool bracketmatch(const string& y) {//括号匹配正确时返回true 12.08
	int ly = y.length();
	ArrayStack<char> r(ly);
	const char *ty = y.c_str();
	for (int i = 0; i < ly; i++) {
		if (ty[i] == '(')
			r.push(ty[i]);
		else if (ty[i] == ')') {
			if (r.top() == '(')
				r.pop();
			else
				return false;
		}
	}
	return (r.length() == 0) ? true : false;
}

void drawbtn(string txt, int shiftx, int shifty, int fontsize, int btns[][4], int btnno,
	int cornersize = 6, int lx = 60, int ly = 60, COLORREF btncolor = WHITE, 
	COLORREF linecolor = BLACK, COLORREF txtcolor = BLACK, int linestyle = PS_SOLID, 
	int thickness = 2, string font = "Microsoft Yahei") {//绘制按钮
	TCHAR* _txt = new TCHAR[txt.length() + sizeof(TCHAR)];
	TCHAR* _font = new TCHAR[font.length() + sizeof(TCHAR)];
	string2TCHAR(txt, _txt);
	string2TCHAR(font, _font);
	COLORREF oldbkcolor = getbkcolor();
	//set
	setfillcolor(btncolor); //按钮颜色
	setlinecolor(linecolor);//按钮边线颜色
	setlinestyle(linestyle, thickness);//按钮边线线型、宽度
	settextcolor(txtcolor);//按钮中文字颜色
	setbkcolor(btncolor);//背景色=按钮颜色
	settextstyle(fontsize, 0, _font);//字体
	btns[btnno][0] = lx + shiftx;//左侧x
	btns[btnno][1] = ly + shifty;//上侧y
	btns[btnno][2] = lx + shiftx + textwidth(_txt) + 2 * cornersize;//右侧x
	btns[btnno][3] = ly + shifty + textheight(_txt) + 2 * cornersize;//下侧y
	//draw
	fillrectangle(btns[btnno][0], btns[btnno][1], btns[btnno][2], btns[btnno][3]);//rectangle();
	outtextxy(lx + shiftx + cornersize, ly + shifty + cornersize, _txt);
	//set_back
	setbkcolor(oldbkcolor);
	delete[]_txt;
	delete[]_font;
}

void drawtxt(string txt, int shiftx, int shifty, int fontsize, int lx = 60, int ly = 60,
	COLORREF txtcolor = YELLOW, string font = "Microsoft Yahei") {
	TCHAR* _txt = new TCHAR[txt.length() + sizeof(TCHAR)];
	TCHAR* _font = new TCHAR[font.length() + sizeof(TCHAR)];
	string2TCHAR(txt, _txt);
	string2TCHAR(font, _font);
	settextcolor(txtcolor);//文字颜色
	settextstyle(fontsize, 0, _font);//字体
	//draw
	outtextxy(lx + shiftx, ly + shifty, _txt);
	delete[]_txt;
	delete[]_font;
}

void coverold(int lx, int ly, int shiftx, int shifty, int fontsize) {//覆盖之前的表达式
	setlinecolor(LIGHTBLUE);
	setfillcolor(LIGHTBLUE);
	fillrectangle(lx + shiftx, ly + shifty, 1024, ly + shifty + fontsize);
}

void fix_draw_main(int btns[][4]) {//绘制固定组件 12.08
	//int lx = 60, ly = 60, fontsize, shiftx, shifty;
	//按钮
	drawbtn("输入表达式",  0, 0, 30, btns, 0);//0号"输入表达式"按钮
	drawbtn("计算", 40, 150, 30, btns, 1);//1号"计算"按钮
	drawbtn("绘制二叉树", 150, 150, 30, btns, 2);//2号"绘制二叉树"按钮
	drawbtn("随机输入", 330, 150, 30, btns, 3);//3号"随机输入"按钮
	drawbtn("退出", 910, -50, 24, btns, 4, 4);//4号"退出"按钮
	drawbtn("从文件输入", 480, 150, 30, btns, 5);//5号"从文件输入"按钮
	drawtxt("每个操作数、运算符和左右括号之间都需以空格分隔，表达式的长度不超过100个字符", 150, 10, 24);//输入要求提示信息
	drawtxt("输入的表达式：", 5, 80, 24);//"输入的表达式："提示信息
	drawtxt("RPN：", 5, 250, 24);//"RPN: " 提示信息
	drawtxt("Answer: ", 5, 330, 24);//"Answer: " 提示信息
	/*LOGFONT txtstyle;
	gettextstyle(&txtstyle);//获得字体格式
	COLORREF clr = getbkcolor();
	COLORREF cle = BLACK;*/
	//(strlen((char*)inputbtn) - 2)
	//_getch();
}

void dynamic_draw_main(string& in, string& RPN, string& Answer, bool illchar, bool brktmatch) {//绘制动态组件 12.08
	//int lx = 60, ly = 60, fontsize, shiftx, shifty;
	if (!illchar && brktmatch)//string: in 提示信息
		drawtxt(in, 150, 80, 24);
	else if (illchar)
		drawtxt("输入存在非法字符", 150, 80, 24);
	else if (!brktmatch)
		drawtxt("输入表达式的括号不匹配", 150, 80, 24);

	drawtxt(RPN, 150, 250, 24);//string: RPN 提示信息
	drawtxt(Answer, 150, 330, 24);//string: Answer 提示信息
}

void fix_draw_drawtree(int btns[][4]) {
	drawbtn("返回", 20, 20, 30, btns, 0, 6, 0, 0);//0号"返回"按钮
}

void deconstruct_expTree(expTreePtr root) {
	if (root) {
		if (root->lchild)
			deconstruct_expTree(root->lchild);
		if (root->rchild)
			deconstruct_expTree(root->rchild);
		free(root);
	}
	return;
}

bool btnpressed(int x, int y, int btns[][4], int i) {//当i号按钮被按下时返回true 12.08
	return (x >= btns[i][0] && x <= btns[i][2] && y >= btns[i][1] && y <= btns[i][3]) ? true : false;
}

void PrintTree(expTreePtr tree, int x, int y, float xFar, float yFar) {//绘制二叉树
	if (tree->lchild) {
		line(x, y, x - xFar, y + yFar);
		PrintTree(tree->lchild, x - xFar, y + yFar, xFar / 1.8, yFar + 10); //*1.2
	}
	if (tree) {
		setfillcolor(BLACK);
		fillcircle(x, y, 15);
		TCHAR num[10];
		TCHAR ch[10];
		string temp = (string)(tree->data);
		int len = temp.length();
		for (int i = 0; i <= len; i++) {
			ch[i] = temp[i];
		}
		_stprintf_s(num, _T("%s"), ch);
		//cout << tree->data;
		outtextxy(x - 7.5, y - 7.5, num);
	}
	if (tree->rchild) {
		line(x, y, x + xFar, y + yFar);
		PrintTree(tree->rchild, x + xFar, y + yFar, xFar / 1.8, yFar + 10);
	}
}

void PrintUITree(expTreePtr tree) {//绘制二叉树
	const int UI_FAR_X = 220;
	const int UI_FAR_Y = 20;
	setbkcolor(LIGHTBLUE);
	cleardevice();
	PrintTree(tree, 1024 / 2, 50, UI_FAR_X, UI_FAR_Y);//WIN_X / 2
}

void random_input(string& in) {
	char expforms[5][30] = {"i-i*(i-i)", "i/(i+i)", "i+i*i+i/(i+i)-i", "i*i/i+i-i+i*i+i", "i/i-i+i*i-i"};
	stringstream putwhatevercharacterintome;
	//确定一个表达式模板
	srand((unsigned)time(NULL));
	int t = rand() % 5;
	//确定每个数
	int m = 0;
	while (expforms[t][m] != '\0') {
		int r = 0, real = 0, image = 0;
		if (expforms[t][m] == 'i') {//确定是否含虚部
			r = rand() % 2;
			if (r)
				image = rand() % 100;
			real = rand() % 100;
			Complex tp(real, image);
			putwhatevercharacterintome << tp;
		}
		else
			putwhatevercharacterintome << expforms[t][m];
		putwhatevercharacterintome << ' ';
		m++;
	}
	getline(putwhatevercharacterintome, in);
	in.pop_back();//弹出最后的空格，防止影响exploader
}

int main() {
	initgraph(1024, 600, EW_SHOWCONSOLE);//初始化绘图窗口
	HWND hWnd = GetHWnd();
	SetWindowText(hWnd, _T("Expression Calculator with Complex"));//修改窗口名称
	IMAGE mainscreen, drawtree;//主屏幕IMAGE对象，绘制树界面IMAGE对象
	Resize(&mainscreen, 1024, 600);
	Resize(&drawtree, 1024, 600);
	LOGFONT lgfont;//设置字体抗锯齿↓↓
	SetWorkingImage(&drawtree);
	gettextstyle(&lgfont);
	lgfont.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&lgfont);
	SetWorkingImage(&mainscreen);
	gettextstyle(&lgfont);
	lgfont.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&lgfont);//设置字体抗锯齿↑↑
	setbkcolor(LIGHTBLUE);
	cleardevice();
	int btns[7][4] = {};//每行保存每个按钮的xy值，[0]: "输入表达式"，[1]: "计算"，[2]: "绘制二叉树"
	fix_draw_main(btns);//绘制固定组件
	bool illchar = false, brktmatch = true;//(有)无非法字符，括号(是)否匹配
	bool exitflag = false, inmainscreen = true, fileinput = false;//(是)否退出程序，(是)否在主屏幕
	string in = "( 8+2i - -3i ) * 2", RPN, Answer;
	expTreePtr root = NULL;
	Complex result;
	ifstream thefile;
	ExMessage m;
	while (!exitflag) {//消息循环
		if (inmainscreen) {//当前在主屏幕
			dynamic_draw_main(in, RPN, Answer, illchar, brktmatch);//绘制动态组件
			SetWorkingImage();//当前绘图设备改为绘图窗口
			putimage(0, 0, &mainscreen);//将主屏幕IMAGE对象输出至绘图窗口
			SetWorkingImage(&mainscreen);//当前绘图设备改回主屏幕IMAGE对象
			if (peekmessage(&m)) {
				if (m.message == WM_LBUTTONUP) {//若左键弹起
					if (btnpressed(m.x, m.y, btns, 0)) {//"输入表达式"按下
						if (fileinput == true) {
							fileinput = false;
							thefile.close();
						}
						TCHAR last[101] = {};//上次输入串临时空间
						string2TCHAR(in, last);
						TCHAR intemp[101] = {};//输入串临时空间
						InputBox(intemp, 100, NULL, _T("输入表达式"), last, 650, 0, false);
						if (intemp[0] != '\0') {
							TCHAR2string(intemp, in);
							illchar = illegalchar(in);
							brktmatch = bracketmatch(in);
							coverold(60, 60, 150, 80, 24);//覆盖之前的表达式
							coverold(60, 60, 150, 250, 24);//覆盖之前的RPN
							coverold(60, 60, 150, 330, 24);//覆盖之前的Answer
							RPN = "";
							Answer = "";
							deconstruct_expTree(root);//析构之前的二叉树
						}
					}
					else if (btnpressed(m.x, m.y, btns, 1)) {//"计算"按下
						coverold(60, 60, 150, 250, 24);//覆盖之前的RPN
						coverold(60, 60, 150, 330, 24);//覆盖之前的Answer
						if (!illchar && brktmatch) {
							ArrayDeque<expnode> y(100);//capacity
							new_exploader(y, in);
							RPNresult(y, RPN, result);
							root = expnodeBuildTree(RPN);
							ArrayStack<Complex> p(100);
							PostOrderTraverse(p, root);
							stringstream sans;
							sans << p.pop();
							sans >> Answer;
						}
						else if (illchar)
							RPN = "输入存在非法字符";
						else if (!brktmatch)
							RPN = "输入表达式的括号不匹配";
					}
					else if (btnpressed(m.x, m.y, btns, 2)) {//"绘制二叉树"按下
						if (Answer == "")
							RPN = "请先计算结果，再绘制二叉树";
						else {
							SetWorkingImage(&drawtree);
							inmainscreen = false;
							PrintUITree(root);
							fix_draw_drawtree(btns);
						}
					}
					else if (btnpressed(m.x, m.y, btns, 3)) {//"随机输入"按下
						if (fileinput == true) {
							fileinput = false;
							thefile.close();
						}
						random_input(in);
						illchar = false;
						brktmatch = true;
						coverold(60, 60, 150, 80, 24);//覆盖之前的表达式
						coverold(60, 60, 150, 250, 24);//覆盖之前的RPN
						coverold(60, 60, 150, 330, 24);//覆盖之前的Answer
						RPN = "";
						Answer = "";
						deconstruct_expTree(root);//析构之前的二叉树
					}
					else if (btnpressed(m.x, m.y, btns, 5)) {//"从文件输入"按下
						if (fileinput == false) {
							fileinput = true;
							thefile.open("./expinput.txt", ios::in);
						}
						if (thefile.is_open()) {
							if (!thefile.eof()) {
								getline(thefile, in);
								illchar = illegalchar(in);
								brktmatch = bracketmatch(in);
								RPN = "再次点击\"从文件输入\"以读取下一行";
								Answer = "点击\"计算\"以计算结果";
								deconstruct_expTree(root);//析构之前的二叉树
							}
							else {
								RPN = "已到达文件尾，再次点击可从头读取";
								fileinput = false;
								thefile.close();
							}
							coverold(60, 60, 150, 80, 24);//覆盖之前的表达式
							coverold(60, 60, 150, 250, 24);//覆盖之前的RPN
							coverold(60, 60, 150, 330, 24);//覆盖之前的Answer
						}
					}
					else if (btnpressed(m.x, m.y, btns, 4))
						exitflag = true;
				}
			}
		}
		else {//当前在绘制二叉树界面
			SetWorkingImage();//当前绘图设备改为绘图窗口
			putimage(0, 0, &drawtree);//将绘制二叉树界面IMAGE对象输出至绘图窗口
			SetWorkingImage(&drawtree);//当前绘图设备改回主屏幕IMAGE对象;
			if (peekmessage(&m)) {
				if (m.message == WM_LBUTTONUP) {
					if (btnpressed(m.x, m.y, btns, 0)) {//"返回"按下
						SetWorkingImage(&mainscreen);
						inmainscreen = true;
						fix_draw_main(btns);
					}
				}
			}
		}
		flushmessage();
		Sleep(30);
	}
	closegraph();
	/*string uu;
	getline(cin, uu);
	TCHAR *j = new TCHAR[uu.length() + 1];
	string2TCHAR(uu, j);
	for (int i = 0; i < uu.length() + 1; i++)
		cout << char(j[i]) << endl;
	delete []j;*/
	/*ArrayDeque<expnode> y(100);//capacity
	string str;
	getline(cin,str);
	new_exploader(y,str);*/
	/*string RPN;
	Complex result;
	RPNresult(y, RPN, result);//12.08
	cout << "RPN: " << RPN << endl;
	cout << "Result: " << result << endl;
	//	ArrayStack<expTreePtr> p;
	//	ArrayStack<expTreePtr> *Ptrp = &p;
	expTreePtr root = expnodeBuildTree(RPN);
	//	cout << root << endl;
	//
	//	if (!(int)(root->data)) {
	//		cout << "NULL!!" << endl;
	//
	//	} else {
	//		cout << (int)(root->data) << endl;
	//	}
	//	cout << root->data << endl;
	//	cout << root->lchild->data << endl;
	//	cout << root->rchild->data << endl;
	ArrayStack<Complex> p;
	//ArrayStack<int> *Ptrp = &p;
	PostOrderTraverse(p, root);
	cout << "answer = " << p.pop() << endl;*/
	return 0;
}