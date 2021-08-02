Problem Description
===================

We want to find intersection results of a line vs box. 

.. figure:: _static/{{folder_name}}/illustration.png
   :align: center 

   A line intersects with box.

Results
===================

Line vs Box - Normal
---------------------
The box is fixed at (0,0) to (1,1). A line passes through the box. Two intersect points can be found.

.. figure:: _static/{{folder_name}}/line_box_normal.png
   :align: center 

   A line intersects with box (Normal).

Line vs Box - Corner
---------------------
The box is fixed at (0,0) to (1,1). A corner of the box is on the line.

.. figure:: _static/{{folder_name}}/line_box_corner.png
   :align: center 

   A line intersects with box (Corner).

Line vs Box - Edge
--------------------
The box is fixed at (0,0) to (1,1). The line is on one edge of the box. 

.. figure:: _static/{{folder_name}}/line_box_edge.png
   :align: center 

   A line intersects with box (Edge).

Line vs Box - Positive Side
-----------------------------
A line cuts a box into two sides. Here, we get a positive side as a ploygon.

We want to construct a line rotate about a point, say p1: :math:`(x1, y1)`. p1 is the rotational center. And, we have another point p2: math:`(x2, y2)`. A line can be constructed in two points form.

Given two different points :math:`(x1, y1)` and :math:`(x2, y2)`, there is exactly one line that passes through them. There are several ways to write a linear equation of this line.

If :math:`x1 ≠ x2`, the slope of the line is :math:`\frac{y_{2}-y_{1}}{x_{2}-x_{1}}` Thus, a point-slope form is

.. math::

   y - y_{1} &= \frac{y_2 - y_1}{x_2 - x_1} (x - x_1) \\
   (x_2 - x_1)(y - y_1) &= (y_2 - y_1)(x - x_1)

finally, the line equation will be

.. math::

   (y_1 - y_2) x + (x_2 - x_1) y + (x_1 y_2 - x_2 y_1) = 0

p2 is also a point on a circle. The center of the circle is p1. Radius of this circle is r. Thus, p2 can be obtained by 

.. math::

   x_2 &= x_1 + r \cos \theta \\
   y_2 &= y_1 + r \sin \theta

.. figure:: _static/{{folder_name}}/lb.gif
   :align: center 

   A line intersects with box and its positive side.