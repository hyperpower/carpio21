Problem 1 Two Segments Intersection 
====================================

Description
-------------


We want to find two segments intersection. Here, we define 6 possible cases for the intersect or not.

.. table:: Segement vs. Segment Locations
    :widths: auto
    :align: center 

    =====  ============
     Num.    Type
    =====  ============
      0     NO       
      1     CONNECT  
      2     TOUCH    
      3     OVERLAP  
      4     SAME     
      5     INTERSECT
    =====  ============


Results
-------------

Location 0 NO
^^^^^^^^^^^^^^
Two segments are seperated. There are no intersection between the two segments. 
But function of finding the intersection point is still working.
The intersection point is on neither of the segments.

.. figure:: fig/NO.png
   :alt: Figure intersection NO
   :align: center 

   An example of two segments has no intersection.

Location 1 CONNET
^^^^^^^^^^^^^^^^^^
Two segments are connected. 

.. figure:: fig/CONNECT.png
   :alt: Figure intersection CONNECT
   :align: center 

   An example of two segments are connected.


Location 2 TOUCH
^^^^^^^^^^^^^^^^^^
Two segments are touched.

.. figure:: fig/TOUCH.png
   :alt: Figure intersection TOUCH
   :align: center 

   An example of two segments are touched.


Location 3 OVERLAP
^^^^^^^^^^^^^^^^^^^^^
Two segments are overlap each other. 

.. warning::
    The intersection point can't be found. The point is (0,0) 

.. figure:: fig/OVERLAP.png
   :alt: Figure intersection overlap
   :align: center 

   An example of two segments are overlap.


Location 4 SAME
^^^^^^^^^^^^^^^^^^
Two segments are the same. 

.. warning::
    The intersection point can't be found. The point is the start point of the first segment 

.. figure:: fig/SAME.png
   :alt: Figure intersection SAME
   :align: center 

   An example of two segments are same.

Location 4 INTERSECT
^^^^^^^^^^^^^^^^^^^^^^^
Two segments are intersect.

.. figure:: fig/INTERSECT.png
   :alt: Figure intersection INTERSECT
   :align: center 

   An example of two segments are intersect.


Problem 2 Multiple Segments Intersection 
=========================================

Description
---------------

The input is a set S of segments in the plane (each defined by a pair of points). The output is
a list of all the places where these line segments intersect.

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




