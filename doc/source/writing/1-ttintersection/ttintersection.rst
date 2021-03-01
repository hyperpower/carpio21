Triangle intersection method
==============================

This file used for testing reStructureText file

Basic Vector operator
----------------------

Vector Define
+++++++++++++++++

vector :math:`\mathbf{a}` belongs to :math:`\mathbf{R^3}`

.. math::
   :label: vector_define

   \mathbf{a} = (a_0, a_1, a_2)

Cross product
+++++++++++++++++
The cross product of two vectors a and b is defined only in three-dimensional space and is denoted by :math:`\mathbf{a} \times \mathbf{b}`.

.. math::
   :label: cross_product1

   \mathbf{a} \times \mathbf{b} = 
   \begin{vmatrix} 
   \mathbf{i} & \mathbf{j} & \mathbf{k}  \\
   a_{0} & a_{1} & a_{2}  \\
   b_{0} & b_{1} & b_{2}  \\
   \end{vmatrix} 

.. math::
   :label: cross_product2

   \mathbf{a} \times \mathbf{b} = 
   \begin{vmatrix} 
   a_{1} & a_{2} \\
   b_{1} & b_{2} \\
   \end{vmatrix} 
   \mathbf{i}
   -
   \begin{vmatrix} 
   a_{0} & a_{2} \\
   b_{0} & b_{2} \\
   \end{vmatrix}
   \mathbf{j}
   +
   \begin{vmatrix} 
   a_{0} & a_{1} \\
   b_{0} & b_{1} \\
   \end{vmatrix}
   \mathbf{j}

.. math::
   :label: cross_product3

   \mathbf{a} \times \mathbf{b} = 
   (a_1b_2-b_1a_2)
   \mathbf{i}
   -
   (a_0b_2-b_0a_2)
   \mathbf{j}
   +
   (a_0b_1-b_0a_1)
   \mathbf{j}



H4 -- Subsubsection
+++++++++++++++++++
