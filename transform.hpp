#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

namespace transform{
    template<class type, class container>
    container map(container arr, type func(type)){
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

    template<class type, class container>
    container filter(container arr, bool func(type)){
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

    template<class type, class container>
    type reduce(container arr, type func(type), type initial){
        size_t len = arr.size();
        auto it = arr.begin();
        #pragma omp parallel for schedule(static) reduction(+:initial)
        for(size_t i = 0; i < len; i++){
            initial += func(*(it+i));
        }
        return initial;
    }
}

#endif
