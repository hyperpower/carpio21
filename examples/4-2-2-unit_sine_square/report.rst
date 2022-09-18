
Problem Description
===================

This test performs on a unit squre :math:`x \in [0, 1]`, :math:`y \in [0, 1]`.

The Poisson equation defined as:

.. math::
   \Delta \phi = - 8 \pi^2 \text{sin}(2 \pi x) \text{sin}(2 \pi y)

This test problem has a doubly periodic boundary condition. Exact solution is,

.. math::
   \phi = \text{sin}(2 \pi x) \text{sin}(2 \pi y)

This test is in [Barad2005]_.

.. figure:: fig/ExactSolutionContour.png
   :alt: Fig of Exact Solution
   :align: center 

   Exact Solution.

Second Order Finite Volume Method
==================================

The simple Second Order FVM is applied on this problem. 

.. figure:: fig/Poisson_SolutionContour.png
   :alt: Fig of Exact Solution
   :align: center 

   Second Order Solution.



Field Face
===================

The data store on the face of grid cell.


.. [Barad2005] Barad, M., & Colella, P. (2005).
           A fourth-order accurate local refinement method for Poisson's equation. 
           Journal of Computational Physics, 209(1), 1-18.
           https://doi.org/10.1016/j.jcp.2005.02.027
