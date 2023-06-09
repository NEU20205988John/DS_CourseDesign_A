//#include <exception>
//#include <stdexcept>
using namespace std;
template <class ElemType>

class ArrayDeque {
	public :
		ArrayDeque(int n = 100) {//n:最多能容纳的元素数
			Capacity = n + 1;
			if (n < 0)
				throw exception();//invalid_argument("invalid 'n' in ArrayStack Constructor");
			a = new ElemType[Capacity];
			Front = 0;
			Tail = 0;
		}
		ArrayDeque(ArrayDeque &u) {
			Capacity = u.Capacity;
			a = new ElemType[Capacity];
			Front = u.Front;
			Tail = u.Tail;
			for (int i = 0; i < Capacity; i++)
				a[i] = u.a[i];
		}
		~ArrayDeque() {
			delete []a;
		}
		void push_front(ElemType u) {
			if ((Tail + 1) % Capacity == Front)
				throw exception();//overflow_error("overflow error in push_front()");
			Front = (Front - 1 + Capacity) % Capacity;
			a[Front] = u;
		}
		void push_back(ElemType u) {
			if ((Tail + 1) % Capacity == Front)
				throw exception();//overflow_error("overflow error in push_back()");
			a[Tail] = u;
			Tail = (Tail + 1) % Capacity;
		}
		ElemType pop_front() {
			if (Tail == Front)
				throw exception();//underflow_error("underflow error in pop_front()");
			ElemType y = a[Front];
			a[Front] = 0;
			Front = (Front + 1) % Capacity;
			return y;
		}
		ElemType pop_back() {
			if (Tail == Front)
				throw exception();//underflow_error("underflow error in pop_back()");
			Tail = (Tail - 1) % Capacity;
			ElemType y = a[Tail];
			a[Tail] = 0;
			return y;
		}
		int length() {
			return (Tail - Front + Capacity) % Capacity;
		}
		int capacity() {
			return Capacity - 1;
		}
	protected :
		ElemType *a;
		int Front;
		int Tail;
		int Capacity;//实际存储空间数
} ;