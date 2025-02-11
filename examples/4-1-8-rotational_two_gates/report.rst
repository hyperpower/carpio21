Problem Description
===================

The domain is a unit square :math:`x \in [-1.0, 1.0], y \in [0.0, 1.0]`. The boudary conditions are

.. math::
   \phi = 1 &, \quad \text{if} \quad y = 0.0 \quad \text{and} \quad y = [-0.8, -0.6] \quad \text{or} \quad y = [-0.4, -0.2]\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 0 \quad \text{and} \quad x = 1\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 0.0 \quad \text{and} \quad x = [0.0, 1.0]\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0 \\

The velocity is :math:`\mathbf{u}=(y, -x)`. 

.. figure:: fig/illustration.png
   :alt: two gates
   :align: center 

   Illustration of the problem.

Results
===================

Scheme Comparsion
-------------------
Initial condition of the domain is :math:`\phi = 0.0`. The iteration stops when 3 norms are all less than :math:`10^{-5}`. CFL number is 0.4. 

The final profile at outlet section (x = [0.0, 1.0] and y = 0.0) is shown as follows.

.. figure:: fig/compare_section.png
   :alt: rotate ou
   :align: center 

   Comparsion of schemes.