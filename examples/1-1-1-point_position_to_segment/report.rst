Problem Description
===================

We want to define the loaction of a point to a segment. This problem is a bit
of complicate than obtaining a point loction to a line. Here, we define 7 possible
loctions, as shown below,

.. table:: Point vs. Segment Locations
    :widths: auto
    :align: center

    =====  ============
     Num.    Type
    =====  ============
      0      IN
      1      LEFT
      2      RIGHT
      3      ON START
      4      ON END
      5      OUT START
      6      OUT END
    =====  ============


Result
===================

Location 0 IN
--------------
The point is inside the Segment

.. figure:: ./fig/IN.png
   :alt: Figure point loaction IN
   :align: center 

   An example of the Point located inside of the Segment

Location 1 LEFT
------------------
The point is on the left of the Segment

.. figure:: ./fig/LEFT.png
   :alt: Figure point loaction LEFT
   :align: center 

   An example of the Point located on the left of the Segment

Location 2 RIGHT
------------------
The point is on the left of the Segment

.. figure:: fig/RIGHT.png
   :alt: Figure point loaction RIGHT
   :align: center 

   An example of the Point located on the right of the Segment

Location 3 ON START
---------------------
The point is on the start point of the Segment

.. figure:: fig/ON_START.png
   :alt: Figure point loaction ON_START
   :align: center 

   An example of the Point located on the start point of the Segment

Location 4 ON END
------------------
The point is on the end point of the Segment

.. figure:: fig/ON_END.png
   :alt: Figure point loaction ON_END
   :align: center 

   An example of the Point located on the end point of the Segment

Location 5 OUT START
---------------------
The point is outside the start point of the Segment, but the point is colinear with the Segment.

.. figure:: fig/OUT_START.png
   :alt: Figure point loaction OUT_START
   :align: center 

   An example of the Point located outside the start point of the Segment

Location 6 OUT END
---------------------
The point is outside the end point of the Segment, but the point is colinear with the Segment.

.. figure:: fig/OUT_END.png
   :alt: Figure point loaction OUT_END
   :align: center 

   An example of the Point located outside the end point of the Segment

