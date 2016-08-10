#include <iostream>

class singleton
{
	protected:
		singleton(){};
	private:
		static int _instance;
	public:
		virtual ~singleton();
		
		static int *getinstance();
};

int singleton::_instance = NULL;

class init
{
	init(){};
	~init(){};
	public:
		static int get_a();
		static const int a;
	
};

const int init::a = 11;
int init::get_a() {return a;};
int main()
{
	std::cout << init::get_a() <<std::endl;
}
