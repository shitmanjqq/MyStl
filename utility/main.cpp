#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include "tuple.h"

MyStl::tuple<double, char, std::string> get_student(int id)
{
    if (id == 0)
        return MyStl::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1)
        return MyStl::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2)
        return MyStl::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

std::tuple<double, char, std::string> get_std_student(int id)
{
    if (id == 0)
        return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1)
        return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2)
        return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

int main()
{
    auto student0 = get_student(0);
    std::cout << "ID: 0, "
              << "GPA: " << MyStl::get<0>(student0) << ", "
              << "grade: " << MyStl::get<1>(student0) << ", "
              << "name: " << MyStl::get<2>(student0) << '\n';

    double gpa1;
    char grade1;
    std::string name1;
    MyStl::tie(gpa1, grade1, MyStl::ignore) = get_student(1);

    // MyStl::tuple<double &, char &, std::string &> rt{gpa1, grade1, name1};
    // auto rr = get_student(1);
    // rt = rr;

    std::cout << "ID: 1, "
              << "GPA: " << gpa1 << ", "
              << "grade: " << grade1 << ", "
              << "name: " << name1 << '\n';

    // C++17 structured binding:
    // auto [ agpa2, agrade2, aname2 ] = get_student(2);
    auto [gpa2, grade2, name2] = get_student(2);
    // MyStl::tell_type<decltype(name2)> t;
    std::cout << "ID: 2, "
              << "GPA: " << gpa2 << ", "
              << "grade: " << grade2 << ", "
              << "name: " << name2 << '\n';
}