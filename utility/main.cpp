#include <iostream>
#include <functional>
#include "type_traits.h"
 
void f1()
{
    std::cout << "reference to function called\n";
}
void f2(int n)
{
    std::cout << "bind expression called with " << n << " as the argument\n";
}
 
int main()
{
    MyStl::reference_wrapper<void()> ref1 = MyStl::ref(f1);
    ref1();

    MyStl::reference_wrapper<void(int)> ref2 = MyStl::ref(f2);
 
    auto b = std::bind(ref2, std::placeholders::_1);
    b(3);
    auto ref = MyStl::ref(b);
    ref(7);
 
    auto c = []{std::cout << "lambda function called\n"; };
    auto ref3 = MyStl::ref(c);
    ref3();
}