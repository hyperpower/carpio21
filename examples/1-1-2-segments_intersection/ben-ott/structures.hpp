#ifndef H_BST
#define H_BST

#include <set>
#include <vector>
#include <map>
namespace ref{
template <class T, class Comp> 
struct compare {
  Comp* comp; 
  compare(Comp* c) : comp(c) { }; 

  bool operator() (const T* a, const T* b) const {
    return a->less(*b,*comp); 
  }
};

template <class T, class Comp>
struct BST {
  typedef std::set <T*, compare<T,Comp> > Type;
};

// BST<Segment,Point>::Type btree(comp);

template <class T, class A> 
struct PriorityQueue_t {
  typedef std::map<T, std::vector<A> > Type;
};


template <class T, class A> 
class PriorityQueue : public PriorityQueue_t<T,A>::Type {
public:
  void push(const T& value) {
    std::vector<A> v; //empty vector
    this->insert(std::pair<T, std::vector<A> > (value, v));
  }

  void push(const T& value, const A& assoc) {
    std::vector<A> v; v.push_back(assoc);
    std::pair<typename PriorityQueue_t<T,A>::Type::iterator, bool> ret =
      this->insert(std::pair<T, std::vector<A> > (value, v));
    if(false == ret.second)
      ret.first->second.push_back(assoc);
    return;
  }

  T top() {
    return this->begin()->first;
  }

  void pop() {
    this->erase(this->begin());
  }

  bool mem(const T& value) {
    return (this->find(value) != this->end());
  }
};
}
#endif //H_BST