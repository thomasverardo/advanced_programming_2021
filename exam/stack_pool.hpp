#include<iostream>
#include<vector>
#include<iterator>
#include "ap_error.hpp"


template< typename stack_pool, typename T, typename N = std::size_t >
class _iterator{
  N current;  
  stack_pool* pool;

 public:
  using value_type = T;
  using reference = value_type&; 
  using pointer = value_type*;    
  using difference_type = std::ptrdiff_t; 
  using iterator_category = std::forward_iterator_tag;

  _iterator(N x, stack_pool& p) : current{x}, pool{&p} {} 
    


  reference operator*() const { 
    AP_ERROR(!(pool -> empty(current)))<<"The stack is empty"<<std::endl;
    return pool -> value(current);                       
  }

  _iterator& operator++() {  // pre-increment
    AP_ERROR(!(pool -> empty(current)))<<"The stack is empty"<<std::endl;
    current = pool -> next(current);
    return *this;
  }

  _iterator operator++(int) {  // post-increment
    AP_ERROR(!(pool -> empty(current)))<<"The stack is empty"<<std::endl;
    auto tmp = *this;
    ++(*this);
    return tmp;
  }
  
  friend bool operator==(const _iterator& x, const _iterator& y) noexcept{
    return x.current == y.current;
  }

  friend bool operator!=(const _iterator& x, const _iterator& y) noexcept{
    return !(x == y);
  }
};






template <typename T, typename N = std::size_t> 
class stack_pool{ 

  struct node_t{
    T value;
    N next; 

    node_t() noexcept = default;
    node_t(T v, N n) : value{v}, next{n} {}
    ~node_t()=default;
  };
  
  std::vector<node_t> pool; 
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type; //unsigned int (> 0)
  
  stack_type free_nodes; // at the beginning, it is empty
  
  node_t& node(stack_type x) noexcept { return pool[x-1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

  template<typename X>
  stack_type _push( X&& val, stack_type head){ 

    node_t node(std::forward<X>(val), head);

    if (!empty(free_nodes)){
      pool[free_nodes-1] = node;
      head = free_nodes; 
      free_nodes = next(free_nodes);
      return head;
    }else{
      pool.push_back(node);
      head = pool.size();  //(DIM - 1) + 1
      return head;
    }
  } //lvalue

  public:
  stack_pool() : pool{}, free_nodes{stack_type(0)} {pool.reserve(0);} //constructor: empty vector e free nodes uguale a zero

  explicit stack_pool(size_type n): pool{}, free_nodes{stack_type(0)}{pool.reserve(n);} // reserve n nodes in the pool. call pool.reserve
   

  ~stack_pool()=default;

    
  using iterator = _iterator< stack_pool<value_type, stack_type>, value_type, stack_type>;
  using const_iterator = _iterator< const stack_pool<value_type, stack_type>, const value_type, stack_type>; //only stack tupe can change

  iterator begin(stack_type x){ return iterator{ x, *this }; }  //return head
  iterator end(stack_type ) noexcept { return iterator{ end(), *this }; } // this is not a typo //it will returno 0
  //returnn std::size(0)
  //input only stack_type
    
  const_iterator begin(stack_type x) const {return const_iterator{x, *this};} 
  const_iterator end(stack_type ) const noexcept {return const_iterator{end(), *this};}
  
  const_iterator cbegin(stack_type x) const {return const_iterator{x, *this};}
  const_iterator cend(stack_type ) const noexcept {return const_iterator{end(), *this};}
    
  stack_type new_stack() noexcept {return end();} // return an empty stack (return end();)

  void reserve(size_type n){pool.reserve(n);} // reserve n nodes in the pool
  size_type capacity() const noexcept {return pool.capacity();} // the capacity of the pool

  bool empty(stack_type x) const noexcept {return x == end();} //if x == end

  stack_type end() const noexcept { return stack_type(0); }

  // Example of use: pool.value(l) = 77;
  // return node(x).value;
  T& value(stack_type x){
    check_empty(x);
    return node(x).value;
  }

  const T& value(stack_type x) const {
    check_empty(x);
    return node(x).value;
  }

  // return node(x).next;
  /**
   * @brief 
   * 
   * @param x 
   * @return stack_type& 
   */
  stack_type& next(stack_type x){
    check_empty(x);
    return node(x).next;
  }
  
  const stack_type& next(stack_type x) const{
    check_empty(x);
    return node(x).next;
  }

  void check_empty(stack_type x){
    AP_ERROR( !empty(x) )<<"The stack is empty"<<std::endl;
  }

 //integer small than 8 bytes and pass by value
 //so no &head
  stack_type push(const T& val, stack_type head){ //mettere check_capacity()
    return _push(val, head);
    } //lvalue
 
  
  stack_type push(T&& val, stack_type head){ //head=1+idx
    return _push(std::move(val), head);
    } //rvalue
 
  //pop return the new head
  stack_type pop(stack_type x){ //x è head

    check_empty(x);
    auto new_head = next(x);
    pool[x - 1].next = free_nodes;    // Prendere il nodo e sostituire il next con ultimo free_nodes
    free_nodes = x;                    // cambiare il free_nides
    return new_head;                  // ritornare il nuovo head della pila
  } // delete first node

  stack_type free_stack(stack_type x){
    
    while(x!=end())
      x = pop(x);
    
    return x;
  } // free entire stack //delete the entire stack


    //input ostream if we want to write on file
  void print_stack(std::ostream& os, const stack_type x) {
    check_empty(x);
    // for (auto i : x)
    for (auto i = begin(x); i != end(x); ++i)
      os<<"value: "<< *i<<"\n";
    }


  std::size_t elem_stack(const stack_type x){
    std::size_t count{0};
    // for (auto i : x)
    for (auto i = begin(x); i != end(x); ++i)
      ++count;
    return count;
  }


};
