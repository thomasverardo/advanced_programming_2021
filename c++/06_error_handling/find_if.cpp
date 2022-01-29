#include <vector>
#include <iostream>

//if u find something, u turn an iterator

template <typename I, typename T>
I find(I first, I last, T value){ 
    if(first == last) return last; //if empty, return nothing

    
    // while(first != last){
    //     if(*first == value) return first;
    //     ++first;
    // }
    // return last;

    //OR
    //while(first != last && *first != value) ++first;

    //But, if you know that you WILL find the value..
    //while(*first != value) ++first;

    //OR (with sentinel)
    --last;
    auto tmp = std::move(*last);
    if (tmp == value) return last;
    *last = value;

    while(*first != value) ++first;
    if( first == last) return ++last;

    return first;
}




int main(){
    std::vector<int> v;
    for(auto i=0; i<10; ++i){
        v.emplace_back(i);
    }

    auto x = find(v.begin(), v.end(), 9);
    if(x==v.end()){
        std::cout << "not found" << std::endl;
    } else std::cout<< "found "<<*x<<std::endl;

    return 1;
}