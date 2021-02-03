#include "algebra/solver/jacobi.hpp"
#include "algebra/algebra.hpp"
#include "gtest/gtest.h"
#include "utility/clock.hpp"

using namespace carpio;

TEST(solver, direct1) {
	typedef MatrixV_<Vt> Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt> Solver;
	double rawA[4 * 4] = {
			10., -1., 2., 0.,
			-1., 11., -1., 3.,
			2., -1., 10., -1.,
			0.0, 3., -1., 8. };
	Mat A(4, 4, 16, rawA);
	Arr b { 6., 25., -11., 15. };

	Solve(A, b);
	b.show();
}


TEST(solver, jacobi_dense1){
	typedef MatrixV_<Vt>    Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt>     Solver;
	double  rawA[4*4] = {10., -1.,  2., 0.,
	                    -1., 11., -1., 3.,
	                     2., -1., 10., -1.,
	                     0.0, 3., -1., 8.};
	Mat A( 4, 4, 16, rawA);
	Arr b{ 6., 25., -11., 15.};
	Arr x( 4);
	Arr expected_x{ 1.0, 2.0, -1.0, 1.0 };
	int n = 4;

	Solver solver(1000, 1e-6);
	solver.solve(A, x, b);
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;

	// checking if all values are correct (up to some error value)
	for (int i = 0; i < n; ++i) {
		EXPECT_NEAR(expected_x[i], x[i], 1e-3) << " element y[" << i
				<< "] is wrong";
	}

}

TEST(solver, jacobi_scr1){
	typedef MatrixV_<Vt>    Mat;
	typedef MatrixSCR_<Vt>  MatSCR;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt>     Solver;
	double  rawA[4*4] = {10., -1.,  2., 0.,
	                    -1., 11., -1., 3.,
	                     2., -1., 10., -1.,
	                     0.0, 3., -1., 8.};
	Mat A( 4, 4, 16, rawA);
	Arr b{ 6., 25., -11., 15.};
	Arr x( 4);
	Arr expected_x{ 1.0, 2.0, -1.0, 1.0 };
	MatSCR Ascr(A);
	int n = 4;

	Solver solver(1000, 1e-6);
	solver.solve(Ascr, x, b);
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;

	// checking if all values are correct (up to some error value)
	for (int i = 0; i < n; ++i) {
		EXPECT_NEAR(expected_x[i], x[i], 1e-3) << " element y[" << i
				<< "] is wrong";
	}
}

TEST(solver, jacobi_scr2){
	typedef MatrixV_<Vt>    Mat;
	typedef MatrixSCR_<Vt>  MatSCR;
	typedef MatrixSCO_<Vt>  MatSCO;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt>     Jacobi;
	typedef CG_<Vt>         Solver;
	MatSCO mat;
	mm_read_mtx_sparse("./test/input_files/mm/685_bus.mtx", mat);
	std::cout << "Num. Row = " << mat.size_i()    << std::endl;
	std::cout << "Num. Col = " << mat.size_j()    << std::endl;
	std::cout << "Num. NZ  = " << mat.non_zeros() << std::endl;
//	std::cout << "Is sym   = " << mat.
	// Transfer to SCR
	MatSCR matr(mat);
	Arr    x(mat.size_i());
	x.assign(0.0);

	Arr    b(mat.size_i());
	b.assign(1.0);
	std::cout << "Jacobi Soler =============\n";
	tick_t start = Clock::Tick();  // =====>
	Jacobi solver(1000, 1e-4);
	solver.solve(matr, x, b);
	tick_t end = Clock::Tick();    // <====

	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;

}

TEST(solver, CG_scr2){
	typedef MatrixV_<Vt>    Mat;
	typedef MatrixSCR_<Vt>  MatSCR;
	typedef MatrixSCO_<Vt>  MatSCO;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt>     Jacobi;
	typedef CG_<Vt>         CG;
	typedef CGS_<Vt>        CGS;
	typedef BiCG_<Vt>       BiCG;
	typedef BiCGSTAB_<Vt>   BiCGSTAB;
	MatSCO mat;
	mm_read_mtx_sparse("./test/input_files/mm/685_bus.mtx", mat);

	int maxiter = 1000;
	Vt  tol     = 1e-6;

	// Transfer to SCR
	MatSCR matr(mat);
	Arr    x(mat.size_i());
	x.assign(0.0);

	Arr    b(mat.size_i());
	b.assign(1.0);
	std::cout << "CG Soler =============\n";
	tick_t start = Clock::Tick();  // =====>
	CG solver("IC", maxiter, tol);
	solver.solve(matr, x, b);
	tick_t end = Clock::Tick();    // <====
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;


	std::cout << "CGS Soler =============\n";
	x.assign(0.0);
	b.assign(1.0);
	start = Clock::Tick();  // =====>
	CGS cgs(maxiter, tol);
	int rc = cgs.solve(matr, x, b);
	end = Clock::Tick();    // <====
	std::cout << "ret code = " << rc << std::endl;
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;

	std::cout << "BiCG Soler =============\n";
	x.assign(0.0);
	b.assign(1.0);
	start = Clock::Tick();  // =====>
	BiCG bicg(maxiter, tol);
	rc = bicg.solve(matr, x, b);
	end = Clock::Tick();    // <====
	std::cout << "ret code = " << rc << std::endl;
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end)<< "ms" << std::endl;

	std::cout << "BiCGSTAB Soler =============\n";
	x.assign(0.0);
	b.assign(1.0);
	start = Clock::Tick();  // =====>
	BiCGSTAB bicgstab(maxiter, tol);
	rc = bicgstab.solve(matr, x, b);
	end = Clock::Tick();    // <====
	std::cout << "ret code = " << rc << std::endl;
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end)
			<< "ms" << std::endl;

	std::cout << "BiCGSTAB Dia Soler =============\n";
	x.assign(0.0);
	b.assign(1.0);
	start = Clock::Tick();  // =====>
	BiCGSTAB bicgstab_dia("Dia", maxiter, tol);
	rc = bicgstab_dia.solve(matr, x, b);
	end = Clock::Tick();    // <====
	std::cout << "ret code = " << rc << std::endl;
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end)
			<< "ms" << std::endl;

	std::cout << "BiCGSTAB IC Soler =============\n";
	x.assign(0.0);
	b.assign(1.0);
	start = Clock::Tick();  // =====>
	BiCGSTAB bicgstab_ic("IC", maxiter, tol);
	rc = bicgstab_ic.solve(matr, x, b);
	end = Clock::Tick();    // <====
	std::cout << "ret code = " << rc << std::endl;
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
	std::cout << "d   time = " << Clock::TimespanToMillisecondsD(start, end)
			<< "ms" << std::endl;
}

TEST(solver, DISABLED_parallel){
	typedef MatrixV_<Vt>    Mat;
	typedef MatrixSCR_<Vt>  MatSCR;
	typedef MatrixSCO_<Vt>  MatSCO;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt>     Solver;
	MatSCO mat;
	mm_read_mtx_sparse("./test/input_files/mm/685_bus.mtx", mat);
	std::cout << "Num. Row = " << mat.size_i()    << std::endl;
	std::cout << "Num. Col = " << mat.size_j()    << std::endl;
	std::cout << "Num. NZ  = " << mat.non_zeros() << std::endl;
#ifdef OPENMP
	std::cout << "OPENMP\n";
	omp_set_num_threads(4);
	tick_t start = Clock::Tick();
	// Transfer to SCR
	MatSCR matr(mat);
	Arr    x(mat.size_i());
	x.assign(0.0);

	Arr    b(mat.size_i());
	b.assign(1.0);
	Solver solver(100, 1e-4);
	solver.solve(matr, x, b);
	tick_t end = Clock::Tick();
	std::cout << "D Time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;
#endif
}







