Problem Description
===================

The domain is a unit square :math:`x \in [0.0, 1.0], y \in [0.0, 1.0]`. The boudary conditions are

.. math::
   \phi = 1 &, \quad \text{if} \quad x = 0.0\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 1.0\\
   \phi = 0 &, \quad \text{if} \quad y = 0.0\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0\\

The velocity is :math:`\mathbf{u}=(1, 1)`. The final profile will be :math:`\phi = 1.0` on left top of the domain. Meanwhile, the right bottom profile will be :math:`\phi = 0.0`. 

.. figure:: fig/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.

The :math:`y=x` is a step which jumps from 1 to 0.

Results
===================

First Order Upwind
-------------------
Initial condition of the domain is :math:`\phi = 0.0`. The iteration stops when :math:`||\phi^{n+1} - \phi^{n}|| < 10^{-5}`. CFL number is 0.5.

.. figure:: fig/fou_phi.gif
   :alt: the gif
   :align: center 

   First Order Upwind.

.. figure:: fig/fou_phi_last.png
   :align: center 

   First Order Upwind (Last Step).

.. figure:: fig/norm.png
   :align: center 

   Normal of each step.