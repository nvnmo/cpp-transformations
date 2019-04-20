#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <typeinfo>
#include <vector>
#include <list>

namespace transform{

template<typename T,typename U>
struct is_same{
    static const bool result = false;
};

template<typename T>
struct is_same<T,T>{
    static const bool result = true;
};

template<typename T,typename U>
constexpr bool eqType(){
    return is_same<T,U>::result;
}

template<typename T1, typename T2>
constexpr bool is_parallel_suppoted_type(){
    return eqType<std::vector<T1>,T2>();
}

/* Seperate implementation class since C++ does not allow
    * partial function template specialization (yet)
    */
template<typename type, typename container, bool parallel>
struct map_impl{};

/* Only certain containers are supported by this parallel 
    * implementation. Therefore, we can use partial specialization to
    * make the decision at compile time leaving no runtime penalties.
    */

// parallel implementation
template<typename type, typename container>
struct map_impl<type,container,true>{
    static container map(container& arr, type func(type)){
        auto new_arr = container(arr.size());
        size_t len = arr.size();
        auto it = arr.begin();
        auto new_it = new_arr.begin();
        #pragma omp parallel for schedule(static)
        for(size_t i = 0; i < len; i++){
            auto new_ele = func(*(it+i));
            *(new_it+i) = new_ele;
        }
        return new_arr;
    }
};

// normal implementation
template<typename type, typename container>
struct map_impl<type,container,false>{
    static container map(container& arr, type func(type)){
        auto new_arr = container();
        for(auto it=arr.begin(); it!=arr.end(); it++){
            auto new_ele = func(*it);
            new_arr.insert(new_arr.end(), new_ele);
        }
        return new_arr;
    }
};

template<typename type, typename container, bool parallel>
struct filter_impl{};

// parallel implementation
template<typename type, typename container>
struct filter_impl<type,container,true>{
    static container filter(container& arr, bool func(type)){
        auto new_arr = container();
        size_t len = arr.size();
        auto it = arr.begin();
        #pragma omp parallel for schedule(static)
        for(size_t i = 0; i < len; i++){
            if(func(*it)){
                #pragma omp critical                
                {new_arr.insert(new_arr.end(), *(it+i));}
            }
        }
        return new_arr;
    }
};

// normal implementation
template<typename type, typename container>
struct filter_impl<type,container,false>{
    static container filter(container& arr, bool func(type)){
        auto new_arr = container();
        for(auto it=arr.begin(); it!=arr.end(); it++){
            if(func(*it))
                new_arr.insert(new_arr.end(), *it);
        }
        return new_arr;
    }
};

template<typename type, typename container, bool parallel>
struct reduce_impl{};

// parallel implementation
template<typename type, typename container>
struct reduce_impl<type,container,true>{
    static type reduce(container& arr, type func(type), type initial){
        size_t len = arr.size();
        auto it = arr.begin();
        #pragma omp parallel for schedule(static) reduction(+:initial)
        for(size_t i = 0; i < len; i++){
            initial += func(*(it+i));
        }
        return initial;
    }
};

// normal implementation
template<typename type, typename container>
struct reduce_impl<type,container,false>{
    static type reduce(container& arr, type func(type), type initial){
        for(auto it=arr.begin(); it!=arr.end(); it++){
            initial += func(*it);
        }
        return initial;
    }
};


template<typename type, typename container>
container map(container& arr, type func(type)){
    return map_impl<type,container,is_parallel_suppoted_type<type,container>()>::map(arr,func);
}

template<class type, class container>
container filter(container& arr, bool func(type)){
    return filter_impl<type,container,is_parallel_suppoted_type<type,container>()>::filter(arr,func);
}

template<class type, class container>
type reduce(container& arr, type func(type), type& initial){
    return reduce_impl<type,container,is_parallel_suppoted_type<type,container>()>::reduce(arr,func,initial);
}

template<class type, class container>
type reduce(container& arr, type func(type), type&& initial){
    return reduce_impl<type,container,is_parallel_suppoted_type<type,container>()>::reduce(arr,func,initial);
}

} // transform

#endif