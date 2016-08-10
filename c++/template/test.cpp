#include <iostream>

template <typename T>
T _min(T x, T y)
{
	return x>y?y:x;
}
int main()
{
//	using std::cout;
//	using std::endl;
	using namespace std;
	int n1 = 10, n2 = 20;
	float d1 = 12.2 ,d2 = 6.6666;
	cout<< "show min int " << _min(n1, n2) << endl;
	cout<< "show min float " << _min(d1, d2) << endl;
}
