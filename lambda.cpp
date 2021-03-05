// compile with: 
//    g++ -std=c++17 lambda.cpp

#include <iostream>
#include <functional>
#include <vector>
#include <numeric>


//successor function
// succ : int -> int 
auto succ = [](int x){ 
   return x+1;
};

//head type is polymorphic; 
//head : vec<α> -> α 
auto head = [](std::vector<auto> v){
  return v.front();
};


//function composition
// dot : (β -> η, α -> β) -> α -> η 
auto dot = [](auto f,auto g) {
   return [f,g](auto x)  {
      return f(g(x));
   };
};

// map : (α -> β) -> vec<α> -> vec<β> 
auto map = [](auto f) {
   return [f](std::vector<auto> v){
      auto v2 = v;
      std::transform(v.begin(), v.end(),v2.begin(),f);
      return v2;
   };
};

// reduce : (α -> α -> α) -> α -> vec<α> -> vec<α> 
auto reduce = [](auto bin_op) {
   return [bin_op](auto neutral_elem){
      return [bin_op,neutral_elem](std::vector<auto> v){
         return std::accumulate(v.begin(), v.end(),neutral_elem,bin_op);
      };
   };
};

int main()
{
    using namespace std;


    vector<int> vec;
    vector<vector<int>> vec_vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec_vec.push_back(vec);

    cout << head(vec) << endl;            //10
    cout << head(map(succ)(vec)) << endl; //11    

    auto plus = [](int x,int y){ return x+y; };
    auto adder = reduce(plus)(0);

    cout << adder(vec) << endl;            // 10 + 20 + 30 = 60
    cout << adder(map(succ)(vec)) << endl; // 11 + 21 + 31 = 63
    

    //dot can compose head applications:
    // ∀ x. dot(head,head)(x) == head(head(x)) 
    cout << head(head(vec_vec)) << endl;  //10
    cout << dot(head,head)(vec_vec) << endl; //10

    //list fusion equivalence:
    // ∀ f g l.
    //    dot(map f,map g) (l) = map(dot(f,g)) (l)
    auto lhs = dot(map(succ),map(succ))(vec);
    auto rhs = map(dot(succ,succ))(vec);


    cout << ( head(lhs) == head(rhs) ) <<endl; //true (1)
    
    return 0;
}

