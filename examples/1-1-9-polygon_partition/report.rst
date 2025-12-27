Problem Description
===================

We want to perform Polygon Partition. We want to decompose a polygon into a set of triangles. There are a few methods shown as follows:

- Ear Clipping Method
- Monotone Chain Method

Ear Clipping Method
===================

The ear clipping algorithm is a simple and widely used method for triangulating simple polygons in computational geometry. It decomposes a polygon into a set of triangles without adding new vertices, which is essential for applications like computer graphics rendering, finite element analysis, and 3D modeling.
The algorithm works by iteratively "clipping" ears from the polygon. An ear is a vertex $v_i$ where the diagonal between its two adjacent vertices $v_{i-1}$ and $v_{i+1}$ lies entirely inside the polygon and does not intersect any edges (except at endpoints). Additionally, no other vertices lie inside the triangle formed by $v_{i-1}$, $v_i$, and $v_{i+1}$.

Examples
===================

Case 1
-------------------
The original polygon is shown in :numref:`fig-c1`.

.. _fig-c1:   
.. figure:: ./fig/examplepolygon.png
   :align: center

   Original Polygon

The result is shown in :numref:`gif-c1`.

.. _gif-c1:
.. figure:: ./fig/examplepolygon.gif
   :align: center

   Partition process

Case 2
-------------------
The original polygon is shown in :numref:`fig-c2`.

.. _fig-c2:
.. figure:: ./fig/man.png
   :align: center

   Original Polygon

The result is shown in :numref:`gif-c2`.

.. _gif-c2:
.. figure:: ./fig/man.gif
   :align: center

   Partition process


Case 3
-------------------
The original polygon is shown in :numref:`fig-c3`.

.. _fig-c3:
.. figure:: ./fig/shui.png
   :align: center

   Original Polygon

The result is shown in :numref:`gif-c3`.

.. _gif-c3:
.. figure:: ./fig/shui.gif
   :align: center

   Partition process

