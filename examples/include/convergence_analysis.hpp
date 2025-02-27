#ifndef _CONVERGENCE_ANALYSIS_HPP_
#define _CONVERGENCE_ANALYSIS_HPP_

#include <string>
#include "io/gnuplot.hpp"

#include "example_define.hpp"


using namespace carpio;

template<class VN, class VVN>
void PlotResidual(const std::string& ffn, 
                  const VN&  ln,
                  const VVN& llr){
    Gnuplot gnu;
    gnu.set_ylogscale();
	// gnu.set_xrange(-0.1, 1.1);
	// gnu.set_yrange(-0.1, 1.1);
	gnu.set_ylabel("Residual");
	gnu.set_xlabel("Number of iteration");
    gnu.set_yformat("10^{%L}");
	// gnu.set_equal_aspect_ratio();
	// gnu.set_palette_blue_red();
    auto itern = ln.begin();
    auto iterr = llr.begin();
    for(;itern != ln.end();){
        auto a = ToGnuplotActor(*iterr);
        a.title("Mesh n = " + ToString(*itern));
        a.style("with lines lw 2");
	    gnu.add(a);
        itern++;
        iterr++;
    }
    gnu.set_terminal_png(ffn, fig_width, fig_height, "Fira Code", 16);
	gnu.plot();
}

std::list<double> ConvergenceOrderReference(int order,
               const std::vector<int>& ln,
               const std::list<double>& l1){
    std::list<double> res;
    res.push_back(l1.front());
    auto itern = ln.begin();
    itern++;
    for(;itern != ln.end();itern++){
        auto nm = *(std::prev(itern));
        auto n  = (*itern);
        auto v  = res.back() / std::pow(double(n) / double(nm), double(order));
        res.push_back(v);
    }
    return res;
}

void PlotError(const std::string& ffn,
               const int order,
               const std::vector<int>& ln,
               const std::list<double> & l1,
               const std::list<double> & l2,
               const std::list<double> & li
               ){
    Gnuplot gnu;
    gnu.set_xlogscale();
    gnu.set_ylogscale();
	gnu.set_ylabel("Norm");
	gnu.set_xlabel("1/n");
    gnu.set_yformat("10^{%L}");
	// gnu.set_equal_aspect_ratio();
    std::list<double> lh;
    for(auto& n:ln){
        lh.push_back(1.0/n);
    }
    auto a1 = ToGnuplotActor(lh, l1);
    a1.title("L1-Norm");
    a1.style("with linespoints lw 2 pt 7");
    auto a2 = ToGnuplotActor(lh, l2);
    a2.title("L2-Norm");
    a2.style("with linespoints lw 2 pt 7");
    auto ai = ToGnuplotActor(lh, li);
    ai.title("Linf-Norm");
    ai.style("with linespoints lw 2 pt 7");
    gnu.add(a1);
    gnu.add(a2);
    gnu.add(ai);

    auto l1ref = ConvergenceOrderReference(order, ln, l1);
    auto a1r = ToGnuplotActor(lh, l1ref);
    // a1r.title("2 Order");
    a1r.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

    auto l2ref = ConvergenceOrderReference(order, ln, l2);
    auto a2r = ToGnuplotActor(lh, l2ref);
    a2r.title(ToString(order) + "-Order Reference");
    a2r.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

    auto liref = ConvergenceOrderReference(order, ln, li);
    auto air = ToGnuplotActor(lh, liref);
    // a2r.title("2 Order");
    air.style("with lines lw 1 lc rgb \"#0C0D0E\" dt 2");

    gnu.add(a1r);
    gnu.add(a2r);
    gnu.add(air);

    gnu.set_terminal_png(ffn, fig_width, fig_height);
    gnu.set_key("top left");
	gnu.plot();
}

inline Vt cal_order(Vt e2n, Vt en){
    return std::log2(en / e2n);
}
void OutputError(
    const std::string& ffn,
    const std::vector<int>& ln,
    const std::list<double> & l1,
    const std::list<double> & l2,
    const std::list<double> & li){
        
    std::ofstream fout(ffn,std::ios::out);

    tfm::format(fout, "n,L1-Norm, O-L1, L2-Norm, O-L2, Linf-Norm, O-Linf\n");
    int count = 0;
    auto itervn = ln.begin();
    auto iterl1 = l1.begin();
    auto iterl2 = l2.begin();
    auto iterli = li.begin();
    for(;itervn != ln.end();){
        tfm::format(std::cout,
                    "n: %8d N1: %10.5e N2: %10.5e Ni: %10.5e\n",
                    *itervn, *iterl1, *iterl2, *iterli);
        if(count > 0){
            auto pl1 = std::prev(iterl1);
            auto pl2 = std::prev(iterl2);
            auto pli = std::prev(iterli);
            tfm::format(fout,
                    "%8d, %10.3e, %10.2f, %10.3e, %10.2f, %10.3e, %10.2f\n",
                    *itervn, *iterl1, cal_order(*iterl1, *pl1),
                             *iterl2, cal_order(*iterl2, *pl2), 
                             *iterli, cal_order(*iterli, *pli));
        }else{
            tfm::format(fout,
                    "%8d, %10.3e, %10s, %10.3e, %10s, %10.3e, %10s\n",
                    *itervn, *iterl1, " ",  *iterl2," ",  *iterli, " ");
        }
        itervn++;
        iterl1++;
        iterl2++;
        iterli++;
        count++;
    }
    fout.close();
}


#endif 