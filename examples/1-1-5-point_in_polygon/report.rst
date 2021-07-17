Problem Description
===================

The classical **Winding Number** test for a point in a polygon. The function will return a bool value:

  - **True** a point is in or on the polygon (inside or on the edge of the polygon).
  - **False** a point is outside of the polgon. 

Results
===================

Random points test
-------------------
Few points have been generated randomly. The funtion **IsInOn(Point, PointChain)** is used for test the point location. The results is shown as follows:

.. figure:: _static/{{folder_name}}/random_points.png
   :alt: Points location to a polygon
   :align: center 

   Random points location to a polygon.

Special case test
------------------
The first specical case is that the point is *on the edge* of the polgon. The winding number method will return **True**.

.. figure:: _static/{{folder_name}}/edge_points.png
   :alt: Point on the edge of a polygon
   :align: center 

   Points are on the edges of a polygon.


The second specical case is that the point is *on the vertex* of the polgon. 

.. figure:: _static/{{folder_name}}/vertex_points.png
   :alt: Point on the vertex of a polygon
   :align: center 

   Points are on the vertrices of a polygon.



