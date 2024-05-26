Problem Description
===================

We want to find intersection results of a Line vs Box. 

.. figure:: fig/illustration.png
   :align: center 

   A line intersects with box.

Line Box
=======================

Intersections
---------------------

Normal
^^^^^^^^^^^^^^^^^^^^
The box is fixed at (0,0) to (1,1). A line passes through the box. Two intersect points can be found.

.. figure:: fig/line_box_normal.png
   :align: center 

   A line intersects with box (Normal).

Corner
^^^^^^^^^^^^^^^^^^^^
The box is fixed at (0,0) to (1,1). A corner of the box is on the line.

.. figure:: fig/line_box_corner.png
   :align: center 

   A line intersects with box (Corner).

Edge
^^^^^^^^^^^^^^^^^^^^
The box is fixed at (0,0) to (1,1). The line is on one edge of the box. 

.. figure:: fig/line_box_edge.png
   :align: center 

   A line intersects with box (Edge).

Positive Polygon
-----------------------------
A line cuts a box into two sides. Here, we get a positive side as a ploygon.

We want to construct a line rotate about a point, say p1: :math:`(x_1, y_1)`. p1 is the rotational center. And, we have another point p2: :math:`(x_2, y_2)`. A line can be constructed from these two points.

Given two different points :math:`(x_1, y_1)` and :math:`(x_2, y_2)`, there is exactly one line that passes through them. There are several ways to write a linear equation.

If :math:`x_1 ≠ x_2`, the slope of the line is :math:`\frac{y_{2}-y_{1}}{x_{2}-x_{1}}`. Thus, a point-slope form is

.. math::

   y - y_1 &= \frac{y_2 - y_1}{x_2 - x_1} (x - x_1) \\
   (x_2 - x_1)(y - y_1) &= (y_2 - y_1)(x - x_1)

finally, the line equation will be

.. math::

   (y_1 - y_2) x + (x_2 - x_1) y + (x_1 y_2 - x_2 y_1) = 0

p2 is also a point on a circle. The center of the circle is p1. Radius of this circle is :math:`r`. Thus, p2 can be obtained by 

.. math::

   x_2 &= x_1 + r \cos \theta \\
   y_2 &= y_1 + r \sin \theta

.. figure:: fig/lb.gif
   :align: center 

   A line intersects with box and its positive side.

Negative Polygon
-----------------------------

The figure shows the both side of a line splitted box. The positive side colored in blue. And, the negative side colored in green.

.. figure:: fig/pn_01.png
   :align: center 

   A line intersects with box and its positive and negative sides.

.. figure:: fig/pn_02.png
   :align: center 

   A line intersects with box and its positive and negative sides.

Plane Box
==========================

Intersections
---------------------

Normal
^^^^^^^^^^^^^^^^^^^^

A plane passes through the box. Multiple intersect points can be found.

.. raw:: html
   :file: fig/plane_box_normal.div

.. figure:: fig/0.svg
   :align: center

   A Plane intersects with box (Normal).

Corner
^^^^^^^^^^^^^^^^^^^^

A plane touches the box on corner.

.. raw:: html
   :file: fig/plane_box_corner.div

.. figure:: fig/0.svg
   :align: center

   A Plane intersects with box (Corner).


Edge
^^^^^^^^^^^^^^^^^^^^

A plane touches the box on edge.

.. raw:: html
   :file: fig/plane_box_edge.div

.. figure:: fig/0.svg
   :align: center

   A Plane intersects with box (Edge).