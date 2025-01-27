
Problem Description
===================

This test performs on a unit squre :math:`x \in [0, 1]`, :math:`y \in [0, 1]`.

The Laplace equation defined as:

.. math::
   \Delta \phi = 0

This test problem have fixed value boundary conditions. 

.. figure:: fig/illustration.png
   :alt: Fig of Problem illustration
   :align: center 

   Definition of the solution region.


Solved Result
=======================

Laplace equation can be solved directly. Results are shown in :numref:`fig-solved`

.. _fig-solved:
.. figure:: fig/Solved_Phi.png
   :alt: Fig of Solve_Phi
   :align: center 
   
   Solution.

Iteration
===================

Explicit Scheme
^^^^^^^^^^^^^^^^

.. _fig-ex-n1:
.. figure:: fig/explicit_Norm1Compare.png
   :alt: Fig of Explicit norm 1
   :align: center 
   
   Error Norm 1 of explicit iteration

.. _fig-ex-n2:
.. figure:: fig/explicit_Norm2Compare.png
   :alt: Fig of Explicit norm 1
   :align: center 
   
   Error Norm 2 of explicit iteration

.. _fig-ex-ninf:
.. figure:: fig/explicit_NormInfCompare.png
   :alt: Fig of Explicit norm Inf
   :align: center 
   
   Error Norm inf of explicit iteration


Implicit Scheme
^^^^^^^^^^^^^^^^

The process of the interation is shown in :numref:`fig-im-iter`. 
The value of :math:`\phi` proximates to the solved value in :numref:`fig-solved`.

.. _fig-im-iter:
.. figure:: fig/implicit_iter.gif
   :alt: Fig of Implicit norm 1
   :align: center 
   
   Process of implicit iteration

.. _fig-im-n1:
.. figure:: fig/implicit_Norm1Compare.png
   :alt: Fig of Implicit norm 1
   :align: center 
   
   Error Norm 1 of implicit iteration

.. _fig-im-n2:
.. figure:: fig/implicit_Norm2Compare.png
   :alt: Fig of Implicit norm 1
   :align: center 
   
   Error Norm 2 of implicit iteration

.. _fig-im-ninf:
.. figure:: fig/implicit_NormInfCompare.png
   :alt: Fig of Implicit norm Inf
   :align: center 
   
   Error Norm inf of implicit iteration

Crank–Nicolson Scheme
^^^^^^^^^^^^^^^^^^^^^^

.. _fig-cn-n1:
.. figure:: fig/CN_Norm1Compare.png
   :alt: Fig of Crank–Nicolson norm 1
   :align: center 
   
   Error Norm 1 of Crank–Nicolson iteration

.. _fig-cn-n2:
.. figure:: fig/CN_Norm2Compare.png
   :alt: Fig of Crank–Nicolson norm 1
   :align: center 
   
   Error Norm 2 of Crank–Nicolson iteration

.. _fig-cn-ninf:
.. figure:: fig/CN_NormInfCompare.png
   :alt: Fig of Crank–Nicolson norm Inf
   :align: center 
   
   Error Norm inf of Crank–Nicolson iteration

