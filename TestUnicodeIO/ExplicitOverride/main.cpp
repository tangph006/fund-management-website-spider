#include <stdio.h>

class base1
{
public:
    virtual void fun1()
    {
        printf("base1::fun1()\r\n");
    }
};

class base2
{
public:
    virtual void fun1()=0;
};

class sub : public base1, public base2
{
public:

    virtual void fun1()
    {
        printf("sub::fun1()\r\n");
    }
};

class sub2 : public sub
{
public:
    virtual void fun1()
    {
        base1::fun1();
    }
};

void main()
{
    sub2* p1 = new sub2;
    base2* p2 = dynamic_cast<base2*>(p1);

    p1->fun1();
    p2->fun1();
}
