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
#include <tuple>

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
protected:
    double _t_dt; // total time
public:
    ProbeInfo(): _t_dt(0){
    }

    double sum_total_time() const{
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

    double total_time(){
       return sum_total_time();
    }

    double total_call() const{
        return lstart.size();
    }

    double avg_time(){
        return total_time() / total_call();
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
    std::string to_string_indent(const std::string& sep = " ") const{
        std::string res;
        auto iter = this->begin();
        auto n    = std::next(iter);
        for(;iter!= this->end(); iter++, n++){
            if(n != this->end()){
                res += sep;
            }else{
                res += *iter;
            }
        }
        return res;
    }
};
class ProfileTable: public std::list<std::tuple<ProbeName, long long, double> >{
public:
    typedef std::tuple<ProbeName, long long, double> Row;

    ProfileTable(){};

    auto find(const ProbeName& name){
        auto it = this->begin();
        
        for(; it!= this->end();it++){
            if(std::get<0>(*it) == name){
                return it;
            }
        }
        return this->end();
    }

    void push(const ProbeName& pn, double dt){
        auto it = this->find(pn);
        if(it != this->end()){
            Row& r = (*it);
            auto& c = std::get<1>(r);
            c+=1;
            auto& tt = std::get<2>(r);
            tt+=dt;
        }else{
            this->emplace_back(Row(pn, 1, dt));
        }
    }

    void add_dt(const ProbeName& pn, double dt){
        auto it = this->find(pn);
        if(it != this->end()){
            Row& r = (*it);
            auto& tt = std::get<2>(r);
            tt+=dt;
        }else{
            throw std::invalid_argument("Not find :" + pn.to_string());
        }
    }

    void show(){
        tfm::format(std::cout, "%-20s |%11s |%11s |%11s\n", 
                    "name", "total num", "total time", "avg time");
        for(auto& row : *this){
            tfm::format(std::cout, "%-20s %12d %12.2f %12.4f\n", 
                         std::get<0>(row).to_string_indent(), 
                         std::get<1>(row),
                         std::get<2>(row) / 1000.0,
                         std::get<2>(row) / std::get<1>(row) / 1000.0);
        }
    }

};


class Profile{
private:
    static Profile * pinstance_;
    static std::mutex mutex_;

    // int level;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

    std::list<TimePoint> _cur_tick;
    ProbeName _cur_name;

    ProfileTable _table;
protected:
    Profile(){
        // level = 0;
    }
    ~Profile() {
    }
    
public:
    Profile(Profile &other) = delete;
    void operator=(const Profile &) = delete;

    void probe_start(const std::string& name){
        // level++;
        _cur_name.push_back(name);
        _cur_tick.push_back(std::chrono::system_clock::now());
        _table.push(_cur_name, 0);
    }

    void probe_end(){
        auto tn = std::chrono::system_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::microseconds>(tn - _cur_tick.back()).count();
        _table.add_dt(_cur_name, dt);
        _cur_name.pop_back();
        _cur_tick.pop_back();
    }

    void show_list() {
        _table.show();
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