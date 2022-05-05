#pragma once
#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

#include <map>
#include <cmath>
#include <mutex>
#include <memory>
#include <iostream>
#include <algorithm>

#include "type_define.hpp"
#include "tinyformat.hpp"
#include "clock.hpp"

namespace carpio {

struct FunctionInfo{
    std::string name;
    std::list<tick_t> lstart;
    std::list<tick_t> lend;

    double sum(){
        auto istart = lstart.begin();
        auto iend   = lend.begin();
        double dt = 0;
        for(;(istart != lstart.end()) || (iend != lend.end());){
            dt += Clock::TicksToMillisecondsD((*iend) - (*istart));
            istart++;
            iend++;
        }
        return dt;
    }
    void show_sum(){
        auto calls = lstart.size();
        double ms = this->sum();
        std::string date = tfm::format("%20s :  %d   %.3d\n",
                                 name, calls, ms);
        std::cout << date;
    }
};

class Profile{
private:
    static Profile * pinstance_;
    static std::mutex mutex_;

    int level;
    std::list<std::string> _cur_name;
    std::list<tick_t> _cur_tick;

    std::list<FunctionInfo> _list;
protected:
    Profile(){
        level = 0;
    }
    ~Profile() {
    }

    auto _find(const std::string& name){
        for(auto iter = _list.begin(); iter!= _list.end(); ++iter){
            if(iter->name == name){
                return iter;
            }
        }
        return _list.end();
    }
    
public:
    Profile(Profile &other) = delete;
    void operator=(const Profile &) = delete;

    void probe_start(const std::string& name){
        level++;
	    _cur_tick.push_back(Clock::Tick());
        // auto nn = _cur_tick.size();
        std::string nn = ToString(level) + "_" + name;
        _cur_name.push_back(nn);
        auto pinfo = this->_find(nn);
        if (pinfo == this->_list.end()){
            FunctionInfo info;
            info.name = nn;
            _list.push_back(info);
        }
    }

    void probe_end(){
        level--;
        // _cur_end = Clock::Tick();
        auto cn = _cur_name.back();
        auto ct = _cur_tick.back();
        auto pinfo = this->_find(cn);
        if (pinfo != this->_list.end()){
            pinfo->lstart.push_back(ct);
            pinfo->lend.push_back(Clock::Tick());
        }
        _cur_name.pop_back();
        _cur_tick.pop_back();
    }

    void show_list(){
        for(auto& info: _list){
            info.show_sum();
        }
    }

    static Profile *Get();
};

Profile* Profile::pinstance_{nullptr};
std::mutex Profile::mutex_;

Profile *Profile::Get(){
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr){
        pinstance_ = new Profile();
    }
    return pinstance_;
}

inline void ProfileStart(const std::string& name){
    auto p = Profile::Get();
    p->probe_start(name);
}

inline void ProfileEnd(){
    auto p = Profile::Get();
    p->probe_end();
}

inline void ProfileListShow(){
    auto p = Profile::Get();
    p->show_list();
    

}

}


#endif