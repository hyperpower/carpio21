Problem Description
===================

This example shows a square domain $\Omega = [0, 1] \times [0, 1]$ with a
circle centered at $(0.5, 0.5)$ and radius $0.25$.

.. figure:: fig/problem_description.png
   :align: center

The domain is discretized by a uniform grid with $N \times N$ cells. For each
cell, the area covered by the circle is computed with ``IntersectAreaCircleBox``
and used as the prescribed volume fraction.

.. figure:: fig/fraction_preset.png
   :align: center

Interface Reconstruction
========================

The problem is to reconstruct the circular interface from the volume fractions
in cells. We refer to an algorithm for solving this problem as a volume-of-fluid
**interface reconstruction algorithm**.

PLIC
----

PLIC: piecewise linear interface calculation is a numerical representation of
the interface using a line segment with an arbitrary orientation.

Youngs
++++++

Youngs' Method is one of methods to evaluate the interface normal. Present
method descript in [Scardovelli2003]_.

.. figure:: fig/reconstruct_youngs_n16.png
   :align: center

.. csv-table:: Error Norm of Youngs Reconstruction
   :file: fig/youngs_error_table.txt

Centered Columns Scheme
+++++++++++++++++++++++

Method Descript in [Scardovelli2003]_, [Pilliod2004]_.

.. figure:: fig/reconstruct_ccs_n16.png
   :align: center

.. csv-table:: Error Norm of CCS Reconstruction
   :file: fig/ccs_error_table.txt

Reference
=========

.. [Scardovelli2003] Scardovelli, R., Zaleski, S., 2003.
                     Interface reconstruction with least-square fit and split Eulerian-Lagrangian advection.
                     Numerical Methods in Fluids 41, 251-274. https://doi.org/10.1002/fld.431

.. [Pilliod2004] Pilliod Jr., J.E., Puckett, E.G., 2004. Second-order accurate volume-of-fluid algorithms for tracking material interfaces. Journal of Computational Physics 199, 465-502. https://doi.org/10.1016/j.jcp.2003.12.023
