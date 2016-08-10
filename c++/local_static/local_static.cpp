#include <iostream>
class test{
	public:
	test(int i = 0){
		std::cout << "start\n";
	}
	~test(){
		std::cout << "end\n";
	}
	void show(){
		std::cout << "i = " << i++ << std::endl;
	}
	private:
	int i;
};

test & see()
{
	static test fs(1);
	return fs;
}
int main()
{
	see().show();
	see().show();
	see().show();
	see().show();
	see().show();
	see().show();
}
