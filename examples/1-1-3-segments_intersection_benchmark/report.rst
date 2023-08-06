Problem 1 Multiple Segments Intersection 
=========================================

Description
---------------

The input is a set S of segments in the plane (each defined by a pair of points). The output is
a list of all the places where these line segments intersect.

Benchmark comparision based on four methods:

- Trivial n2 method
- Simple sweep line method
- Bentley-Ottmann method
- Bentley-Ottmann method (CGAL implimentation)

Method 1 Trivial n2
^^^^^^^^^^^^^^^^^^^^^^^

There’s a trivial :math:`O(n^2)` algorithm: Just apply segment intersection to all pairs of segments.
 
Method 2 Sweep Line -- Simple
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Generally speaking sweepline means that you are processing the data in some order (e.g. left to
right order). A data structure is maintained that keeps the information orgnised from the part of
the data currently to the left of the sweepline.

Method 3 Sweep Line -- Bentley-Ottmann
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Example
-------------------------------

Case1 - Normal Intersection
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Segemnts are in trivial case. None of them are vertial nor horizontal. Intersections are expected on segement, not 
on segment ends. Input data are shown in :numref:`table-case1`.


.. _table-case1:
.. csv-table:: Input Segments for case1
   :file: fig/bo_case1.txt
   :widths: 25, 25, 25, 25
   :header-rows: 1


Intersection results are shown in :numref:`fig-bo_case1`.


.. _fig-bo_case1:
.. figure:: fig/bo_case1.png
   :alt: Figure intersection case1
   :align: center 

   Normal intersections
   

Benchmark
-------------------------------

Random Segments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Different amount of segments have been randomly generated. The methods mentioned have been tested.

.. figure:: fig/benchmark.png
   :alt: Figure intersection benchmark
   :align: center 

   benchmark of two intersection methods



