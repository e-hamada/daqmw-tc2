#ifndef _MYCLASS
#define _MYCLASS

#include <iostream>

class MyClass
{
public:
	MyClass();
	MyClass(int x, int y);
	virtual ~MyClass();
	int set_x(int x);
	int set_y(int y);
	int set_z(int z);
	int get_x();
	int get_y();
	int get_z();
private:
	int m_x;
	int m_y;
    int m_z;

};

#endif
