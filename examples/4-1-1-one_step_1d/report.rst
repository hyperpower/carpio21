Problem Description
===================

The 1D step profile has been initialized as:

.. math::
   x &\in [0.0, 0.5]   , \phi = 1,  \\
   x &\in [0.5, 2.0], \phi = 0
   :label: eq_initial

The velocity is :math:`u=1` m/s. After 1 second (100 steps with :math:`\Delta t = 0.01`), the final profile should be 

.. math::
   x \in [0.0, 1.5]&, \phi = 1,  \\
     else          &, \phi = 0
   :label: eq_final 

.. figure:: fig/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.

Results
===================

First Order Upwind
-------------------

.. figure:: fig/FOU_iter.gif
   :alt: the gate profile
   :align: center 

   First Order Upwind.

.. figure:: fig/FOU_phi_100.png
   :alt: the gate profile
   :align: center 

   First Order Upwind (Last Step).



