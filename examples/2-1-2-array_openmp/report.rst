
Problem Description
===================

This test case just illustrates how to use OpenMP. The prefromence test has been carried out for comparsion. Note that the perfromence may highly depend on hardware platform. This test program could get different results on an other computer. 

Time Test
===================
OpenMP takes some time to allocate the threads and manage them. This procedure will spend some time. Compare to the time for runing simple task, the time used for managing threads can't not be ignored. Here, we want to find out what kind of task is suitable for paralleling in terms of execution time. A simple sleep funtion is used as single task function. Multiple threads, say n threads, will execut the function n times. If they are runing in prefect parallel, the execution time time will be as same as the single thread running the funtion for just one time. 

We set the number of threads, n = 2, 4, 8. The ratio of acceleration defined as follows:

.. math::
   r = \frac{\Delta t_s}{\Delta t_p n_t}

where, :math:`r` is ratio, :math:`\Delta t_s` is serial execution time, :math:`\Delta t_p` is parallel execution time, :math:`n_t` is number of threads.
The results shown in following Figure.   

.. figure:: fig/timetest.png
   :alt: Time test for OpenMP
   :align: center 

   Time test for OpenMP

Two obvious conclusions can be reached:

- More threads will take more time to manage.
- The longer execution time of the single function, the better parallel execution time will be obtained.

Array Operation Test
======================
A simple array operation(+,-,*) execute in parallel. Following figure shows the result. The operation is acted on arrays with different length. The longer array takes more time to operate.

.. figure:: fig/arrayop.png
   :alt: arrar test for OpenMP
   :align: center 

   Array test for OpenMP