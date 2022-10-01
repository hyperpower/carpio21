
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

Numerical Error
=======================


The error :math:`E` is obtained by

.. math::
   E = \phi_{\text{exact}} - \phi_{\text{compute}}

where, :math:`\phi_{\text{exact}}` is exact solution. :math:`\phi_{\text{compute}}` is compute solution.

We expect error to converge as 

.. math::
  :label: eq_e

   E = C h^m 

where, :math:`C` is a constant, :math:`h` is cell size, :math:`m` is expected converge rate.
:eq:`eq_e` implies

.. math::
  :label: eq_log

   \log E =\log C + m \log h 

Hence, the data :math:`\log E` vs. :math:`\log h` should be asymptotic to a straight
line with slop :math:`m`. If the cell size :math:`h` reduced to :math:`h/2`, 
:math:`m` can be obtained from :eq:`eq_e`.

.. math::

   \frac{E_h}{E_{h/2}} = \frac{h^m}{(h/2)^m} = 2^m

.. math::
  :label: eq_m

   m = \log_2 \frac{E_h}{E_{h/2}}



Second Order Finite Volume Method
==================================

The simple Second Order FVM is applied on this problem.

.. image:: fig/Poisson_SolutionContour10.png
   :width: 49%
.. image:: fig/Poisson_ErrorContour10.png
   :width: 49%
.. figure:: fig/0.svg
   :alt: Fig of Exact Solution
   :align: center 

   Solution in 10x10 mesh (Left: Solution, Right: Error).


.. image:: fig/Poisson_SolutionContour20.png
   :width: 49%
.. image:: fig/Poisson_ErrorContour20.png
   :width: 49%
.. figure:: fig/0.svg
   :alt: Fig of Exact Solution
   :align: center 
   
   Solution in 20x20 mesh (Left: Solution, Right: Error).


.. image:: fig/Poisson_SolutionContour40.png
   :width: 49%
.. image:: fig/Poisson_ErrorContour40.png
   :width: 49%
.. figure:: fig/0.svg
   :alt: Fig of Exact Solution
   :align: center 
   
   Solution in 40x40 mesh (Left: Solution, Right: Error).


.. image:: fig/Poisson_SolutionContour80.png
   :width: 49%
.. image:: fig/Poisson_ErrorContour80.png
   :width: 49%
.. figure:: fig/0.svg
   :alt: Fig of Exact Solution
   :align: center 
   
   Solution in 80x80 mesh (Left: Solution, Right: Error).

.. figure:: fig/residual.png
   :align: center 

   Solver residual on each mesh.

.. csv-table:: Error Norm in different mesh resolutions.
   :file: fig/error_table.txt
   :header-rows: 1

.. figure:: fig/error.png
   :align: center 

   Error Norm in different mesh resolutions.
   


Reference
===================


.. [Barad2005] Barad, M., & Colella, P. (2005).
           A fourth-order accurate local refinement method for Poisson's equation. 
           Journal of Computational Physics, 209(1), 1-18.
           https://doi.org/10.1016/j.jcp.2005.02.027
