#ifndef _O_DATA_IDX_HPP_
#define _O_DATA_IDX_HPP_

#include "domain/domain_define.hpp"

namespace carpio {

class ODataIdx {
protected:
    St _idx;

public:
    ODataIdx() :
        _idx(0) {
    }

    explicit ODataIdx(St idx) :
        _idx(idx) {
    }

    St idx() const {
        return _idx;
    }

    void set_idx(St idx) {
        _idx = idx;
    }
};

}

#endif
