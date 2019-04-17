#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

namespace transform{
    template<class type, class container>
    container map(container arr, type func(type)){
        auto new_arr = container();
        for(auto it=arr.begin(); it!=arr.end(); it++){
            auto new_ele = func(*it);
            new_arr.insert(new_arr.end(), new_ele);
        }
        return new_arr;
    }

    template<class type, class container>
    container filter(container arr, bool func(type)){
        auto new_arr = container();
        for(auto it=arr.begin(); it!=arr.end(); it++){
            if(func(*it))
                new_arr.insert(new_arr.end(), *it);
        }
        return new_arr;
    }

    template<class type, class container>
    type reduce(container arr, type func(type), type initial){
        for(auto it=arr.begin(); it!=arr.end(); it++){
            initial += func(*it);
        }
        return initial;
    }
}

#endif