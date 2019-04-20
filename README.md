# C++ Transformations
Map, Reduce and Filter operations for standard C++ containers

**Note:** This is a fork that tries to parallelize the tranforms using OpenMP. You can find more details on my [blog post](https://nvnmo.github.io/blog/2019/04/19/openmp-parallel-for-cpp).

You can find the original repo [here](https://github.com/georgeshanti/cpp-transformations).

## Map

#### Example:
    #include<iostream>
    #include<vector>
    #include "transform.hpp"

    int multiplyByTwo(int i){
        return i*2;
    }

    int main(){
        std::vector<int> arr{1,2,3,4,5};
        for(auto i:arr){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
        auto new_arr = transform::map<int,std::vector<int>>(arr, multiplyByTwo);
        for(auto i:new_arr){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
        return 0;
    }

#### Output:
    1 2 3 4 5
    2 4 6 8 10

## Reduce

#### Example:
    #include<iostream>
    #include<vector>
    #include "transform.hpp"

    int multiplyByTwo(int i){
        return i*2;
    }

    int main(){
        std::vector<int> arr{1,2,3,4,5};
        for(auto i:arr){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
        auto val = transform::reduce<int,std::vector<int>>(arr, multiplyByTwo, 0);
        std::cout<<val<<std::endl;
        return 0;
    }

#### Output:
    1 2 3 4 5
    30

## Filter

#### Example:
    #include<iostream>
    #include<vector>
    #include "transform.hpp"

    bool divisibleByTwo(int i){
        return i%2==0;
    }

    int main(){
        std::vector<int> arr{1,2,3,4,5,6};
        for(auto i:arr){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
        auto new_arr = transform::filter<int,std::vector<int>>(arr, divisibleByTwo);
        for(auto i:new_arr){
            std::cout<<i<<" ";
        }
        std::cout<<std::endl;
        return 0;
    }

#### Output:
    1 2 3 4 5
    2 4 6 8 10