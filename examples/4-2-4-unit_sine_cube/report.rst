
Problem Description
===================

This test performs on a unit squre :math:`x \in [0, 1]`, :math:`y \in [0, 1]`, :math:`z \in [0, 1]`.

The Poisson equation defined as:

.. math::
   \Delta \phi = - 3 \pi^2 \text{sin}(\pi x) \text{sin}(\pi y)

:math:`\phi=0` on the entire boundary. Exact solution is,

.. math::
   \phi = \text{sin}( \pi x) \text{sin}( \pi y) \text{sin}( \pi z)

This test is the Problem 2 in [Spotz1996]_. 

.. raw:: html
   :file: fig/exact.div

.. figure:: fig/0.svg
   :align: center

   Exact Solution



Second Order Finite Volume Method
==================================


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


.. [Spotz1996] Spotz, W. F., & Carey, G. F. (1996). 
               A high-order compact formulation for the 3D Poisson equation. 
               Numerical Methods for Partial Differential Equations, 12(2), 235–243.
               https://doi.org/10.1002/(SICI)1098-2426(199603)12:2%3C235::AID-NUM6%3E3.0.CO;2-R