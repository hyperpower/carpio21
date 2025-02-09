Problem Description
===================

The domain is a unit square :math:`x \in [0.0, 1.0], y \in [0.0, 1.0]`. The boudary conditions are

.. math::
   \phi = 1 &, \quad \text{if} \quad x = 0.0 \quad \text{and} \quad y = [0.0, 0.3]\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 1.0\\
   \phi = 0 &, \quad \text{if} \quad y = 0.0\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0\\

The velocity is :math:`\mathbf{u}=(1, 1)`. 

.. figure:: fig/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.

The :math:`y=x` and :math:`y=x+0.3` is a step which jumps between 1 and 0.

Results
===================

First Order Upwind
-------------------
Initial condition of the domain is :math:`\phi = 0.0`. The iteration stops when :math:`||\phi^{n+1} - \phi^{n}|| < 10^{-5}`. CFL number is 0.4.

.. figure:: fig/fou_iter.gif
   :alt: the gif
   :align: center 

   First Order Upwind.

.. figure:: fig/fou_phi-last.png
   :align: center 

   First Order Upwind (Last Step).

.. figure:: fig/fou_section.png
   :align: center 

   First Order Upwind (x = 0.6).

Center Difference Scheme
-------------------------
.. figure:: fig/CDS_iter.gif
   :alt: the gif
   :align: center 

   Center Difference Scheme.

.. figure:: fig/CDS_section.png
   :align: center 

   CDS (x = 0.6).

QUICK
-------------------------
.. figure:: fig/QUICK_iter.gif
   :alt: the gif
   :align: center 

   QUICK Scheme.

.. figure:: fig/QUICK_section.png
   :align: center 

   QUICK (x = 0.6).

Compare
-------------------------
.. figure:: fig/section_compare.png
   :align: center 

   Comparison all schemes (x = 0.6).