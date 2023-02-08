#include "structures.hpp"
#include "geometry.hpp"
#include "bentley_ottmann.hpp"
#include <iostream>
#include <utility>
#include <iterator>
#include <algorithm>
namespace ref{
map_event bentley_ottmann(std::vector<Segment>& set) {
  map_event inter;
  //std::cout << "Map : declared\n";
  // priority queue : insert all endpoints
  PriorityQueue<Event, Segment*> queue;

  std::vector<Segment>::iterator it;
  for(it = set.begin(); it != set.end(); it++) {
    Event e_left(it->get_left()), e_right(it->get_right());
    queue.push(e_left, &(*it)), queue.push(e_right);
  }
  
  // binary search tree : empty
  Point p_sweep(rat(I(0)),rat(I(0)));
  compare<Segment,Point> comp(&p_sweep);
  BST<Segment,Point>::Type btree(comp);

  // treatment of events
  while(queue.size()) {
    // current event : p event on top of set
    Event p = queue.top();
    Point point = p.get_point();
    vector_seg l_set = queue.begin()->second;    
    queue.pop();
    
    p_sweep.assign(point.get_abscissa(), point.get_ordinate());

    //checking for intersection
    std::pair<vector_seg, vector_seg> ir_set = get_sets(point, btree);
    
    if(l_set.size() + ir_set.first.size() + ir_set.second.size() > 1) {
      map_event::iterator it_ev = (inter.insert(pair_event(p, vector_seg()))).first;
      
      //it_ev->second : union of l_set, ir_set.first and ir_set.second
      vector_seg::iterator it_seg = l_set.begin();
      for(; it_seg != l_set.end(); 
            it_ev->second.push_back(*it_seg++));
      for(  it_seg = ir_set.first.begin(); 
            it_seg != ir_set.first.end();
            it_ev->second.push_back(*it_seg++));
      for(  it_seg = ir_set.second.begin();
            it_seg != ir_set.second.end();
            it_ev->second.push_back(*it_seg++));
      //std::cout << it_ev->second << "\n";
    }
    

    //delete ir_set.first and ir_set.second from btree
    vector_seg::iterator it_seg = ir_set.first.begin();
    for(; it_seg != ir_set.first.end(); 
          btree.erase(*it_seg++));
    for(it_seg = ir_set.second.begin(); 
        it_seg != ir_set.second.end();
        btree.erase(*it_seg++));
        
    //update sl
    p_sweep.assign(point.get_abscissa(), point.get_ordinate());
    
    //insert l_set and ir_set.first in btree
    for(it_seg = l_set.begin(); it_seg != l_set.end();
        btree.insert(*it_seg++));
    for(it_seg = ir_set.first.begin(); it_seg != ir_set.first.end();
        btree.insert(*it_seg++));
    
    //treat new events
    if(l_set.size() + ir_set.first.size() == 0) {
      //p is only the rightpoint of several segments
      Segment* s_a, * s_b;
      find_neighboors(point, btree, s_a, s_b); 
      
      compute_new_events(s_a, s_b, p, queue);
    }
    
    else {
      vector_seg v(l_set.size() + ir_set.first.size());
      set_union(l_set.begin(), l_set.end(),
                ir_set.first.begin(), ir_set.first.end(), v.begin());
      Segment* sl = find_leftmost(v, point), * sr = find_rightmost(v, point);
      Segment* s_b = find_left_neighboor(sl, btree), * s_a = find_right_neighboor(sr, btree);
      
      compute_new_events(sl, s_b, p, queue); compute_new_events(sr, s_a, p, queue);
    }
  }
  return inter;
}

void find_neighboors(const Point& p, BST<Segment,Point>::Type& btree,
                     Segment*& above, Segment*& below) {
  //create a segment of length 0 representing p :
  rat x = p.get_abscissa(), y = p.get_ordinate();
  Segment s(x, y, x, y);
  
  //search for upper neighboor
  BST<Segment,Point>::Type::iterator it = btree.upper_bound(&s);
  
  if(it == btree.end())
    above = NULL;
  else
    above = *it;
  if(it == btree.begin())
    below = NULL;
  else 
    below = *--it;
}

Segment* find_leftmost(vector_seg& v, const Point& p) {
  //asuming v isn't empty
  vector_seg::iterator it = v.begin();
  Segment* min = *v.begin();

  while(++it != v.end()) {
    if((*it)->less(*min,p))
      min = *it;
  }
  return min;
}

Segment* find_rightmost(vector_seg& v, const Point& p) {
  //assuming v isn't empty
  vector_seg::iterator it = v.begin();
  Segment* max = *v.begin();
  
  while(++it != v.end()) {
    if(max->less(**it,p))
      max = *it;
  }
  return max;
}

Segment* find_left_neighboor(Segment* s, BST<Segment,Point>::Type& btree) {
  BST<Segment,Point>::Type::iterator it = btree.find(s);
  if(it == btree.begin())
    return NULL;
  else
    return *--it;
}

Segment* find_right_neighboor(Segment* s, BST<Segment,Point>::Type& btree) {
  BST<Segment,Point>::Type::iterator it = btree.find(s);
  if(++it == btree.end()) 
    return NULL;
  else
    return *it;
}

std::pair<vector_seg, vector_seg> get_sets(const Point& p, BST<Segment,Point>::Type& btree) {
  vector_seg i,r;
  if(btree.empty())
    return std::pair<vector_seg, vector_seg> (i,r);
  //create a segment of lentgh zero representing p :
  rat x = p.get_abscissa(); 
  rat y = p.get_ordinate();
  Segment* s = new Segment(x, y, x, y);
  
  
  BST<Segment,Point>::Type::iterator it = btree.upper_bound(s);
  std::reverse_iterator<BST<Segment,Point>::Type::iterator> rit(it);

  Point q;
  while(rit != btree.rend() 
    && (*rit)->high(p) == y) { // p is on segment
    if((*rit)->is_in(p))
      i.push_back(*rit);
    else if((*rit)->is_rend(p))
      r.push_back(*rit);
    rit++;
  }

  return std::pair<vector_seg, vector_seg> (i,r);
}

void compute_new_events(Segment* s0, Segment* s1, const Event& p,
                        PriorityQueue<Event,Segment*>& q) {
  Point i;

  if(s0 && s1 && s0->intersect(*s1, i)) {
    Event ev_i(i);
    if(p < ev_i && ! q.mem(ev_i))
      q.push(ev_i);
  }
}
}