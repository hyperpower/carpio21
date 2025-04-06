
Problem Description
===================

This test case benchmark basic array operations.

Dependency
===================

This benchmark test case is running by **Google-benchmark**.
Several library have been tested:

- raw array
- raw array by SIMD
- std::valarray
- armadillo
- Eigen

Please make sure that these library have been poperly installed.


Benchmark Result
======================

Vector Add
----------------

A possible implimentation of Vector add benchmark function are shown as follows:

.. code-block:: cpp

   template<class VECTORTYPE>
   void BM_VectorAdd(benchmark::State& state){
      for (auto _ : state){
        auto n = state.range(0);
        VECTORTYPE v1(n);
        VECTORTYPE v2(n);
        VECTORTYPE result(n);

        for (decltype(n) i = 0; i < n; ++i){
            v1[i] = i * 1.0;
            v2[i] = 1.0;
        }
        result = v1 + v2;

        benchmark::DoNotOptimize(v1);
        benchmark::DoNotOptimize(v2);
        benchmark::DoNotOptimize(result);
      } 
   }



.. figure:: fig/BM_VectorAdd.png
   :align: center 

   Benchmark result of vetor add.


.. figure:: fig/BM_VectorAdd_ratio.png
   :align: center 

   Speed Ratio to Raw array version.


Loop Unrolling or Not Unroll
=================================

This benchmark shows the loop Unrolling.

.. figure:: fig/BM_BLAS0Add.png
   :align: center 

   Benchmark result of the loop unroll.


.. figure:: fig/BM_BLAS0Add_ratio.png
   :align: center 

   Speed Ratio to normal loop.


