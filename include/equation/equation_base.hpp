#ifndef _EQUATION_BASE_HPP_
#define _EQUATION_BASE_HPP_

#include "equation_define.hpp"
#include "utility/any.hpp"
#include "time_control.hpp"
#include "stop_control.hpp"
// #include "equation/event/event.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "domain/domain.hpp"
#include <memory>
#include <map>

namespace carpio {

template<class D> class Event_;
template<class D> class EventCondition_;

template<class D>
class EquationBase_{
public:
    
    typedef D Domain;
    typedef typename Domain::ValueType   Vt;
    typedef typename Domain::Index       Index;
    typedef typename Domain::Grid          Grid;
    typedef typename Domain::spGrid      spGrid;
    typedef typename Domain::Ghost         Ghost;
    typedef typename Domain::spGhost     spGhost;
    typedef typename Domain::Order         Order;
    typedef typename Domain::spOrder     spOrder;
    typedef typename Domain::FieldCenter FieldCenter;
    typedef typename Domain::FieldFace   FieldFace;
    typedef typename Domain::FieldVertex FieldVertex;
    typedef typename Domain::FieldCenterExp FieldCenterExp;
    typedef typename Domain::FieldVertexExp FieldVertexExp;


    typedef EquationBase_<D> Self;

    typedef Event_<D>                      Event;
    typedef std::shared_ptr<Event>       spEvent;

    typedef Condition_<D>                    Condition;
    typedef std::shared_ptr<Condition> spCondition;

    typedef typename std::conditional<std::is_same_v<
                typename Domain::LocationTag, CenterTag>, 
                FieldCenter, FieldVertex>::type FieldMain;
    typedef std::shared_ptr<FieldMain> spFieldMain; 
    typedef typename std::conditional<std::is_same_v<
                typename Domain::LocationTag, CenterTag>, 
                FieldCenterExp, FieldVertexExp>::type FieldMainExp;
    typedef std::shared_ptr<FieldMainExp> spFieldMainExp; 

    typedef std::shared_ptr<FieldCenter>    spFieldCenter;
    typedef std::shared_ptr<FieldFace>      spFieldFace;
    typedef std::shared_ptr<FieldCenterExp> spFieldCenterExp;
    typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

    typedef std::map<std::string, spFieldMain> Fields;
    typedef std::map<std::string, Any>             Configures;
    typedef std::map<std::string, spFieldFace>     FieldFaces;
    typedef std::map<std::string, spBoundaryIndex> BIs;
    typedef std::map<std::string, spEvent>         Events;

    typedef TimeControl_<D> TimeControl;
    typedef std::shared_ptr<TimeControl> spTimeControl;

    typedef StopControl_<D> StopControl;
    typedef std::shared_ptr<StopControl> spStopControl;

    typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;

    typedef Solver_<Vt>             Solver;
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt> Solver_Jacobi;
    typedef SOR_<Vt>    Solver_SOR;
    typedef CG_<Vt>     Solver_CG;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;
    typedef std::function<Vt(Vt, Vt, Vt)> FunXYZ_Value;

protected:
    spGrid  _spgrid; 
    spGhost _spghost; 
    spOrder _sporder;

    Configures _configs;
    Fields     _fields;
    FieldFaces _ffaces;
    BIs        _bis;
    Events     _events; 

    spTimeControl _time;
    spStopControl _stop;
public:
    EquationBase_(){}; 

    EquationBase_(spGrid  spgrid, 
                  spGhost spghost,
                  spOrder sporder):
        _spgrid(spgrid), 
        _spghost(spghost),
        _sporder(sporder), 
        _time(nullptr){
    }

    

    virtual ~EquationBase_(){};

    virtual int run_one_step(St step, Vt time) = 0;

    virtual int initialize() = 0; 

    virtual int finalize() =0; 

    virtual int solve() = 0;

    virtual std::string name() const{
        return "EquationBase";
    };
    
    void run() {
        // the equation don't have time
        if (this->_time == nullptr) {
            initialize();
            // start events
            run_events(0, 0.0, Event::START);

            solve();

            run_events(1, 0.0, Event::END);
            finalize();
        } else {
            // events before calculation
            initialize();
            run_events(this->_time->current_step(), //
                       this->_time->current_time(), //
                       Event::START);
            // loop
            while (!this->_time->is_end() && (!_stop->is_stop())) {
                //
                // events before each step
                run_events(this->_time->current_step(),  //
                           this->_time->current_time(),     //
                           Event::BEFORE);

                // run one step =================
                run_one_step(this->_time->current_step(),
                             this->_time->current_time());
                // ==============================

                // events after each step
                run_events(this->_time->current_step(),  //
                           this->_time->current_time(),     //
                           Event::AFTER);
                //
                this->_time->advance();
            }
            // if(this->has_event("_STOP_")){
            //     this->_events["_STOP_"]->show();
            // }
            // events after calculation
            run_events(this->_time->current_step(),    //
                       this->_time->current_time(),       //
                     Event::END);
            finalize();
        }
    }

    void run_events(St step, Vt t, int fob) {
        for (auto& event : this->_events) {
            if (event.second->do_execute(step, t, fob)) {
                event.second->execute(step, t, fob, (*this));
            }
        }
    }

    virtual Ghost& ghost() {
        return *(this->_spghost);
    }

    virtual Grid& grid() {
        return *(this->_spgrid);
    }

    virtual const Ghost& ghost() const{
        return *(this->_spghost);
    }

    virtual const Grid& grid() const{
        return *(this->_spgrid);
    }

    virtual const TimeControl& time_control() const{
        return *(this->_time);
    }

    virtual void set_time_term(St n, Vt dt, Vt tau = 1){
        this->_time = spTimeControl(
                           new TimeControl( n, dt, tau));
        this->_stop = spStopControl(
                           new StopControl());
    }

    virtual void set_time_scheme(
            const std::string& name,
            const Vt&          v = 0.5){
        ASSERT(name == "explicit"
            || name == "implicit"
            || name == "CN"
            || name == "CNgeneral");
        this->_configs["set_time_scheme"] = name;
        if(name == "CNgeneral"){
            this->_configs["cn_omega"] = v;
        }
    }
    
    virtual void set_space_scheme(
            const std::string& name){
        this->_configs["space_scheme"] = name;
    } 

    bool has_field(const std::string& key) const {
        auto it = this->_fields.find(key);
        if (it != this->_fields.end()) {
            return true;
        }
        return false;
    }
    const FieldMain& field(const std::string& key) const{
        if(this->has_field(key)){
            return *(this->_fields.at(key));
        }else{
            throw std::invalid_argument( key + "is not fields" );
        }
    }
    FieldMain& field(const std::string& key){
        if(this->has_field(key)){
            return *(this->_fields.at(key));
        }else{
            throw std::invalid_argument( key + "is not fields" );
        }
    }
    bool has_field_face(const std::string& key) const {
        auto it = this->_ffaces.find(key);
        if (it != this->_ffaces.end()) {
            return true;
        }
        return false;
    }
    const FieldFace& field_face(const std::string& key) const{
        if(this->has_field(key)){
            return *(this->_ffaces.at(key));
        }else{
            throw std::invalid_argument( key + "is not fields" );
        }
    }
    bool has_config(const std::string& key) const {
        auto it = this->_configs.find(key);
        if (it != this->_configs.end()) {
            return true;
        }
        return false;
    }

    void set_solver(const std::string&    name,
                    const int& max_iter = 1000,
                    const Vt&  tol      = 1e-4,
                    const Any& any      = 1.0
                    ) {
        // name should be
        ASSERT(name == "Jacobi" || name == "CG" || name == "SOR");
        this->_configs["set_solver"]           = name;
        this->_configs["set_solver_max_iter"]  = max_iter;
        this->_configs["set_solver_tolerence"] = tol;
        if (name == "SOR") {
            this->_configs["SOR_omega"] = any;
        }
    }

    spSolver get_solver(){
        auto spsolver = any_cast<spSolver>(this->_configs["solver"]);
        return spsolver;
    }
    
    bool has_boundary_index(const std::string& key) const {
        auto it = this->_bis.find(key);
        if (it != this->_bis.end()) {
            return true;
        }
        return false;
    }

    void set_boundary_index(
            const std::string& key,
            spBoundaryIndex spbi){
        this->_bis[key] = spbi;
    }

    spBoundaryIndex get_boundary_index(const std::string& key){
        if (this->has_boundary_index(key)){
            return this->_bis[key];
        }else{
            return std::make_shared<BoundaryIndex>(BoundaryIndex());
        }
    }

    /**
     * @brief this function check the events
     *        if flags contain key and value == val return true
     *        the default val == 1
     */
    bool has_event(const std::string& key) const {
        auto it = this->_events.find(key);
        if (it != this->_events.end()) {
            return true;
        }
        return false;
    }

    void add_event(const std::string& key, spEvent spe){
        ASSERT(spe != nullptr);
        auto result = this->_events.insert({key, spe}); 

        if (!result.second) {
            std::cerr << "Key already exists. Value: " << result.first->second << std::endl;
        }
    }
    template<class SPE>
    void add_stop_condition(const std::string& key, SPE spe){
        this->add_event(key, spe);
        _stop->add_condition(spe);
    }

    
    void set_field_center(const std::string& name, spFieldCenter spsource){
        if(this->has_field(name)){
            this->_fields[name] = spsource;
        }else{
            ASSERT_MSG(false, "Field Center : \t" + name + "Not Found"); 
        }
    }

    void set_field_center(const std::string& name, FunXYZ_Value fun){
        if(this->has_field(name)){
            auto& fc = *(this->_fields[name]);
            fc.assign(fun);
        }else{
            ASSERT_MSG(false, "Field Center : \t" + name + "Not Found"); 
        }
    }

    void set_field_face(const std::string& name, FunXYZ_Value fun){
        if(this->has_field_face(name)){
            auto& fc = *(this->_ffaces[name]);
            fc.assign(fun);
        }else{
            ASSERT_MSG(false, "Field Face : \t" + name + "Not Found"); 
        }
    }
    void new_field(const std::string& name){
        if(!(this->has_field(name))){
            this->_fields[name] = spFieldMain(new FieldMain(
                    this->_spgrid,
                    this->_spghost,
                    this->_sporder));
        }
    }
    void new_field_face(const std::string& name, Axes a){
        if(!(this->has_field_face(name))){
            this->_ffaces[name] = spFieldFace(new FieldFace(
                    this->_spgrid,
                    this->_spghost,
                    a));
        }
    }
protected:
    spFieldCenterExp new_field_exp_zero() const{
        return Domain::NewspFieldCenterExpZero(
            this->_spgrid,
            this->_spghost,
            this->_sporder 
        );
    }
    // spFieldCenterExp new_field_exp_coe_one() const{
    //     return Domain::NewspFieldCenterExpCoeOne(
    //                 this->_spgrid, this->_spghost, this->_sporder);
    // }
    spFieldMainExp new_field_exp_coe_one() const{
        return Domain::NewspFieldExpCoeOne(
                    this->_spgrid, this->_spghost, this->_sporder, typename Domain::LocationTag());
    }
    template<class Container>
    void new_fields(const Container& list){
        auto isstring = std::is_same<typename Container::value_type,
                                     std::string>::value;
        ASSERT(isstring == true);
        for(auto& str : list){
            this->new_field(str);
        }
    }

    virtual spSolver _init_solver() {
        // initial solver
        spSolver spsolver;
        if (this->has_config("set_solver")) {
            std::string sn = any_cast<std::string>(
                    this->_configs["set_solver"]);
            Vt  tol      = any_cast<Vt>(this->_configs["set_solver_tolerence"]);
            int max_iter = any_cast<int>(this->_configs["set_solver_max_iter"]);
            if (sn == "Jacobi") {
                spsolver = spSolver(new Solver_Jacobi(max_iter, tol));
            } else if (sn == "CG") {
                spsolver = spSolver(new Solver_CG(max_iter, tol));
            } else if (sn == "SOR") {
                ASSERT(this->has_config("SOR_omega"));
                Vt omega = any_cast<Vt>(this->_configs["SOR_omega"]);
                spsolver = spSolver(new Solver_SOR(max_iter, tol, omega));
            }
        } else {
            // default solver
            spsolver = spSolver(new Solver_Jacobi(500, 1e-7));
        }
        return spsolver;
    }

    virtual int _build_mat_and_solve(const FieldMainExp& fexp, FieldMain& fres){
        auto& solver = *(any_cast<spSolver>(this->_configs["solver"]));
        Mat a;  
        Arr b;
        BuildMatrix(fexp, a, b);
        Arr x = fres.to_array();
        this->_configs["solver_return_code"] = solver.solve(a, x, b);
        fres.assign(x);
        return any_cast<int>(this->_configs["solver_return_code"]);
    }
};



}

#endif