#include<iostream>
#include<vector>
#include<iterator>
#include "ap_error.hpp"

template< typename stack_pool, typename T, typename N = std::size_t >
/**
 * @brief Class iterator
 * 
 * @param n index
 * @param pool
 */
class _iterator{
  N current;  
  stack_pool* pool;

 public:
  using value_type = T;                                 //type of value obtain by deference the iterator
  using reference = value_type&;                        
  using pointer = value_type*;    
  using difference_type = std::ptrdiff_t;               //type that can be used to identify the distance between iterators
  using iterator_category = std::forward_iterator_tag;  //type of iterator

  /**
   * @brief Construct a new iterator object
   * 
   * @param x  index
   * @param p current (this) pointer to the pool of stacks
   */
  _iterator(N x, stack_pool* p) noexcept: current{x}, pool{p} {} 
    
  /**
   * @brief Dereferencing operator
   * 
   * @return reference 
   */
  reference operator*() const { 
    AP_ERROR(!(pool -> empty(current)))<<"The stack is empty"<<std::endl;
    return pool -> value(current);
  }

  /**
   * @brief Pre-increment operator
   * 
   * @return _iterator& 
   */
  _iterator& operator++() {  // pre-increment
    AP_ERROR(!(pool -> empty(current)))<<"The stack is empty"<<std::endl;
    current = pool -> next(current);
    return *this;
  }

  /**
   * @brief Post-increnoexcment operator
   * 
   * @return _iterator 
   */
  _iterator operator++(int) {  // post-increment
    AP_ERROR(!(pool -> empty(current)))<<"The stack is empty"<<std::endl;
    auto tmp = *this;
    ++(*this);
    return tmp;
  }
  
  /**
   * @brief Equality operator
   * 
   * @param x 
   * @param y 
   * @return true 
   * @return false 
   */
  friend bool operator==(const _iterator& x, const _iterator& y) noexcept{
    return x.current == y.current;
  }

  friend bool operator!=(const _iterator& x, const _iterator& y) noexcept{
    return !(x == y);
  }
};






template <typename T, typename N = std::size_t> 
class stack_pool{ 

  /**
   * @brief data type node with a value and the pointer to the previous node
   * 
   */
  struct node_t{
    T value;
    N next; 
    
    /**
     * @brief Construct a new node t object
     * 
     * @param v is the value
     * @param n is the index of the pool
     */
    node_t(T v, N n) noexcept: value{v}, next{n} {}
    ~node_t() noexcept = default;
  };
  
  std::vector<node_t> pool; 
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type; //unsigned int (> 0)
  
  stack_type free_nodes; // at the beginning, it is empty
  
  /**
   * @brief get the node from index x that is positioned in the vector pool at x-1
   * 
   * @param x index
   * @return node_t& 
   */
  node_t& node(stack_type x) noexcept { return pool[x-1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

  /**
   * @brief General function that accept both r-value and l-value
   * 
   * 
   * @tparam X 
   * @param val Forwarding reference 
   * @param head Stack to insert the value
   * @return new stack_type 
   */
  template<typename X>
  stack_type _push( X&& val, stack_type head){ 

    node_t node{std::forward<X>(val), head};

    if (!empty(free_nodes)){
      pool[ free_nodes-1 ] = node;
      head = free_nodes; 
      free_nodes = next( free_nodes );
      return head;
    }else{
      pool.push_back(node);
      head = pool.size();  //(DIM - 1) + 1
      return head;
    }
  }

  public:

  /**
   * @brief Default constructor of a new stack pool object and reserve zero
   * 
   */
  stack_pool() noexcept: pool{}, free_nodes{stack_type(0)} {
    pool.reserve(0);
    }
  explicit stack_pool(size_type n): pool{}, free_nodes{stack_type(0)}{pool.reserve(n);} // reserve n nodes in the pool. call pool.reserve
   
  /**
   * @brief Copy constructor and Move constructor
   * 
   */
  stack_pool(const stack_pool&) = default;
  stack_pool(stack_pool&&) noexcept = default;

  /**
   * @brief Copy assignment and move assignment
   * 
   * @return stack_pool& 
   */
  stack_pool& operator=(const stack_pool&) = default;
  stack_pool& operator=(stack_pool&&) noexcept = default;

  ~stack_pool() noexcept=default;

    
  using iterator = _iterator< stack_pool<value_type, stack_type>, value_type, stack_type>;
  using const_iterator = _iterator< const stack_pool<value_type, stack_type>, const value_type, stack_type>; //only stack tupe can change

  /**
   * @brief Begin and end iterators
   * 
   * @param x stack_type
   * @return iterator 
   */
  iterator begin(stack_type x){ return iterator{ x, this }; }  //return head
  iterator end(stack_type ) { return iterator{ end(), this }; } // this is not a typo //it will returno 0
  //returnn std::size(0)
  //input only stack_type
    
  const_iterator begin(stack_type x) const {return const_iterator{x, this};} 
  const_iterator end(stack_type ) const {return const_iterator{end(), this};}
  
  const_iterator cbegin(stack_type x) const {return const_iterator{x, this};}
  const_iterator cend(stack_type ) const {return const_iterator{end(), this};}
    
  /**
   * @brief Function to create a new empty stack
   * 
   * @return stack_type empty stack
   */
  stack_type new_stack() noexcept {
    return end();
  }

  /**
   * @brief Reserve n memory using the function of std::vector
   * 
   * @param n size_type
   */
  void reserve(size_type n){
    pool.reserve(n);
  }

  /** 
   * @return The capacity of the vector 
   */
  size_type capacity() const noexcept {
    return pool.capacity();
  }

  /**
   * @brief Check if the stack is empty
   * 
   * @param x stack_type
   * @return true 
   * @return false 
   */
  bool empty(stack_type x) const noexcept {
    return x == end();
  }

  /**
   * @brief The end of each stack is always the index zero
   * 
   * @return stack_type 
   */
  stack_type end() const noexcept { 
    return stack_type(0); 
  }

  /**
   * @brief Used to return the value 
   * 
   * @param x stack_tipy
   * @return T& 
   */
  T& value(stack_type x){
    check_empty(x);
    return node(x).value;
  }

  const T& value(stack_type x) const {
    check_empty(x);
    return node(x).value;
  }

  /**
   * @brief Used to return the next node
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

  /**
   * @brief Function to check if the stack is empty
   * 
   * @param x 
   */
  void check_empty(stack_type x){
    AP_ERROR( !empty(x) )<<"The stack is empty"<<std::endl;
  }

 /**
  * @brief Push methods with l-value or r-value references
  * 
  * @param val 
  * @param head 
  * @return stack_type 
  */
  stack_type push(const T& val, stack_type head){
    return _push(val, head);
  } //lvalue
 
  stack_type push(T&& val, stack_type head){
    return _push(std::move(val), head);
  } //rvalue
 
  //pop return the new head
  /**
   * @brief Used to remove the last node of the stack
   * 
   * @param x 
   * @return stack_type 
   */
  stack_type pop(stack_type x){

    check_empty(x);
    auto new_head = next(x);
    pool[x - 1].next = free_nodes;    
    free_nodes = x;                   
    return new_head;
  
  }

  /**
   * @brief Remove all nodes of a stack
   * All removed nodes will be part of the free_nodes stack
   * 
   * @param x 
   * @return stack_type 
   */
  stack_type free_stack(stack_type x){
    
    while(x!=end())
      x = pop(x);
    
    return x;
  }

  /**
   * @brief Print all elements of a stack
   * 
   * @param os std::ostream, pass all types of ostream
   * @param x 
   */
  void print_stack(std::ostream& os, const stack_type x) {
    check_empty(x);
    for (auto i = begin(x); i != end(x); ++i)
      os<<"value: "<< *i<<"\n";
    }

  /**
   * @brief Number of nodes on the stack
   * 
   * @param x 
   * @return std::size_t 
   */
  std::size_t elem_stack(const stack_type x){
    std::size_t count{0};
    for (auto i = begin(x); i != end(x); ++i)
      ++count;
    return count;
  }


};
