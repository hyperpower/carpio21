#ifndef _S_ORDER_XYZ_HPP_
#define _S_ORDER_XYZ_HPP_


#include "domain/structure/structure_define.hpp"
#include "domain/structure/sindex.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/sghost.hpp"
#include "sorder.hpp"
#include "algebra/array/multi_array.hpp"

namespace carpio{

struct SOrderXYZTag:  public SOrderTag{};

template<St DIM, class GRID, class GHOST, class LOCTAG>
class SOrderXYZ_ : public SOrder_<DIM, GRID, GHOST, LOCTAG>{
};


template<St DIM, class GRID, class GHOST>
class SOrderXYZ_ <DIM, GRID, GHOST, CenterTag>: public SOrder_<DIM, GRID, GHOST, CenterTag>{
public:
    static const St Dim = DIM;
    typedef SOrder_<DIM, GRID, GHOST, CenterTag> Base; 
    typedef SIndex_<DIM> Index;
    typedef CenterTag LocationTag;
    typedef GRID  Grid;
    typedef SOrderXYZTag Tag;
    typedef typename std::shared_ptr<Grid> spGrid;
    typedef GHOST Ghost;
    typedef typename std::shared_ptr<Ghost> spGhost;

    typedef std::vector<Index> OrderVector;
    typedef typename OrderVector::iterator iterator;
    typedef typename OrderVector::const_iterator const_iterator;

    typedef MultiArray_<St, DIM> OrderMatrix;
protected:
    OrderVector _ov;  // order vector
    OrderMatrix _om;  // order matrix

    Axes _face_axes;
#ifdef OPENMP
    St _nt;
    std::vector<iterator> _ov_iter; // store the separate index of _ov
    std::vector<const_iterator> _ov_citer; // store the separate index of _ov
#endif
public:
    SOrderXYZ_(spGrid spgrid, 
               spGhost spghost,
               St num_threads = 1
               ):
        Base(spgrid, spghost),
        _ov(this->_ghost->size_cell_normal()),
        _om(this->_grid->n(_X_), this->_grid->n(_Y_), this->_grid->n(_Z_))
#ifdef OPENMP
       ,_nt(num_threads)
       ,_ov_iter(num_threads + 1)
       ,_ov_citer(num_threads + 1)
#endif
    {
        _build_ov();
        _build_om();
#ifdef OPENMP
        _build_ovt();
#endif
    };

    SOrderXYZ_(spGrid spgrid, 
               spGhost spghost,
               Axes    a):
       Base(spgrid, spghost), _face_axes(a),
        _ov(this->_ghost->size_face_normal(a)),
        _om(this->_grid->n(_X_), this->_grid->n(_Y_), this->_grid->n(_Z_)){
        _build_ov_for_face(a);
        _build_om_for_face(a);
    }

    St size() const{
        return _ov.size();
    }

    iterator begin(){
        return _ov.begin();
    }

    iterator end(){
        return _ov.end();
    }

    const_iterator begin() const{
        return _ov.begin();
    }

    const_iterator end() const{
        return _ov.end();
    }
#ifdef OPENMP
    St num_threads() const{
        return _nt;
    }
    iterator begin(St tn) { // input thread number
        ASSERT(tn < _nt);
        return _ov_iter[tn];
    }
    const_iterator begin(St tn) const {
        ASSERT(tn < _nt);
        return _ov_citer[tn];
    }
    iterator end(St tn) {
        ASSERT(tn < _nt);
        return _ov_iter[tn + 1];
    }
    const_iterator end(St tn) const {
        ASSERT(tn < _nt);
        return _ov_citer[tn + 1];
    }
#endif
    St get_order(const Index& cell_idx) const{
        if(!(this->_ghost->is_ghost(cell_idx))){
            return _om(cell_idx.i(), cell_idx.j(), cell_idx.k());
        }
        std::cerr<< ">! Error Index = " << cell_idx << std::endl;
        SHOULD_NOT_REACH;
        return 0;
    }
    St get_order(int i, int j = 0, int k = 0) const{
        Index idx(i,j,k);
        return get_order(idx);
    }
    St get_order_face_index(const Index& face_idx,const Axes& a) const{
        auto cidx = this->_grid->face_index_to_cell_index(face_idx, a);
        if(!(this->_ghost->is_ghost(cidx))){
            return _om(face_idx.i(), face_idx.j(), face_idx.k());
        }
        std::cerr<< ">! Error Index = " << face_idx << std::endl;
        SHOULD_NOT_REACH;
        return 0;
    }
protected:
    void _build_ov() {
        St count = 0;
        for (auto k = 0; k < ((DIM == 3) ? this->_grid->n(_Z_) : 1); ++k) {
            for (auto j = 0; j < ((DIM >= 2) ? this->_grid->n(_Y_) : 1); ++j) {
                for (auto i = 0; i < this->_grid->n(_X_); ++i) {
                    Index index(i,j,k);
                    if(!(this->_ghost->is_ghost(index))){
                        _ov[count] = index;
                        count++;
                    }
                }
            }
        }
    }

    void _build_om() {
        // call after _build_ov
        for(typename OrderVector::size_type ii = 0; ii < _ov.size(); ++ii ){
            _om(_ov[ii].i(), _ov[ii].j(), _ov[ii].k()) = ii;
        }
    }
    void _build_ov_for_face(const Axes& a) {
        St count = 0;
        for (auto k = 0; k < ((DIM == 3) ? this->_grid->n(_Z_) : 1); ++k) {
            for (auto j = 0; j < ((DIM >= 2) ? this->_grid->n(_Y_) : 1); ++j) {
                for (auto i = 0; i < this->_grid->n(_X_); ++i) {
                    Index index(i,j,k);
                    if(!(this->_ghost->is_ghost(index))){
                        _ov[count] = index;
                        count++;
                        if(index[a] == this->_grid->n(a) - 1){
                            _ov[count] = this->_grid->cell_index_to_face_index(index, _P_, a);
                            count++;
                        }
                    }
                }
            }
        }
    }
    void _build_om_for_face(const Axes& a) {
        // call after _build_ov_for_face
        St in = this->_grid->n(_X_) + (a == _X_ ? 1 : 0);
        St jn = (DIM >= 2) ? this->_grid->n(_Y_) + (a == _Y_ ? 1 : 0) : 1;
        St kn = (DIM >= 3) ? this->_grid->n(_Z_) + (a == _Z_ ? 1 : 0) : 1;
        _om.reconstruct(in, jn, kn);
        for(typename OrderVector::size_type ii = 0; ii < _ov.size(); ++ii ){
            _om(_ov[ii].i(), _ov[ii].j(), _ov[ii].k()) = ii;
        }
    }
#ifdef OPENMP
    void _build_ovt(){
        St d = _ov.size() / _nt;
        for(St i = 0; i < _nt; i++ ){
            St step = d * i;
            auto it = this->begin();
            std::advance(it, step);
            _ov_iter[i]  = it;
            auto cit = this->begin();
            _ov_citer[i]  = std::next(cit, step);;
        }
        _ov_iter[_nt] = this->end();
        _ov_citer[_nt] = this->end();
    }
#endif
};

template<St DIM, class GRID, class GHOST>
class SOrderXYZ_ <DIM, GRID, GHOST, VertexTag>: public SOrder_<DIM, GRID, GHOST, VertexTag>{
public:
    static const St Dim = DIM;
    typedef SOrder_<DIM, GRID, GHOST, VertexTag> Base;
    typedef SIndex_<DIM> Index;
    typedef VertexTag LocationTag;
    typedef GRID  Grid;
    typedef SOrderXYZTag Tag;
    typedef typename std::shared_ptr<Grid> spGrid;
    typedef GHOST Ghost;
    typedef typename std::shared_ptr<Ghost> spGhost;

    typedef std::vector<Index> OrderVector;
    typedef typename OrderVector::iterator iterator;
    typedef typename OrderVector::const_iterator const_iterator;

    typedef MultiArray_<St, DIM> OrderMatrix;
protected:
    

    OrderVector _ov;  // order vector
    OrderMatrix _om;  // order matrix

public:
    SOrderXYZ_(spGrid spgrid, 
               spGhost spghost,
               St num_threads = 1
               ):Base(spgrid, spghost),
        _ov(this->_ghost->size_vertex_normal()),
        _om(this->_grid->n(_X_) - 1 , this->_grid->n(_Y_) - 1, this->_grid->n(_Z_) - 1)
    {
        _build_ov();
        _build_om();
    };

    St size() const{
        return _ov.size();
    }

    iterator begin(){
        return _ov.begin();
    }

    iterator end(){
        return _ov.end();
    }

    const_iterator begin() const{
        return _ov.begin();
    }

    const_iterator end() const{
        return _ov.end();
    }

    St get_order(const Index& vidx) const{
        if(this->_ghost->is_normal_vertex(vidx)){
            return _om(vidx.i(), vidx.j(), vidx.k());
        }
        std::cerr<< ">! Error Index = " << vidx << std::endl;
        SHOULD_NOT_REACH;
        return 0;
    }
    St get_order(int i, int j = 0, int k = 0) const{
        Index idx(i,j,k);
        return get_order(idx);
    }
     
    
protected:
    void _build_ov() {
        St count = 0;
        for (auto k = 0; k < ((DIM == 3) ? this->_grid->n(_Z_) : 1); ++k) {
            for (auto j = 0; j < ((DIM >= 2) ? this->_grid->n(_Y_) : 1); ++j) {
                for (auto i = 0; i < this->_grid->n(_X_); ++i) {
                    Index index(i,j,k);
                    if(!(this->_ghost->is_ghost(index))
                        && !(this->_ghost->is_boundary(index, _P_))){
                        _ov[count] = index;
                        count++;
                    }
                }
            }
        }
    }

    void _build_om() {
        // call after _build_ov
        for(typename OrderVector::size_type ii = 0; ii < _ov.size(); ++ii ){
            _om(_ov[ii].i(), _ov[ii].j(), _ov[ii].k()) = ii;
        }
    }
};


}



#endif