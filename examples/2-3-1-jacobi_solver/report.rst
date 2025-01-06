Problem Description
===================

This example shows Jacobi method for solving :math:`\mathbf{A}x = \mathbf{b}`.

Test Problem
===================
The Matrix is obtained from MatrixMarket. The name is {{ mat_name }}. The equation which we want
to solve:

.. math::
   \mathbf{A} \cdot \mathbf{x} = \mathbf{b}

where, :math:`\mathbf{A}` the matrix, :math:`\mathbf{x}` unknown vector and :math:`\mathbf{b}` is the 
vector on right hand side.


.. table:: Matrix {{mat_name}} Information.
   :widths: auto
   :align: center 

   ================== =======================
   Item               Info   
   ================== =======================
   i                  {{ num_row }}  
   j                  {{ num_col }}   
   none-zero          {{ num_nonezero }}   
   ================== =======================

Solver Test
======================

