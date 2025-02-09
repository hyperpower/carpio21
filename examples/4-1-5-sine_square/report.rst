Problem Description
===================
This case is the test 2 in [Zhang2015]_.
The domain is a unit square :math:`x \in [0.0, 1.0], y \in [0.0, 1.0]`.
The boudary conditions are

.. math::
   \phi =\sin^2(\frac{10}{3} \pi y) &, \quad \text{if} \quad x = 0.0 \quad \text{and} \quad y = [0.0, 0.3]\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 1.0\\
   \phi = 0 &, \quad \text{if} \quad y = 0.0\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0\\

The velocity is :math:`\mathbf{u}=(1, 1)`. 

.. figure:: fig/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.


Results
===================

This example tests three GRP (Generalized polynomial-ratio) schemes.

.. figure:: fig/GPR0_iter.gif
   :alt: the gif
   :align: center 

   GPR0.


.. figure:: fig/GPR0_section.png
   :alt: the gate profile
   :align: center 

   GPR0. (x = 0.6, Section A-A)


.. figure:: fig/GPR12_iter.gif
   :alt: the gif
   :align: center 

   GPR12.


.. figure:: fig/GPR12_section.png
   :alt: the gate profile
   :align: center 

   GPR12. (x = 0.6, Section A-A)


.. figure:: fig/GPR13_iter.gif
   :alt: the gif
   :align: center 

   GPR13.


.. figure:: fig/GPR13_section.png
   :alt: the gate profile
   :align: center 

   GPR13. (x = 0.6, Section A-A)

Compare
------------------
.. figure:: fig/section_compare.png
   :align: center 

   Comparison all schemes (x = 0.6, Section A-A).

Reference
===================
 
.. [Zhang2015] Zhang, D., etc (2015).
           A Review on TVD Schemes and a Refined Flux-Limiter for Steady-State Calculations.
           Journal of Computational Physics 302(12), 114-54. 
           https://doi.org/10.1016/j.jcp.2015.08.042



