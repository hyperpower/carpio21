#pragma once
#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

#include <map>
#include <cmath>
#include <mutex>
#include <memory>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "type_define.hpp"
#include "tinyformat.hpp"
#include "clock.hpp"

namespace carpio {

struct FunctionInfo{
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
    std::string name;
    std::list<TimePoint> lstart;
    std::list<TimePoint> lend;

    double sum() const{
        auto istart = lstart.begin();
        auto iend   = lend.begin();
        double dt = 0;
        for(;(istart != lstart.end()) || (iend != lend.end());){
            dt += std::chrono::duration_cast<std::chrono::milliseconds>((*iend) - (*istart)).count();
            istart++;
            iend++;
        }
        return dt;
    }
    void show_sum() const{
        auto calls = lstart.size();
        double ms = this->sum();
        std::string date = tfm::format("%25s :  %10d   %.3f\n",
                                 name, calls, ms);
        std::cout << date;
    }
};
class ProbeInfo{
public:
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
    std::list<TimePoint> lstart;
    std::list<TimePoint> lend;

    ProbeInfo(){
    }

    auto size(){
        if(lstart.size() == lend.size()){
            return lstart.size();
        }else{
            // warning
            return lstart.size();
        }
    }

    double sum() const{
        auto istart = lstart.begin();
        auto iend   = lend.begin();
        double dt = 0;
        for(;(istart != lstart.end()) && (iend != lend.end());){
            dt += std::chrono::duration_cast<std::chrono::milliseconds>((*iend) - (*istart)).count();
            istart++;
            iend++;
        }
        return dt;
    }
    void show_sum() const{
        
    }
};

class ProbeName : public std::list<std::string>{
protected:
    typedef ProbeName Self;
public:
    bool operator==(const Self& rhs) const{
        if(this->size() == rhs.size()){
            auto iter = this->begin();
            auto riter = rhs.begin();
            for(; iter != this->end() || riter != rhs.end();){
                if(*iter != *riter){
                    return false;
                }
                iter++;
                riter++;
            }
            return true;
        }else{
            return false;
        }
    }
    std::string to_string(const std::string& sep = "_") const{
        std::string res;
        auto iter = this->begin();
        auto n    = std::next(iter);
        for(;iter!= this->end(); iter++, n++){
            res += *iter;
            if(n != this->end()){
                res += sep;
            }
        }
        return res;
    }
};
class ProfileTable{
public:
    std::list<ProbeName> lname;
    std::list<ProbeInfo> linfo;

    auto find(const ProbeName& name){
        auto itern = lname.begin();
        auto iteri = linfo.begin();
        
        for(; itern!= lname.end() && iteri != linfo.end();){
            if(*itern == name){
                return std::pair<std::list<ProbeName>::iterator,
                                 std::list<ProbeInfo>::iterator>(itern, iteri);
            }
            itern++;
            iteri++;
        }
        return std::pair<std::list<ProbeName>::iterator,
                         std::list<ProbeInfo>::iterator>(lname.end(), linfo.end());
    }

    void push(const ProbeName& pn, const typename ProbeInfo::TimePoint& t){
        lname.push_back(pn);
        linfo.emplace_back(ProbeInfo());
        linfo.back().lstart.push_back(t);

    }

    void clear(){
        lname.clear();
        linfo.clear();
    }



};
class Profile{
private:
    static Profile * pinstance_;
    static std::mutex mutex_;

    // int level;
    ProbeName _cur_name;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

    // std::list<TimePoint> _cur_tick;

    ProfileTable _table;
protected:
    Profile(){
        // level = 0;
    }
    ~Profile() {
    }

    // auto _find(const std::string& name){
    //     for(auto iter = _list.begin(); iter!= _list.end(); ++iter){
    //         if(iter->name == name){
    //             return iter;
    //         }
    //     }
    //     return _list.end();
    // }
    
public:
    Profile(Profile &other) = delete;
    void operator=(const Profile &) = delete;

    void probe_start(const std::string& name){
        // level++;
        auto tn = std::chrono::system_clock::now();
        std::string nn = name;
        _cur_name.push_back(nn);
        auto p = _table.find(_cur_name);
        if (p.first == _table.lname.end()){
            _table.push(_cur_name, tn);
        }else{
            p.second->lstart.push_back(tn);
        }
        
    }

    void probe_end(){
        auto tn = std::chrono::system_clock::now();
        auto p = _table.find(_cur_name);
        if (p.first != _table.lname.end()){
            p.second->lend.push_back(tn);
        }else{
            throw std::logic_error("no probe start");
        }
        _cur_name.pop_back();
    }

    void show_list() const{
        for(auto& name: _table.lname){
            std::cout << name.to_string() << std::endl;
        }
    }

    void clean(){
        _cur_name.clear();
        // _cur_tick.clear();
        _table.clear();
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

inline void ProfileClean(){
    auto p = Profile::Get();
    p->clean();
}

}


#endif