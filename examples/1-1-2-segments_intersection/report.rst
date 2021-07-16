Problem Description
===================

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
===================

Location 0 NO
--------------
Two segments are seperated. There are no intersection between the two segments. But function of finding the intersection point is still working. The intersection point is on neither of the segments.

.. figure:: _static/{{folder_name}}/NO.png
   :alt: Figure intersection NO
   :align: center 

   An example of two segments has no intersection.

Location 1 CONNET
------------------
Two segments are connected. 

.. figure:: _static/{{folder_name}}/CONNECT.png
   :alt: Figure intersection CONNECT
   :align: center 

   An example of two segments are connected.


Location 2 TOUCH
------------------
Two segments are touched.

.. figure:: _static/{{folder_name}}/TOUCH.png
   :alt: Figure intersection TOUCH
   :align: center 

   An example of two segments are touched.


Location 3 OVERLAP
--------------------
Two segments are overlap each other. 

.. warning::
    The intersection point can't be found. The point is (0,0) 

.. figure:: _static/{{folder_name}}/OVERLAP.png
   :alt: Figure intersection overlap
   :align: center 

   An example of two segments are overlap.


Location 4 SAME
--------------------
Two segments are the same. 

.. warning::
    The intersection point can't be found. The point is (0,0)

.. figure:: _static/{{folder_name}}/SAME.png
   :alt: Figure intersection SAME
   :align: center 

   An example of two segments are same.

Location 4 INTERSECT
---------------------
Two segments are intersect.

.. figure:: _static/{{folder_name}}/INTERSECT.png
   :alt: Figure intersection INTERSECT
   :align: center 

   An example of two segments are intersect.
