#ifndef _S_INDEX_HPP
#define _S_INDEX_HPP


#include <array>
#include <list>
#include "domain/domain_define.hpp"

namespace carpio{

typedef int Idx;

template<St DIM>
class SIndex_: public std::array<int, DIM> {
public:
    static const St Dim = DIM;
    typedef SIndex_<DIM> Self;
public:
    SIndex_() {
        this->fill(0);
    }
    SIndex_(int a, int b = 0, int c = 0) {
        this->at(0) = a;
        if (Dim >= 2) {
            this->at(1) = b;
        }
        if (Dim >= 3) {
            this->at(2) = c;
        }
    }
    SIndex_(const SIndex_<DIM>& other) {
        for (St d = 0; d < Dim; ++d) {
            this->at(d) = other[d];
        }
    }
    int i() const {
        return this->at(0);
    }
    int j() const {
        return (Dim >= 2) ? this->at(1) : 0;
    }
    int k() const {
        return (Dim >= 3) ? this->at(2) : 0;
    }

    // plus 1
    // Self p(St dim, St step = 1) {
    //     Self res(*this);
    //     if (dim == 0) {
    //         res[0] += step;
    //     }else if (dim == 1) {
    //         res[1] += step;
    //     }else { // dim == 2
    //         res[2] += step;
    //     }
    //     return res;
    // }
    Self p(St dim, unsigned int step = 1) const {
        Self res(*this);
        if (dim == 0) {
            res[0] += step;
        }
        if (dim == 1) {
            res[1] += step;
        }
        if (dim == 2) {
            res[2] += step;
        }
        return res;
    }
    // Self m(St dim) {
    //     Self res(*this);
    //     if (dim == 0) {
    //         res[0] -= 1;
    //     }
    //     if (dim == 1) {
    //         res[1] -= 1;
    //     }
    //     if (dim == 2) {
    //         res[2] -= 1;
    //     }
    //     return res;
    // }
    Self m(St dim, unsigned int step = 1) const {
        Self res(*this);
        if (dim == 0) {
            res[0] -= step;
        }
        if (dim == 1) {
            res[1] -= step;
        }
        if (dim == 2) {
            res[2] -= step;
        }
        return res;
    }
    // high level shift
    Self shift(St dim, const Orientation& ori) const{
        // dim 0 1 2
        //     x y z
        // ori 0 1 2
        //     m p c
        ASSERT(ori == 0 || ori == 1);
        if (ori == 0) {
            return this->m(dim);
        } else {
            return this->p(dim);
        }
    }
    Self shift(St dim, int step) const{
        // dim 0 1 2
        //     x y z
        // ori 0 1 2
        //     m p c
        if (step < 0) {
            return this->m(dim, std::abs(step));
        } else if (step > 0){
            return this->p(dim, std::abs(step));
        }else{
            return (*this);
        }
    }
    // Self shift(St dim, St ori) const {
    //     // dim 0 1 2
    //     //     x y z
    //     // ori 0 1 2
    //     //     m p c
    //     ASSERT(ori == 0 || ori == 1 || ori == 2);
    //     if (ori == 0) {
    //         return this->m(dim);
    //     } else if (ori == 1) {
    //         return this->p(dim);
    //     } else {
    //         return (*this);
    //     }
    // }

    int value(St i) const {
        if (i < Dim) {
            return this->at(i);
        } else {
            return 0;
        }
    }

    void set(St i, int val) {
        if (i < Dim) {
            this->at(i) = val;
        }
    }

    bool operator==(const SIndex_<DIM>& other) const {
        bool res = true;
        for (St d = 0; d < Dim; ++d) {
            res = res && this->at(d) == other[d];
            if (!res) {
                return res;
            }
        }
        return res;
    }

    int operator()(const St& dim) const {
        return dim < DIM ? this->at(dim) : 0;
    }

    void show() const {
        std::cout << "(";
        for (St d = 0; d < Dim; ++d) {
            std::cout << this->at(d);
            if (d != Dim - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")";
    }
};
template<St DIM>
struct SIndex_compare_ {
    typedef SIndex_<DIM> Index;
    bool operator()(const Index& a, const Index& b) const {
        for (St d = 0; d < DIM; d++) {
            if (a[d] < b[d]) {
                return true;
            } else if (a[d] == b[d]) {
                continue;
            } else {
                return false;
            }
        }
        return false;
    }
};

template<St DIM>
struct SIndex_hash_ {
    typedef SIndex_<DIM> Index;
    std::size_t operator()(const Index& a) const {
        std::size_t res[DIM];
        for (St d = 0; d < DIM; d++) {
            res[d] = std::hash<St> { }(a[d]);
        }
        switch (DIM) {
        case 1: {
            return res[0];
            break;
        }
        case 2: {
            return res[0] ^ (res[1] << 1);
            break;
        }
        case 3: {
            return res[0] ^ ((res[1] << 1) >> 1) ^ (res[2] << 1);
            break;
        }
        }
        return false;
    }
};

template<St DIM>
std::ostream& operator<<(std::ostream& stream, const SIndex_<DIM>& index) {
    stream << "(";
    for (St d = 0; d < DIM; ++d) {
        stream << index[d];
        if (d != DIM - 1) {
            stream << ", ";
        }
    }
    stream << ")";
    return stream;
}
template<St DIM>
void Shift(SIndex_<DIM>& index, St dim, St ori, St step = 1) {
    if(ori == _M_){
        index[dim] = index[dim] - step;
    }else if(ori == _P_){
        index[dim] = index[dim] + step;
    }
}
template<St DIM>
SIndex_<DIM> Plus(const SIndex_<DIM>& index, St dim, St step = 1) {
    SIndex_<DIM> res(index);
    res[dim] = index[dim] + step;
    return res;
}
template<St DIM>
SIndex_<DIM> Minus(const SIndex_<DIM>& index, St dim, St step = 1) {
    SIndex_<DIM> res(index);
    res[dim] = index[dim] - step;
    return res;
}
template<St DIM>
SIndex_<DIM> GetDeltaIndex(const SIndex_<DIM>& c, const SIndex_<DIM>& g) {
    SIndex_<DIM> res(c);
    for (St d = 0; d < DIM; ++d) {
        res[d] = g[d] - c[d];
    }
    return res;
}
template<St DIM>
bool OnSameAxe(const SIndex_<DIM>& c, const SIndex_<DIM>& g) {
    auto d = GetDeltaIndex(c, g);
    short count = 0;
    for(int i = 0; i < DIM; ++i){
        if(d[i] != 0){
            count++;
        }
    }
    return count <= 1;
} 
template<St DIM>
bool OnSameAxe(const SIndex_<DIM>& delta_idx) {
    short count = 0;
    for(short i = 0; i < DIM; ++i){
        if(delta_idx[i] != 0){
            count++;
        }
    }
    return count <= 1;
} 
template<St DIM>
Axes GetDeltaAxe(const SIndex_<DIM>& c, const SIndex_<DIM>& g) {
    auto d = GetDeltaIndex(c, g);
    for(int i = 0; i < DIM; ++i){
        if(d[i] != 0){
            return ToAxes(i);
        }
    }
    SHOULD_NOT_REACH;
    return _X_;
}
template<St DIM>
Axes GetDeltaAxe(const SIndex_<DIM>& didx) {
    for(int i = 0; i < DIM; ++i){
        if(didx[i] != 0){
            return ToAxes(i);
        }
    }
    SHOULD_NOT_REACH;
    return _X_;
}
template<St DIM>
Orientation GetDeltaOrient(const SIndex_<DIM>& c, const SIndex_<DIM>& g) {
    auto d = GetDeltaIndex(c, g);
    for(int i = 0; i < DIM; ++i){
        if(d[i] > 0)
            return _P_;
        else if(d[i] < 0)
            return _M_;
    }
    return _C_;
}
template<St DIM>
Orientation GetDeltaOrientOnSameAxe(const SIndex_<DIM>& delta_idx) {
    for(int i = 0; i < DIM; ++i){
        if(delta_idx[i] > 0)
            return _P_;
        else if(delta_idx[i] < 0)
            return _M_;
    }
    return _C_;
}
template<St DIM>
Orientation GetDeltaOrientOnAxe(
    const SIndex_<DIM>& c, const SIndex_<DIM>& g, const Axes& a) {
    if(g[a] > c[a])
        return _P_;
    else if(g[a] < c[a])
        return _M_;
    return _C_;
}

template<St DIM>
auto CrossListIndex(const SIndex_<DIM>& idx){
    typedef const SIndex_<DIM> Index;
    std::list<Index> res;
    for(auto& d : ArrAxes<DIM>()){
        res.push_back(idx.m(d));
        res.push_back(idx.p(d));
    }
    return res;
}


}

#endif