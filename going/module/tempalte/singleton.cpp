#include <iostream>

template <typename T> class singleton
{
	protected:
		singleton(){};
	private:
		static T *_instance;
	public:
		virtual ~singleton();
		
		static T *getinstance();
};

template <typename T> T* singleton<T>::_instance = NULL;
