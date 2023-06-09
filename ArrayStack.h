//#include <exception>
//#include <stdexcept>
using namespace std;
template <class ElemType>

class ArrayStack {
	public :
		ArrayStack(int n = 100) {//n: Capacity
			if (n < 0)
				throw exception();//invalid_argument("invalid 'n' in ArrayStack Constructor");
			a = new ElemType[n];
			Top = 0;
			Capacity = n;
		}
		ArrayStack(ArrayStack &u) {//Copy Constructor
			Capacity = u.Capacity;
			a = new ElemType[Capacity];
			Top = u.Top;
			for (int i = 0; i < Top; i++)
				a[i] = u.a[i];
		}
		~ArrayStack() {
			delete []a;
		}
		void push(ElemType u) {
			if (Top == Capacity)
				throw exception();//overflow_error("overflow error in push()");
			a[Top] = u;
			Top++;
		}
		ElemType pop() {
			if (Top == 0)
				throw exception();//underflow_error("underflow error in pop()");
			Top--;
			ElemType y = a[Top];
			a[Top] = 0;
			return y;
		}
		int length() {
			return Top;
		}
		ElemType top() {
			if (Top)
				return a[Top - 1];
			else
				return -1;//////
		}
		int capacity() {
			return Capacity;
		}
	protected :
		ElemType *a;
		int Top;//Points to the next unit of the top element
		int Capacity;
} ;