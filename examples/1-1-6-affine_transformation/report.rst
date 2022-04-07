Problem Description
===================

We want to performe Affine Transformation.

An affine transformation is any transformation that preserves collinearity (i.e., all points lying on a line initially still lie on a line after transformation) and ratios of distances (e.g., the midpoint of a line segment remains the midpoint after transformation). In this sense, following functions are considered as affine transformations:

- Translate
- Scale
- Rotate
- Shear Matrix
- Reflect

Any transformation can be defined in a single affine transformation matrix :math:`\mathbf{M}` (3 by 3 in 2D, 4 by 4 in 3D) dhfei.

.. math::
   \mathbf{M}_{2D} = 
   \begin{pmatrix}
     a_0 & a_1 & b_0 \\ 
     a_3 & a_4 & b_1 \\
     0   & 0   & 1
   \end{pmatrix} \quad \quad
   \mathbf{M}_{3D} = 
   \begin{pmatrix}
     a_0 & a_1 & a_3 & b_0 \\ 
     a_4 & a_5 & a_6 & b_1 \\
     a_7 & a_8 & a_9 & b_2 \\
     0   & 0   & 0   & 1
   \end{pmatrix}

Such a matrix :math:`\mathbf{M}` corresponds to a affine transformation **T()** that transforms point (or vector) :math:`\mathbf{x}` to point (or vector) :math:`\mathbf{y}`

Translate
===================

A translation matrix simply moves an object along one or more of the three axes. A transformation matrix representing only translations has the simple form:

.. math::
   \mathbf{M}_{2D} = 
   \begin{pmatrix}
     1 & 0 & t_x \\ 
     0 & 1 & t_y \\
     0   & 0   & 1
   \end{pmatrix} \quad \quad
   \mathbf{M}_{3D} = 
   \begin{pmatrix}
     1 & 0 & 0 & t_x \\ 
     0 & 1 & 0 & t_y \\
     0 & 0 & 1 & t_z \\
     0 & 0 & 0 & 1
   \end{pmatrix}

The box (dashed line) locates at (0,0) to (1,1). It can be translated by a vector :math:`\mathbf{t} = (t_x, t_y)` .

.. figure:: fig/affine_trans.png
   :align: center 

   Translate a box.





Scale
===================
A scaling transform changes the size of an object by expanding or contracting all voxels or vertices along the three axes by three scalar values specified in the matrix. 
A scaling matrix has the following form:

.. math::
   \mathbf{S}_{2D} = 
   \begin{pmatrix}
     s_x & 0 & 0 \\ 
     0 & s_y & 0 \\
     0   & 0 & 1
   \end{pmatrix} \quad \quad
   \mathbf{S}_{3D} = 
   \begin{pmatrix}
     s_x & 0 & 0 & 0 \\ 
     0 & s_y & 0 & 0 \\
     0 & 0 & s_z & 0 \\
     0 & 0 & 0   & 1
   \end{pmatrix}


.. figure:: fig/affine_scale.png
   :align: center 

   Scale a box.

A scaling about a piont is a combination of transform and scaling operations. 

.. figure:: fig/affine_scale_about.png
   :align: center 

   Scale a box about a point.

.. raw:: html
   :file: fig/scale_3d.div

.. figure:: fig/0.svg
   :align: center

   Scale a shape in 3D


Rotate
===================
A rotation matrix rotates an object about one of the three coordinate axes, or any arbitrary vector.

.. math::
   \mathbf{R}_{2D} = 
   \begin{pmatrix}
     \text{cos}(\theta) & -\text{sin}(\theta) & 0 \\ 
     \text{sin}(\theta) &  \text{cos}(\theta) & 0 \\
     0   & 0 & 1
   \end{pmatrix}

.. math::
   \mathbf{R}_{x} = 
   \begin{pmatrix}
     1 & 0 & 0 & 0 \\
     0 & \text{cos}(\theta) &  \text{sin}(\theta)  & 0 \\ 
     0 &-\text{sin}(\theta) &  \text{cos}(\theta)  & 0 \\
     0 & 0 & 0 & 1
   \end{pmatrix}

.. math::
   \mathbf{R}_{y} = 
   \begin{pmatrix}
     \text{cos}(\theta) & -\text{sin}(\theta) & 0 & 0 \\ 
     0 & 1 & 0 & 0 \\
     \text{sin}(\theta) &  \text{cos}(\theta) & 1 & 0 \\
     0 & 0 & 0 & 1
   \end{pmatrix}


.. math::
   \mathbf{R}_{z} = 
   \begin{pmatrix}
     \text{cos}(\theta) & -\text{sin}(\theta) & 0 & 0 \\ 
     \text{sin}(\theta) &  \text{cos}(\theta) & 0 & 0 \\
     0 & 0 & 1 & 0 \\
     0 & 0 & 0 & 1
   \end{pmatrix}


.. figure:: fig/affine_rotate.png
   :align: center 

   Rotate a box about origin. (2D)


.. figure:: fig/affine_rotate_about.png
   :align: center 

   Rotate a box about a point. (2D)


Shear
===================

Shear operations "tilt" objects. 

.. math::
   \mathbf{S}_{x} = 
   \begin{pmatrix}
     1 & \text{tan}(\theta) & 0 \\ 
     0 & 1 & 0 \\
     0   & 0 & 1
   \end{pmatrix} \quad
   \mathbf{S}_{y} = 
   \begin{pmatrix}
     1 & 0 & 0 \\ 
     \text{tan}(\theta) & 1 & 0 \\
     0   & 0 & 1
   \end{pmatrix}

.. figure:: fig/affine_shear_X.png
   :align: center 

   Rotate a box in X. (2D)


.. figure:: fig/affine_shear_Y.png
   :align: center 

   Rotate a box in Y. (2D)


.. figure:: fig/affine_shear_about_in_X.png
   :align: center 

   Rotate a box about a point in X. (2D)


.. figure:: fig/affine_shear_about_in_Y.png
   :align: center 

   Rotate a box about a point in Y. (2D)


Reflect
=========================

Reflect objects about origin.


.. math::
   \mathbf{M}_{\text{origin}} = 
   \begin{pmatrix}
    -1 & 0 &  0 & 0 \\ 
     0 & -1 &  0 & 0 \\
     0 &  0 & -1 & 0 \\
     0 &  0 &  0 & 1
   \end{pmatrix}

Reflect objects about an aix.

.. math::
   \mathbf{M}_{x} = 
   \begin{pmatrix}
     1 &  0 &  0 & 0 \\ 
     0 & -1 &  0 & 0 \\
     0 &  0 & -1 & 0 \\
     0 &  0 &  0 & 1
   \end{pmatrix}
   \mathbf{M}_{y} = 
   \begin{pmatrix}
     1 &  0 &  0 & 0 \\ 
     0 & -1 &  0 & 0 \\
     0 &  0 &  1 & 0 \\
     0 &  0 &  0 & 1
   \end{pmatrix}
   \mathbf{M}_{z} = 
   \begin{pmatrix}
     1 &  0 &  0 & 0 \\ 
     0 &  1 &  0 & 0 \\
     0 &  0 & -1 & 0 \\
     0 &  0 &  0 & 1
   \end{pmatrix}


Reflect about point
-------------------------
.. figure:: fig/affine_reflect_about_o.png
   :align: center 

   Reflect a box about origin. (2D)


.. figure:: fig/affine_reflect_about_point.png
   :align: center 

   Reflect a box about a point. (2D)

Reflect about line
-------------------------

Reflect about aix
^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: fig/affine_reflect_about_X.png
   :align: center 

   Reflect a box about x. (2D)


.. figure:: fig/affine_reflect_about_Y.png
   :align: center 

   Reflect a box about y. (2D)

Reflect about a line (2D)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A line :math:`L` defined as:

.. math::
   L: \quad a x + b y + c = 0

A point :math:`\mathbf{P_0} = (x_0, y_0)` is reflected by the line :math:`L`. The new point is :math:`\mathbf{P_n} = (x_n, y_n)`. The middle point :math:`\mathbf{P_m}` of :math:`\mathbf{P_0}` and  :math:`\mathbf{P_1}` can be obtained:

.. math::
   \mathbf{P_m} = (x_m, y_m)=  (\frac{x_n + x_0}{2}, \frac{y_n + y_0}{2})

The middle point is on line, 

.. math::
   a x_m + b y_m + c = 0 \\

   a \frac{x_n + x_0}{2} + b \frac{y_n + y_0}{2} + c = 0 \\

   a (x_n + x_0) + b (y_n + y_0) + 2 c = 0 \\

a new line constructed by :math:`\mathbf{P_0}` and  :math:`\mathbf{P_1}` is normal to  :math:`L`. 

.. math::
   \frac{y_n - y_0}{x_n - x_0} = \frac{b}{a} \\
   a (y_n - y_0) = b(x_n - x_0) \\
   a (y_n - y_0) - b(x_n - x_0) = 0

So,

.. math::
   \begin{equation}
    \begin{cases}
      a (x_n + x_0) + b (y_n + y_0) + 2 c = 0 \\
      a (y_n - y_0) - b (x_n - x_0) = 0\\
    \end{cases}       
   \end{equation}

:math:`\mathbf{P_n} = (x_n, y_n)` can be expressed by :math:`\mathbf{P_0} = (x_0, y_0)`,

.. math::
   (x_n, y_n) = (-\frac{(a^2 -b^2)x_0 + 2aby_0 + 2ac}{a^2 + b^2}, 
                 -\frac{(b^2 -a^2)y_0 + 2abx_0 + 2bc}{a^2 + b^2})

In matrix form,

.. math::
   \mathbf{M}_{L} = \frac{1}{a^2 + b^2}
   \begin{pmatrix}
    b^2 - a^2 & -2ab     & -2ac \\ 
    -2ab      & a^2-b^2  & -2bc \\
     0        &  0       &  1  \\
   \end{pmatrix}

Example:

.. figure:: fig/affine_reflect_about_line.png
   :align: center 

   Reflect a box about a line. (2D)





