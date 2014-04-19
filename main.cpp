#include "avl.h"
#include <iostream>

int main()
{
    typedef add<nil, 1>::result root;
    typedef add<root, 2>::result root2;
    typedef add<root2, 3>::result root3;
    std::cout << root3::value << std::endl;
    std::cout << root3::left::value << std::endl;
    std::cout << root3::right::value << std::endl;
    typedef add<root3, 4>::result root4;
    typedef avl_delete<root4, 1>::result deleted;
    std::cout << '\n' << std::endl;
    std::cout << deleted::value << std::endl;
    std::cout << deleted::left::value << std::endl;
    std::cout << deleted::right::value << std::endl;
}
