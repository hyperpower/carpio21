Poisson Equation
==============================

Introduction
------------------------------

Consider the Poisson equation 

.. math::
   :label: poisson

   \nabla^2 \phi = s

in Domain :math:`\Omega` with appropriate boundary conditions on :math:`\partial \Omega`.

where,
:math:`s` is a source function.



2-Order Scheme
-------------------------------



Finite Difference Scheme
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. math::

    \frac{\partial \psi}{\partial x_d} \long \mathbf{i}+\frac{1}{2} \mathbf{e}^d

Finite Volume Scheme
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. math::

   \frac{1}{h} \sum_{d=0}^3\left(\left\langle\frac{\partial \phi}{\partial x_d}
   \right\rangle_{\mathbf{i}+\frac{1}{2} \mathbf{e}^d}-\left\langle\frac{\partial \phi}{\partial x_d}
   \right\rangle_{\mathbf{i}-\frac{1}{2} \mathbf{e}^d}\right)
    = \langle f\rangle_{\mathbf{i}}

.. math::
   :label: fvs 
   
   \frac{1}{h} \sum_{d=0}^3\left[\left\langle\frac{\partial \phi}{\partial x_d}\right\rangle\right]_{\mathbf{i}
   -\frac{1}{2} \mathbf{e}^d}^{\mathbf{i}+\frac{1}{2} \mathbf{e}^d}
    = \langle f\rangle_{\mathbf{i}}

向量的定义
+++++++++++++++++