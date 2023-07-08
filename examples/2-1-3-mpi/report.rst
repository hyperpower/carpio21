
Problem Description
===================

This test case just illustrates how to use MPI. The prefromence test has been carried out for comparsion. Note that the perfromence may highly depend on hardware platform. This test program could get different results on an other computer. 

Time Test
===================
OpenMP takes some time to allocate the threads and manage them. This procedure will spend some time. Compare to the time for runing simple task, the time used for managing threads can't not be ignored. Here, we want to find out what kind of task is suitalbe for paralleling in terms of excution time. A simple sleep funtion is used as single task function. Multiple threads, say n threads, will execut the function n times. If they are runing in prefect parallel, the execution time time will be as same as the single thread running the funtion for just one time. 

We set the number of threads, n = 2, 4, 8, 16. The result is shown in following Figure.   

.. figure:: fig/timetest.png
   :alt: Time test for OpenMP
   :align: center 

   Time test for OpenMP

Two obvious conclusions can be reached:

- More threads will take more time to manage.
- Execution time of single function is better be more than 10 millisecond.

Array Operation Test
======================
A simple array operation(+,-,*) execute in parallel. Following figure shows the result. The operation is acted on arrays with different length. The longer array takes more time to operate. The execution time should be longer than 10 ms. The parallel execution could save time. 

.. figure:: fig/arrayop.png
   :alt: arrar test for OpenMP
   :align: center 

   Array test for OpenMP