#include <iostream>

double hmean(double a, double b);
double gmean(double a, double b);


class bad_hmean{
private:
	double v1;
	double v2;
public:

	bad_hmean(double a =0, double b = 0) : v1(a), v2(b){}
	void mesg();
};

inline void bad_hmean::mesg(){
	std::cout << "hmean(" << v1 << ", " << v2 << ") :" << "invalid arguments a = -b \n";
}


class bad_gmean{
public:
	double v1;
	double v2;

	bad_gmean(double a =0, double b = 0) : v1(a), v2(b){}
	const char * mesg();
};

inline const char * bad_gmean::mesg(){
	return "gmean() arguments should be >= 0\n";
}

void class_try()
{
	double x,y,z;

	std::cout << "Enter two numbers: \n";

	while(std::cin >> x >> y){
		try{
			z = hmean(x,y);
			std::cout << "Harmonic mean of" << x << " and " << y << " is " << z << std::endl;
			std::cout << "Geometric mean of" << x << " and " << y << " is " << gmean(x, y) << std::endl;
		}
		catch(bad_hmean & bg)
		{
			bg.mesg();
			std::cout << "Try again. \n";
			continue;
		}
		catch(bad_gmean & bg)
				{
					std::cout << bg.mesg();
					std::cout << "Values used:  " <<  bg.v1 << ", " << bg.v2 << std::endl;
					std::cout << "Sorry, you don't get to paly any more.\n";
					break;
				}

		std::cout << "Enter next set of number <q to quit>: ";

 	}
	std::cout << "Bye !!! \n";

}

void my_try()
{
	double x,y,z;
	
	std::cout << "Enter two numbers: \n";

	while(std::cin >> x >> y){
		try{
			z = hmean(x,y);
		}
		catch(const char * s)
		{
			std::cout << s << std::endl;
			std::cout << "Enter a new pair of number: \n";
			continue;
		}
		std::cout << "Harmonic mean of" << x << " and " << y << " is " << z << std::endl;
		std::cout << "Enter next set of number <q to quit>: ";

 	}
	std::cout << "Bye !!! \n";

}

double hmean(double a, double b){
	if(a == -b)
		//throw "bad hmean() arguments: a = not -b allwod";
		throw bad_hmean(a,b);
	return 2.0*a*b/(a+b);

}

double gmean(double a, double b){
	if(a < 0 || b < 0)
		throw bad_gmean(a,b);
	return 2.0*a*b/(a+b);

}
