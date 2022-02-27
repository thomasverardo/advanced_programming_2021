#include<iostream>
#include<vector>
#include "ap_error.hpp"
#include"stack_pool.hpp"



int main(){

    stack_pool<int, uint16_t> pool{};
    stack_pool<int, uint16_t> pool2{};
    auto l1 = pool.new_stack();
    
    l1 = pool.push(3, l1);
    l1 = pool.push(1, l1);
    l1 = pool.push(4, l1);
    l1 = pool.push(1, l1);
    l1 = pool.push(5, l1);
    l1 = pool.push(9, l1);
    l1 = pool.push(2, l1);
    l1 = pool.push(6, l1);
    l1 = pool.push(5, l1);
    l1 = pool.push(3, l1);
    l1 = pool.push(5, l1);
    
    auto l2 = pool.new_stack();
    l2 = pool.push(8, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(7, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(3, l2);
    l2 = pool.push(1, l2);
    l2 = pool.push(1, l2);
    l2 = pool.push(5, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(9, l2);
    l2 = pool.push(7, l2);

    pool.value(l1) = 77;

    std::cout<<pool.value(l1);

    // l2=pool.pop(l2);


    auto x=pool.cbegin(l2);


    pool.print_stack(std::cout, l1);

    std::cout<<"the number of elements in the stack is "<<pool.elem_stack(l2)<<std::endl;


    // pool2 = pool;
    
    // pool2.print_stack(std::cout, l2);

}