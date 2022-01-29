#include <iostream>
#include <memory>
#include <chrono>

template <typename ET>
struct MatrixExpression{
  auto operator[](const std::size_t i) const noexcept{return static_cast<const ET&>(*this)[i];}
}

template <typename T>
class Matrix{
  std::size_t _rows;
  std::size_t _cols;
  std::unique_ptr<T[]> elem;
  
public:

  auto rows() const noexcept { return _rows; }
  auto cols() const noexcept { return _cols; }

  template<typename ET>
  Matrix(const MatrixExpression<ET>& e)
  
  Matrix(const std::size_t r,const std::size_t c) //respect order of declaration, how initialize elem if we don't know rows and cols ? 
    : _rows{r},_cols{c},elem{new T[_rows*_cols]} { //don't put noexcept, there is new ! 
  }
  
  // explicit Matrix(const std::size_t n) //avoid call duplication, in order to have less code
  //   :_rows{n},_cols{n},elem {n*n} {
  // }

  explicit Matrix(const std::size_t n)
    : Matrix{n,n} { //this is a delegating constructor
  }

  Matrix(const Matrix& x)
    : Matrix{x._rows,x._cols} { //again delegate constructor
    std::cout<<" copty constructor"<< std::endl;
    std::copy(x.elem.get(),x.elem.get()+_rows*_cols,elem.get()); //or we can use allocator
    
  }

  Matrix& operator=(const Matrix& x){
    elem.reset(); //release the memory
    auto tmp{x}; // COPY constructor ! 
    (*this)  = std::move(tmp);
    return *this;
  }

  //  Matrix(Matrix&&) noexcept = default; //default can be ok, we have smart pointer
  Matrix(Matrix&& x) noexcept//default can be ok, we have smart pointer
    :_rows{x._rows},_cols{x._cols},elem{std::move(x.elem)}{
    std::cout<<"move ctor"<<std::endl;
  }
  
  T& operator[](const std::size_t i) noexcept {return elem[i];}
  const T& operator[](const std::size_t i) const noexcept {return elem[i];}

  T& operator()(const std::size_t i, const std::size_t j){

    //AP_ASSERT_LT(i,_rows);
    //AP_ASSERT_LT(j,_jcols);
    return (*this)[i*_cols+j];
  }

  friend std::ostream& operator<<(std::ostream& os,const Matrix& m){
    for(std::size_t i=0;i<m._rows;++i){
      for(std::size_t j=0;j<m._cols;++j)
	      //os << m(i,j) << " ";
        os << m[i,j] << " ";
      os << std::endl;
    }
    return os;
  }

  Matrix& operator += (const Matrix){

    for(std::size_t i=0; i<_rows*_cols; ++i){
      (*this)[i] += b[i];

    return *this;
    }
  }


  friend Matrix operator+(Matrix a, const Matrix& b){
    return std::forward<Matrix(a+=b);
  }


  //buggy?
  // friend Matrix operator+(const Matrix& a, const Matrix& b){
  //   //AP_ASSERT_EQ(a._rows, b._rows);
  //   //AP_ASSERT_EQ(a._cols, b._cols);

  //   Matrix res{a._rows, a._cols};
  //   for(std::size_t i=0; i<a._rows*a._cols; ++i){
  //     res[i] = a[i] + b[i];
  //   }

  //   return res;
    
  // }

  //friend Matrix operator+(const Matrix& a, const Matrix& b){

};


template <typename L, typename R>
class MatrixSum{
  const L& l;
  const R& r;

  public:
  MatrixSum(const L& l, const R& r): l{l}, r{r}{}
  auto operator [](const std::size_t) const noexcept {return l[i] + r[i];}
};

  template
  auto operator+(const MatrixExpression<ET>& e, const Matrix<T>& b){
    return MatrixSum<MatrixExpression<ET>, Matrix<T>>{e,b};
  }

  template <typename T>
  auto operator+(const Matrix<T>& a, const Matrix<T>& b){
    return MatrixSum<Matrix<T>>, Matrix<T>>{a,b};
  }
   

  


int main() {

  constexpr std::size_t dim{10};
  Matrix<int> M{dim};

  for(int i=0;i<dim*dim;i++)
    M[i]=i;

  Matrix<int> m1 = M;

  auto t0 = std::chrono::high_resolution_clock::now();
  auto res = m1+m1+m1+m1+m1+m1+m1+m1+m1; //when we use expression template, we can't use this, but below
  Matrix<int> res = m1+m1+m1+m1+m1+m1+m1+m1+m1;
  auto t1 = std::chrono::high_resolution_clock::now();

  std::cout<<m1;




      
  // std::cout<<M;
}
