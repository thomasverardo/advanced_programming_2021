#include <iostream>

struct Base {
  Base() { std::cout << "Base\n"; }
  ~Base() { std::cout << "~Base\n"; }
};

struct Derived : public Base {
  Derived() { std::cout << "Derived\n"; }
  ~Derived() { std::cout << "~Derived\n"; }
};

int main() {
  { Derived d; }  
  //costructor: top to bottom --> first base
  //desctructor: bottom to top --> first derived

  std::cout << "\n\npointers\n";
  Base* p = new Derived;
  delete p;

  return 0;
}

//sizeof (child) >= sizeof(parent)