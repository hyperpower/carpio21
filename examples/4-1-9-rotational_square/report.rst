Problem Description
===================

The domain is a square :math:`x \in [0, 80], y \in [0, 80]`. The boudary conditions are

.. math::
   \frac{d \phi}{d x} = 0 

The velocity is 

.. math::
   u = -(y - 40)\frac{2 \pi}{360} 


.. math::
   v = (x - 40)\frac{2 \pi}{360} 

Initial condition of the domain is

.. math::
   \phi = 1, \quad \text{if} \in [16, 24], \quad y \in [44, 36]

.. math::
   \phi = 0, \quad \text{elsewhere}

.. figure:: fig/illustration.png
   :alt: rotational_square
   :align: center 

   Illustration of the problem.

It t = 360, the square will rotate back.

Results
===================

First Order Upwind
--------------------
The movement of the profile is shown as follows.

.. figure:: fig/fou_phi.gif
   :alt: FOU gif
   :align: center 

   Convection of the square.

Final Profile Comparsion
--------------------------
The final profile should equal to initial profile. But, different scheme will have different results.

.. figure:: fig/compare_section_x.png
   :alt: rotate x
   :align: center 

   Comparsion of schemes (x=20).

.. figure:: fig/compare_section_y.png
   :alt: rotate x
   :align: center 

   Comparsion of schemes (y=40).