#include <iostream>
#include <memory> 
#include <string>
#include "utility/clock.hpp"
#ifdef OPENMP
#include <omp.h>
#endif 
#include "algebra/algebra.hpp"
// #include "algebra/io/gnuplot_actor_a.hpp"
#include "io/gnuplot.hpp"
#include "io/text_file.hpp"
#include "utility/tinyformat.hpp"
#include <memory>

using namespace carpio;
typedef MatrixV_<Vt>    Mat;
typedef MatrixSCR_<Vt>  MatSCR;
typedef MatrixSCO_<Vt>  MatSCO;
typedef ArrayListV_<Vt> Arr;
typedef Jacobi_<Vt>     Solver;


void plot_residual(const std::string& fn, const Solver& s){
	Gnuplot gnu;
	gnu.set_terminal_png(fn);
	gnu.set_grid();
	gnu.set_ylogscale();
	// gnu.set_xlogscale();
	gnu.set_yformat("%L");
	gnu.set_xlabel("Iterations (-)");
	gnu.set_ylabel("Residual (-)");
	auto rl = ToGnuplotActor(s.get_residual_array());
	rl.line_color_black();
	gnu.add(rl);
	gnu.plot();
}

void Read_Matrix(TextFile& f, const std::string& name, MatSCO& mat){
	// std::string name = "685_bus.mtx";
	mm_read_mtx_sparse("./" + name, mat);
	f.add_line(tfm::format("    \"mat_name\" : \"%s\",", name));
	f.add_line(tfm::format("    \"num_row\" : %d,", mat.size_i()));
	f.add_line(tfm::format("    \"num_col\" : %d,", mat.size_j()));
	f.add_line(tfm::format("    \"num_nonezero\": %d", mat.non_zeros()));
}

// input singal task time in milliseconds
// ouput wall time of parallel runing
void SolverTest(const MatSCR& mat){
    auto start = std::chrono::steady_clock::now();

	Arr    x(mat.size_i());
	x.assign(0.0);

	Arr    b(mat.size_i());
	b.assign(1.0);
	Solver solver(10000, 1e-7);
	solver.solve(mat, x, b);
	auto end = std::chrono::steady_clock::now();
	plot_residual("./fig/residual", solver);
  	double dt = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "D Time = " << dt << "s" << std::endl;
}

double SolverTimeTest(const MatSCR& mat, int nt){
    auto start = std::chrono::steady_clock::now();
	Solver solver(10000, 1e-7);
#ifdef OPENMP
	omp_set_num_threads(nt);
	Arr    x(mat.size_i());
	x.assign(0.0);
	Arr    b(mat.size_i());
	b.assign(1.0);
	solver.solve(mat, x, b);
#else
	std::cout << "No OpenMP " << std::endl;
#endif
	auto end = std::chrono::steady_clock::now();
  	double dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "D Time = " << dt << "ms" << std::endl;
	return dt;
}



void TimeTestPlot(const MatSCR& mat){
	Gnuplot gnu;
	gnu.set_terminal_png("./fig/timetest");
	gnu.set_grid();
	// gnu.set_xlogscale();
	gnu.set("key left top");
	// gnu.set_yrange(80, 300.0);
	gnu.set_xlabel("Number of Threads (-)");
	gnu.set_ylabel("Parallel Wall Time (s)");
	std::vector<int> vnt = {1, 2, 4, 8, 16};
	gnu.set_xrange(vnt.front(), vnt.back());
	std::vector<int> vdt;
	for(auto& nt: vnt){
		std::cout << "Run by " << nt << " Threads" << std::endl;
		auto dt = SolverTimeTest(mat, nt);
		vdt.push_back(dt);
	}
	auto a = ToGnuplotActor(vnt, vdt);
	a.command("using 1:2 title \"Wall time\"");
	a.style("with linespoints ps 2");
	a.line_color_black();
	a.line_width(2);
	a.point_type(7);
	gnu.add(a);
	gnu.plot();
}

int main(int argc, char** argv) {
	std::cout << "===== Example Jacobi Solver =====" << std::endl;
	TextFile file("res.py");
	file.add_line("RES = {");
	MatSCO mato;
	Read_Matrix(file, "685_bus.mtx", mato);
	file.add_line("}");
	file.write();
	// to scr
	MatSCR mat(mato);
	SolverTest(mat);

	TimeTestPlot(mat);

	// solver time test
}