
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

A possible implimentation of Vector add function are shown as follows:

.. code-block:: cpp

    // new 2 vector 
    double* a = new double[n];
    double* b = new double[n];
    double* result = new double[n];
    // set initial value
    for (decltype(n) i = 0; i < n; ++i) {
      a[i] = i * 1.0;
      b[i] = 1.0;
    }
    // two add functions
    Add_(n, a, b, result);
    Add_(n, a, b, result);

    // delete values
    delete[] a;
    delete[] b;
    delete[] result;



.. figure:: fig/bm_vector_add.png
   :align: center 

   Benchmark result of vetor add.


.. figure:: fig/bm_vector_add_boxr.png
   :align: center 

   Speed Ratio to Raw array version.
