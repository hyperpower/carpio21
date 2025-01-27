
Problem Description
===================

This test performs on a unit cube :math:`x \in [0, 1]`, :math:`y \in [0, 1]`, :math:`z \in [0, 1]`.

The Laplace equation defined as:

.. math::
   \Delta \phi = 0

:math:`\phi= \text{sin}( \pi y) \text{sin}( \pi z)`, :math:`x=0`,

:math:`\phi= 2 \text{sin}( \pi y) \text{sin}( \pi z)`, :math:`x=1`,

:math:`\phi= 0`, :math:`y, z ={0, 1}`,

Exact solution is,

.. math::
   \phi = \frac{\text{sin}( \pi y) \text{sin}( \pi z)}{\text{sinh}( \pi \sqrt{2})}
   [2 \text{sinh}( \pi \sqrt{2} x)  + \text{sinh}( \pi \sqrt{2} (1-x) )]

This test is the Problem 1 in [Spotz1996]_. 

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