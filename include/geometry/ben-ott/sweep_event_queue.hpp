#ifndef _BEN_OTT_SWEEP_EVENT_QUEUE_HPP_
#define _BEN_OTT_SWEEP_EVENT_QUEUE_HPP_



namespace carpio {
    
template <class EVENT, class VALUE> 
class SweepEventQueue_ : public std::map<EVENT, 
            std::array<std::set<const VALUE*>, 3> > {
public:
    typedef const VALUE* constp_Value;
    typedef std::map<EVENT, std::list<constp_Value> > Base;
    typedef std::set<constp_Value> Setcp;
    typedef std::array<Setcp, 3>   ArrSetcp;


    void add(const EVENT& event) {
        Setcp set_empty;  //empty vector
        this->insert(std::pair<EVENT, ArrSetcp> (event, {set_empty, set_empty, set_empty}));
    }
    void add_event(const EVENT& event, const int& idx, typename Setcp::value_type assoc) {
        auto iter = this->find(event);
        if(iter != this->end()){
            auto res = iter->second[idx].insert(assoc);
        }else{
            Setcp v, set_empty; 
            v.insert(assoc);
            switch (idx){
            case 0:
                this->insert(std::pair<EVENT, ArrSetcp> (event, {v, set_empty, set_empty}));
                break;
            case 1:
                this->insert(std::pair<EVENT, ArrSetcp> (event, {set_empty, v, set_empty}));
                break;
            case 2:
                this->insert(std::pair<EVENT, ArrSetcp> (event, {set_empty, set_empty, v}));
                break;
            }
        }
    }

    EVENT top() {
        return this->begin()->first;
    }

    void pop() {
        this->erase(this->begin());
    }

    bool mem(const EVENT& value) {
        return (this->find(value) != this->end());
    }

    #ifdef _DEBUG_MODE_
    void output(std::ofstream& ofs) const{

    }
    #endif
};

} //namespace


#endif