Triangle intersection method
==============================

Basic Vector Operations
--------------------------

Definition of a Vector
++++++++++++++++++++++++

A vector :math:`\vec{a}` belongs to :math:`\mathbf{R^3}`

.. math::
   :label: vector_define

   \vec{a} = (a_0, a_1, a_2)

Cross Product of Vectors
++++++++++++++++++++++++
The cross product of two vectors :math:`\vec{a}` and :math:`\vec{b}` 
in three-dimensional space is denoted as :math:`\vec{a} \times \vec{b}`.

.. math::
   :label: cross_product1

   \vec{a} \times \vec{b} = 
   \begin{vmatrix} 
   \vec{i} & \vec{j} & \vec{k}  \\
   a_{0} & a_{1} & a_{2}  \\
   b_{0} & b_{1} & b_{2}  \\
   \end{vmatrix} 

.. math::
   :label: cross_product2

   \vec{a} \times \vec{b} = 
   \begin{vmatrix} 
   a_{1} & a_{2} \\
   b_{1} & b_{2} \\
   \end{vmatrix} 
   \vec{i}
   -
   \begin{vmatrix} 
   a_{0} & a_{2} \\
   b_{0} & b_{2} \\
   \end{vmatrix}
   \vec{j}
   +
   \begin{vmatrix} 
   a_{0} & a_{1} \\
   b_{0} & b_{1} \\
   \end{vmatrix}
   \vec{j}

.. math::
   :label: cross_product3

   \vec{a} \times \vec{b} = 
   (a_1b_2-b_1a_2)
   \vec{i}
   -
   (a_0b_2-b_0a_2)
   \vec{j}
   +
   (a_0b_1-b_0a_1)
   \vec{j}

Definition of a Plane Equation
++++++++++++++++++++++++++++++++

A plane equation is a mathematical expression describing a plane 
in three-dimensional space. 
A plane consists of all points satisfying a linear equation. 

.. math::
   n_x x + n_y y + n_z z + d = 0

or 

.. math::
   :label: plane_eq

   \vec{n} \cdot \vec{x} + d = 0

where:
  - :math:`n_x, n_y, n_z` are components of the plane's normal vector (:math:`\vec{n}`), 
    which is perpendicular to the plane,
  - :math:`x, y, z` are coordinates of points on the plane,
  - :math:`d` is a constant representing the plane's offset from the origin.



Distance from a Point to a Plane
+++++++++++++++++++++++++++++++++

1. Given a point :math:`P(P_x, P_y, P_z)` and 
   a plane with equation :math:`n_x x + n_y y + n_z z + d = 0`, 
2. The normal vector of the plane is :math:`\vec{n} = (n_x, n_y, n_z)`.
3. Take any point, :math:`Q(Q_x, Q_y, Q_z)` on the plane, 
   satisfying :math:`\vec{n} \cdot Q + d = 0`.
4. The vector :math:`\vec{PQ} = (P_x - Q_x, P_y - Q_y, P_z - Q_z)`.
5. The distance from the point to the plane 
   is the length of the projection of :math:`\vec{PQ}` 
   onto the normal vector :math:`\vec{n}`.
6. Using vector projection and magnitude, 
   the distance (:math:`D`) of a Point to a Plane is derived.

.. math::
   :label: distance_point_plane

   D = \frac{\vec{PQ} \cdot \vec{n} }{ \| \vec{n} \| }

Definition of a Triangle
++++++++++++++++++++++++
A triangle is usually defined by three points, :math:`T=(P_0, P_1, P_2)`. 
The normal of the triangle is defined as: 
:math:`\vec{n}= \vec{v_1} \times \vec{v_2}`, 
where :math:`\vec{v_1} = (P_1 - P_0)`, :math:`\vec{v_2} = (P_2 - P_0)`.


Intersection of Two Triangles
------------------------------
Given two triangles, :math:`U=(P_0, P_1, P_2)`, :math:`V=(Q_0, Q_1, Q_2)`. 

- **Problem 1:** Do triangles :math:`U` and :math:`V` intersect?
- **Problem 2:** If they intersect, what is the result?
   
.. raw:: html
   :file: fig/fig1_tt_define.div

.. figure:: fig/0.svg
   :align: center

   Triangles Definition

The plane containing triangle :math:`U` is defined as :math:`\pi_1`, 
and the plane containing triangle :math:`V` is defined as :math:`\pi_2`。

Method: Moller97
----------------------

.. _step_1_label:

Step 1
+++++++++++++++++++++++++++++ 

Calculate the plane equation of triangle :math:`V`. 

The plane equation (:math:`\pi_2`) of triangle :math:`V` is defined as:

.. math::
   :label: plane_v

   \vec{n_2} \cdot \vec{x} + d_2 = 0

where :math:`\vec{n_2}` is the normal vector of triangle :math:`V`,

.. math::
   :label: n2

   \vec{n_2} = (Q_1 - Q_0) \times (Q_2 - Q_0)

and :math:`d_2` is the distance from the origin to the plane of triangle :math:`V`.

.. math::
   :label: d2

   d_2 = - \vec{n_2} \cdot Q_0


.. raw:: html
   :file: fig/fig2_t1_plane2.div

.. figure:: fig/0.svg
   :align: center

   Plane 2 and Triangle U


Step 2
+++++++++++++++++++++++++++++
Calculate the signed distances from the vertices of triangle :math:`U` 
to the plane (:math:`\pi_2` ). Reject as trivial if all vertices of triangle :math:`U`
are on the same side of the plane :math:`\pi_2`.

The signed distance from vertices :math:`P_i (i=0,1,2)` to the plane :math:`\pi_2`
is inserting the vertices into the plane equation:

.. math::
   :label: d_u

   D_i = \vec{n_2} \cdot P_i + d_2 \quad i=0,1,2

.. raw:: html
   :file: fig/fig3_t1_plane2_distance.div


.. _fig_distance:
.. figure:: fig/0.svg
   :align: center

   Distance from Vertices of Triangle U to Plane 2


- If :math:`D_i > 0`, then :math:`\mathbf{P_i}` is in the positive side of plane 2.

- If :math:`D_i < 0`, then :math:`\mathbf{P_i}` is in the negative side to plane 2.

- If :math:`D_i = 0`, then :math:`\mathbf{P_i}` is coplanar with plane 2.

All possible cases of :math:`D_i` are summarized in the table below:

.. csv-table:: Triangle Intersection Cases
   :header: No., ":math:`D_0`", ":math:`D_1`", ":math:`D_2`","Case", "Count 0", "Count +", "Count -"
   :align: center
   :widths: 3, 3, 3, 3, 8 , 6, 6, 6

   1,$0$,$0$,$0$,Coplanar,3,0,0
   2,－,$0$,$0$,Line Coplanar,2,0,1
   3,＋,$0$,$0$,Line Coplanar,2,1,0
   4,$0$,－,$0$,Line Coplanar,2,0,1
   5,－,－,$0$,Point Coplanar,1,0,2
   6,＋,－,$0$,Point Coplanar Opposite Side,1,1,1
   7,$0$,＋,$0$,Point Coplanar,2,1,0
   8,－,＋,$0$,Point Coplanar Opposite Side,1,1,1
   9,＋,＋,$0$,Point Coplanar,1,2,0
   10,$0$,$0$,－,Line Coplanar,2,0,1
   11,－,$0$,－,Point Coplanar,1,0,2
   12,＋,$0$,－,Point Coplanar Opposite Side,1,1,1
   13,$0$,－,－,Line Coplanar,1,0,2
   14,－,－,－,No Intersection,0,0,3
   15,＋,－,－,Opposite Side,0,1,2
   16,$0$,＋,－,Point Coplanar Opposite Side,1,1,1
   17,－,＋,－,Opposite Side,0,1,2
   18,＋,＋,－,Opposite Side,0,2,1
   19,$0$,$0$,＋,Line Coplanar,2,1,0
   20,－,$0$,＋,Point Coplanar Opposite Side,1,1,1
   21,＋,$0$,＋,Point Coplanar,1,2,0
   22,$0$,－,＋,Point Coplanar Opposite Side,1,1,1
   23,－,－,＋,Opposite Side,0,1,2
   24,＋,－,＋,Opposite Side,0,2,1
   25,$0$,＋,＋,Point Coplanar,1,2,0
   26,－,＋,＋,Opposite Side,0,2,1
   27,＋,＋,＋,No Intersection,0,3,0


No Intersection cases can be found 
by checking :math:`D_0 D_1 > 0` and :math:`D_0 D_2 > 0`.


Step 3 Compute the plane equation of triangle U
++++++++++++++++++++++++++++++++++++++++++++++++

Calculate the plane equation of triangle :math:`U`. 

The plane equation (:math:`\pi_1`) of triangle :math:`U` is defined as:

.. math::
   :label: plane_u

   \vec{n_1} \cdot \vec{x} + d_1 = 0

where :math:`\vec{n_1}` is the normal vector of triangle :math:`U`,

.. math::
   :label: n1

   \vec{n_1} = (P_1 - P_0) \times (P_2 - P_0)

and :math:`d_1` is the distance from the origin to the plane of triangle :math:`U`.
Calculate method is similar to :ref:`Step 1 <step_1_label>`.


Step 4 Compute the signed distances of triangle V to plane 1
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Calculate the signed distances from the vertices of triangle :math:`V`
to the plane (:math:`\pi_1` ). Reject as trivial if all vertices of triangle :math:`V`
are on the same side of the plane :math:`\pi_1`.

The signed distance from vertices :math:`Q_i (i=0,1,2)` to the plane :math:`\pi_1`
is inserting the vertices into the plane equation. Calculate method is similar to **Step 2**.


Step 5 Compute the intersection line
+++++++++++++++++++++++++++++++++++++++
Compute the intersection line and project onto the largest axis.

For **problem 1**. We want to know if the two triangles intersect. The intersection segement does not need to be computed exactly.

The direction vector of intersection line of two plane $\pi_1$ and $\pi_2$ can be found by:

.. math::
   
   \vec{\Gamma} = \vec{n_1} \times  \vec{n_2}

The line, 

.. math::
   
   \vec{L} = \vec{O} + t \vec{\Gamma}

$O$ is some point on the line. We want to compute a scalar position on $L$ that represent
the intersection between $\overline{P_0 P_2}$ and $\vec{L}$, as shown in :numref:`fig_similar_tri_label`.
The intersection point between $\overline{P_0 P_2}$ and $\vec{L}$ is denoted as $P_{02}$. 
For example, $P_0$ and $P_2$ lie on the different side of $\pi_2$. The vertices, $P_i$ ($i=0,2$), are first projected
onto $\pi_2$, then projected onto $\vec{L}$. The projected points are $P_{ji}$ and $P_{jli}$.
Two pairs of similar triangle can be found, which are $\Delta P_0 P_{02} P_{j0}$ and $\Delta P_2 P_{02} P_{j2}$, 
$\Delta P_{j0} P_{02} P_{jl0}$ and $\Delta P_{j2} P_{02} P_{jl2}$.

.. raw:: html
   :file: fig/fig3_t1_plane2_similar.div


.. _fig_similar_tri_label:

.. figure:: fig/0.svg
   :align: center

   Intersection point and similar triangles.

So, we have

.. math::
   :label: similar_tri

   \frac{ \| P_{02} - P_{jl0} \| }{ \| P_{jl2} - P_{jl0} \| } 
   = 
   \frac{ \| P_0 - P_{j0} \| }{ \| P_0 - P_{j0} \| + \| P_2 - P_{j2} \| }

The projection point $P_{ji} (i=0,1,2)$ is on the plane :math:`\pi_2`, so

.. math::
   :label: proj_plane2

   \vec{n_2} \cdot P_{ji} + d_2 = 0 \quad i=0,1,2

Considering the :eq:`d_u`, we have

.. math::
   :label: proj_point

   \vec{n_2} \cdot (P_{0} - P_{j0}) = D_0 \\

   \| n_2 \| \| P_{0} - P_{j0} \| \cos \theta = D_0 \\

   \| n_2 \| \| P_{0} - P_{j0} \| = D_0  \quad (\text{since} \ \theta = 0) \\
   
   \| P_{0} - P_{j0} \| = \frac{D_0}{\| n_2 \| }

Similarly, the right hand side of :eq:`similar_tri` can be calculated as

.. math::
   :label: proj_point2

   \frac{ \| P_0 - P_{j0} \| }{ \| P_0 - P_{j0} \| + \| P_2 - P_{j2} \| }
   =
   \frac{ D_0 }{ D_0 - D_2 }

The left hand side of :eq:`similar_tri` can be calculated by line parameter value $t$, 
the $P_{02}$ can be denoted as $P_{02} = O + t_{02} \Gamma$. So,

.. math::
   :label: line_param

   \frac{ \| P_{02} - P_{jl0} \| }{ \| P_{jl2} - P_{jl0} \| } 
   = 
   \frac{ t_{02} - t_{jl0} }{ t_{jl2} - t_{jl0} }

Combining :eq:`similar_tri` , :eq:`proj_point2` and :eq:`line_param`, we have

.. math::
   :label: t_param

   t_{02} = t_{jl0} + 
   \frac{ D_0 }{ D_0 - D_2 } ( t_{jl2} - t_{jl0} )

Similar calcuations can be done to compute $t_{12}$. $t_{02}$ and $t_{12}$ are the interval on $\vec{\Gamma}$ for triangle U. Interval for triangle V can be computed in the same way. The pesudo code is shown as folows.

.. code-block:: python
   :linenos:

   def intersect_t(t0, t1, t2, D0, D1, D2):
      # D0 and D1 are on different sides 
      isect0=t0+(t1-t0)*D0/(D0-D1)   
      # D0 and D2 are on different sides 
      isect1=t0+(t2-t0)*D0/(D0-D2)   
      return (isect0, isect1)


The actual value of :math:`t` is not important, only the relative order matters. So we can project the intersection line onto the largest axis of :math:`\vec{\Gamma}` to get the scalar value of :math:`t`. $t_{jl0}$, $t_{jl1}$, $t_{jl2}$ can be found by projecting the vertices of triangle U onto the largest axis.

For example, assume that :math:`\vec{\Gamma} = (g_x, g_y, g_z)`. if the x component ($g_x$) is the largest, then $t_{jl0} = P0_x$ $t_{jl1} = P1_x$ and $t_{jl2} = P2_x$.


For **problem 2**, if the two triangles intersect, we want to compute the intersection segment. So the exact intersection point must be computed. Another pairs of similar triangles can be found, which are $\Delta P_0 P_{02} P_{j0}$ and $\Delta P_{j2} P_{02} P_{2}$.

.. math::
   :label: similar2_tri

   \frac{ \| P_{0} - P_{02} \| }{ \| P_{0} - P_{2} \| } 
   = 
   \frac{ \| P_0 - P_{j0} \| }{ \| P_0 - P_{j0} \| + \| P_2 - P_{j2} \| }

Considerting the :eq:`proj_point2`, we have

.. math::
   :label: similar2_tri_2

   \frac{ \| P_{0} - P_{02} \| }{ \| P_{0} - P_{2} \| } 
   = 
   \frac{ D_0 }{ D_0 - D_2 }

following pseudocode shows how to compute the exact intersection points.

.. code-block:: python
   :linenos:

   def intersect_p(p0, p1, p2, D0, D1, D2):
      # D0 and D1 are on different sides 
      Dr1  = D0 / (D0 - D1)
      diff = p1 - p0 
      isect0 = p0 + Dr1 * diff
      # D0 and D2 are on different sides 
      Dr2  = D0 / (D0 - D2)
      diff = p2 - p0 
      isect1 = p0 + Dr2 * diff
      return (isect0, isect1)

Step 6 Compute the intervals for each triangle
++++++++++++++++++++++++++++++++++++++++++++++++
:eq:`t_param` shows the way to compute the intervals. But, the vertices must be carefully chosen according to the sign of $D_i$. $D_0$ and $D_1$ should not be on the same side. So as the $D_0$ and $D_2$. The following code shows how to order the Vertices.

.. code-block:: python
   :linenos:

   if D0*D1 > 0.:      #condition 1                                
      # D0, D1 are on the same side, 
      # D2 on the other side or on the plane. 
      intersect_t(t2,t0,t1,D2,D0,D1)
   elif D0*D2 > 0:     #condition 2
      # Here we know that D0D1<=0.0                                  
      # D0, D2 are on the same side, 
      # D1 on the other or on the plane  
      intersect_t(t1,t0,t2,D1,D0,D2)
   elif D1*D2 > 0.0 or D0 != 0.0:  #condition 3                       
      # here we know that D0D1<=0.0 or that D0!=0.0         
      intersect_t(t0,t1,t2,D0,D1,D2)
   elif D1!=0.0:       #condition 4                              
      intersect_t(t1,t0,t2,D1,D0,D2)
   elif D2!=0.0:       #condition 5                              
      intersect_t(t2,t0,t1,D2,D0,D1)
   else                #condition 6                              
      # triangles are coplanar                        
      return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2)

All the possible conditions are covered in the following table:

.. csv-table:: Conditions Define
   :header: No., Condition, Description
   :align: center
   :widths: 3, 15, 30

   0, $D_0 D_1 > 0$ and $D_0 D_2 > 0$, No Intersection
   1, $D_0 D_1 > 0$, "$D_0$ and $D_1$ are on the same side. $D_0 D_2 \leq 0$"
   2, $D_0 D_2 > 0$, "$D_0$ and $D_2$ are on the same side. $D_0 D_1 \leq 0$"
   3, $D_1 D_2 > 0$ or $D_0 \neq 0$, 
   4, $D_1 \neq 0$, 
   5, $D_2 \neq 0$, 
   6, , Coplanar


.. csv-table:: $D_i$ of a Triangle to the Plane of another Triangle
   :header: $D_0$, $D_1$, $D_2$,$D_0 D_1$, $D_0 D_2$, $D_1 D_2$, "0","1","2", "3", "4", "5","6"
   :align: center
   :widths: 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2

   0,0,0,0,0,0,,,,,,,1
   －,0,0,0,0,0,,,,1,,,
   ＋,0,0,0,0,0,,,,1,,,
   0,－,0,0,0,0,,,,,1,,
   －,－,0,＋,0,0,,1,,,,,
   ＋,－,0,－,0,0,,,,1,,,
   0,＋,0,0,0,0,,,,,1,,
   －,＋,0,－,0,0,,,,1,,,
   ＋,＋,0,＋,0,0,,1,,,,,
   0,0,－,0,0,0,,,,,,1,
   －,0,－,0,＋,0,,,1,,,,
   ＋,0,－,0,－,0,,,,,,,
   0,－,－,0,0,＋,,,,1,,,
   －,－,－,＋,＋,＋,1,,,,,,
   ＋,－,－,－,－,＋,,,,1,,,
   0,＋,－,0,0,－,,,,,1,,
   －,＋,－,－,＋,－,,,1,,,,
   ＋,＋,－,＋,－,－,,1,,,,,
   0,0,＋,0,0,0,,,,,,1,
   －,0,＋,0,－,0,,,,1,,,
   ＋,0,＋,0,＋,0,,,1,,,,
   0,－,＋,0,0,－,,,,,1,,
   －,－,＋,＋,－,－,,1,,,,,
   ＋,－,＋,－,＋,－,,,1,,,,
   0,＋,＋,0,0,＋,,,,1,,,
   －,＋,＋,－,－,＋,,,,1,,,
   ＋,＋,＋,＋,＋,＋,1,,,,,,
   ,,,,,,2,4,4,9,4,2,1

Step 7 Intersect the intervals
+++++++++++++++++++++++++++++++++++++++

The intervals for each triangle can be computed in **Step 6**. Triangle :math:`U` has interval :math:`[t_{u0}, t_{u1}]` and triangle :math:`V` has interval :math:`[t_{v0}, t_{v1}]`.

If the two intervals overlap, then the two triangles intersect. The overlapping interval is the intersection segment of the two triangles.

For problem 1, pseudocode is shown as follows:

.. code-block:: python
   :linenos:

   if (t_u1 < t_v0) or (t_v1 < t_u0):
      return False   # no intersection
   else:
      return True    # intersection

For problem 2, $t$ used to compute the order of intersection points.


Coplanar Triangle Intersection
----------------------------------

When two triangles are coplanar, special handling is required to determine their intersection.


Projection
++++++++++++++++++++++++++++++++

Two triangles share the same plane. $N$ is the normal of the plane. The triangles can be projected onto one of the three coordinate planes (XY, YZ, ZX) based on the component of $N$ with the largest absolute value.

.. code-block:: python
   :linenos:

   abs_n = [abs(N.x), abs(N.y), abs(N.z)]
   if abs_n[0] > abs_n[1]:
      if abs_n[0] > abs_n[2]: # X is largest
         # project onto YZ plane
         return (1,2) #index of Y and Z
      else: # Z is largest
         # project onto XY plane
         return (0,1) #index of X and Y
   else:
      if abs_n[1] > abs_n[2]: # Y is largest
         # project onto XZ plane
         return (0,2) #index of X and Z
      else:
         # project onto XY plane
         return (0,1) #index of X and Y

Edge Intersection Test
+++++++++++++++++++++++++++++++

After projection, the 2D coordinates of the triangles' vertices are used to check for edge intersections. Each edge of triangle U is tested against each edge of triangle V for intersection.

For example, one edge of triangle U is defined by points :math:`P_0` and :math:`P_1`, and one edge of triangle V is defined by points :math:`Q_0` and :math:`Q_1`.  

- Edge of Triangle U: **$P_0 P_1$**
- Edge of Triangle V: **$Q_0 Q_1$**

They can be written in parametric form as

.. math::

   \begin{aligned}
   P(t) &= P_0 + t(P_1 - P_0), \quad t \in [0,1] \\
   Q(u) &= Q_0 + u(Q_1 - Q_0), \quad u \in [0,1]
   \end{aligned}

The two segments intersect if and only if there exist parameters
:math:`t, u \in [0,1]` such that

.. math::

   P_0 + t(P_1 - P_0) = Q_0 + u(Q_1 - Q_0)

Rearranging the equation gives

.. math::

   t(P_1 - P_0) - u(Q_1 - Q_0) = Q_0 - P_0


Introduce the following vector notation:

.. math::

   \mathbf{A} = P_1 - P_0, \quad
   \mathbf{B} = Q_1 - Q_0, \quad
   \mathbf{C} = P_0 - Q_0

Then the equation becomes

.. math::

   t\mathbf{A} - u\mathbf{B} = -\mathbf{C}


To eliminate one parameter, take the cross product of both sides
with :math:`\mathbf{B}`:

.. math::

   (t\mathbf{A} - u\mathbf{B}) \times \mathbf{B}
   = (-\mathbf{C}) \times \mathbf{B} \\
   t\mathbf{A} \times \mathbf{B} - u\mathbf{B} \times \mathbf{B}
   = (-\mathbf{C}) \times \mathbf{B}  = \mathbf{B} \times \mathbf{C}

Using :math:`\mathbf{B} \times \mathbf{B} = 0`, this simplifies to

.. math::

   t(\mathbf{A} \times \mathbf{B}) = \mathbf{B} \times \mathbf{C}

Thus,

.. math::

   t = \frac{\mathbf{B} \times \mathbf{C}}{\mathbf{A} \times \mathbf{B}}

Similarly, taking the cross product with :math:`\mathbf{A}` gives

.. math::

   u = \frac{\mathbf{A} \times \mathbf{C}}{\mathbf{A} \times \mathbf{B}}

For the intersection point to lie on both segments, the parameters
must satisfy

.. math::

   0 \le t \le 1, \quad 0 \le u \le 1

Substituting the expressions for :math:`t` and :math:`u` yields

.. math::

   0 \le \frac{\mathbf{B} \times \mathbf{C}}{\mathbf{A} \times \mathbf{B}} \le 1,
   \quad
   0 \le \frac{\mathbf{A} \times \mathbf{C}}{\mathbf{A} \times \mathbf{B}} \le 1

Let

.. math::

   f = \mathbf{A} \times \mathbf{B}, \quad
   d = \mathbf{B} \times \mathbf{C}, \quad
   e = \mathbf{A} \times \mathbf{C}

Then the interval conditions can be evaluated *without division* by
considering the sign of :math:`f`.

- Case 1: :math:`f > 0`

.. math::

   0 \le d \le f
   \quad \text{and} \quad
   0 \le e \le f

- Case 2: :math:`f < 0`

.. math::

   0 \ge d \ge f
   \quad \text{and} \quad
   0 \ge e \ge f

These inequalities are algebraically equivalent to the original
constraints :math:`t, u \in [0,1]`, but avoid any division operations.

Point in Triangle Test
+++++++++++++++++++++++++++++++

If no edge intersections are found, a final test is performed to check if one triangle is
entirely contained within the other. This is done using a point-in-triangle test.







将三角形 :math:`U` 的 :math:`P_0` 点作为原点
++++++++++++++++++++++++++++++++++++++++++++
对于三角形 :math:`U` ，

.. math::
   :label: minus_u

   \mathbf{e_1} &= P_1 - P_0 \\
   \mathbf{e_2} &= P_2 - P_0

对于三角形 :math:`V` ，

.. math::
   :label: minus_v

   \mathbf{r_0} &= Q_0 - P_0 \\
   \mathbf{r_1} &= Q_1 - P_0 \\
   \mathbf{r_2} &= Q_2 - P_0


计算三角形的相对位置
++++++++++++++++++++++

以 :math:`P_0` 为原点三角形 :math:`U` 作为基准三角形。
可以计算出三角形 :math:`V` 的三个顶点的相对位置。

.. math::

   \mathbf{D_0} &= \mathbf{r_0} \cdot (\mathbf{e_1} \times \mathbf{e_2}) \\
   \mathbf{D_1} &= \mathbf{r_1} \cdot (\mathbf{e_1} \times \mathbf{e_2}) \\
   \mathbf{D_2} &= \mathbf{r_2} \cdot (\mathbf{e_1} \times \mathbf{e_2})

即：

.. math::
   :label: d_vg

   D_i = \mathbf{r_i} \cdot (\mathbf{e_1} \times \mathbf{e_2})    
                = \mathbf{r_i} \cdot \mathbf{n_0} \quad i=0,1,2

如果 :math:`D_i>0` 则 :math:`\mathbf{r_i}` 与 :math:`\mathbf{n_0}` 的同方向。

如果 :math:`D_i<0` 则 :math:`\mathbf{r_i}` 与 :math:`\mathbf{n_0}` 的反方向。

如果 :math:`D_i=0` 则 :math:`\mathbf{r_i}` 与三角形 :math:`V` 的共面（coplane）。


根据 :math:`D_i` 判断类型
+++++++++++++++++++++++++

.. table::
   :widths: 20 20 20 30

   ============  ===========  =========== ==================
   :math:`D_2`   :math:`D_1`  :math:`D_0`    Case
   ============  ===========  =========== ==================
   0             0            0           全共面
   0             0            －           线共面
   0             0            ＋           线共面
   0             －            0           线共面
   0             －            －           点共面
   0             －            ＋           点共面异侧
   0             ＋            0           点共面
   0             ＋            －           点共面异侧
   0             ＋            ＋           点共面
   －             0            0           线共面
   －             0            －           点共面
   －             0            ＋           点共面异侧
   －             －            0           线共面
   －             －            －           不相交
   －             －            ＋           异侧
   －             ＋            0           点共面异侧
   －             ＋            －           异侧
   －             ＋            ＋           异侧
   ＋             0            0           线共面
   ＋             0            －           点共面异侧
   ＋             0            ＋           点共面
   ＋             －            0           点共面异侧
   ＋             －            －           异侧
   ＋             －            ＋           异侧
   ＋             ＋            0           点共面
   ＋             ＋            －           异侧
   ＋             ＋            ＋           不相交
   ============  ===========  =========== ==================

通过对 :math:`D_i` 类型的判断，可以得到6种类型：

- 不相交
- 全共面
- 线共面
- 点共面
- 点共面异侧
- 异侧

三角形与平面得到交点
+++++++++++++++++++++++++

以三角形 :math:`U` 作为基准三角形，则线共面、点共面异侧和异侧这三种情况下，三角形 :math:`V` 在三角形 :math:`U` 所在的平面有两个交点，形成一个线段。

一般的，这两交点可以记做 :math:`\mathbf{t_0}` 和 :math:`\mathbf{t_1}`

:math:`\mathbf{t_0}` 在 :math:`\mathbf{r_0}` 和 :math:`\mathbf{r_1}` 的连线上， 所以

.. math::
   :label: t0

   \mathbf{t_0} = \beta \mathbf{r_0} + (1 - \beta) \mathbf{r_1}


同时，:math:`\mathbf{t_0}` 与 :math:`\mathbf{e_1} \times \mathbf{e_2}` 垂直， 所以

.. math::
   :label: t0_n

   \mathbf{t_0} \cdot (\mathbf{e_1} \times \mathbf{e_2}) = 0

所以，

.. math::
   :label: t0n0

   (\beta \mathbf{r_0} + (1 - \beta) \mathbf{r_1}) \cdot (\mathbf{e_1} \times \mathbf{e_2}) & = 0 \\
   (\beta \mathbf{r_0} + (1 - \beta) \mathbf{r_1}) \cdot \mathbf{n_0} & = 0 \\
   (\beta \mathbf{r_0} \cdot \mathbf{n_0} + (1 - \beta) \mathbf{r_1} \cdot \mathbf{n_0} & = 0 \\
   \beta D_0 + (1 - \beta) D_1 & = 0 \\
   \beta D_0 + D_1 - \beta D_1 & = 0  \\
   \beta (D_1 - D_0) & = D_0  \\
   \beta & = \frac{D_0}{D_1 - D_0} 

同理，可得 :math:`\mathbf{t_1}`


交点与基三角形的关系
+++++++++++++++++++++++++

对于 :math:`\mathbf{t_0}` 或 :math:`\mathbf{t_1}`，与 :math:`\mathbf{e_0}` 和 :math:`\mathbf{e_1}` 叉乘关系可以表示其交点的相对位置。

以 :math:`\mathbf{t_0}` 为例，:math:`(\mathbf{t_0} \times \mathbf{e_0}) \cdot \mathbf{n_0}` 可能大于，等于和小于0，三种情况。
:math:`(\mathbf{t_0} \times \mathbf{e_1}) \cdot \mathbf{n_0}` 可能大于，等于和小于0，也是三种情况。组合有9种情况，分别表示为

.. table:: 交点与基三角形两边的关系
   :widths: 20 20 30

   ==================================== ===================================== =======
   :math:`(\mathbf{t_0}, \mathbf{e_0})` :math:`(\mathbf{t_0}, \mathbf{e_1})`   序号
   ==================================== ===================================== =======
   0                                     0                                     0
   －                                    0                                     1
   －                                    －                                    2
   0                                     －                                    3
   ＋                                    －                                    4
   ＋                                     0                                    5
   ＋                                    ＋                                    6
   0                                     ＋                                    7
   －                                    ＋                                    8
   ==================================== ===================================== =======


.. figure:: ./fig2_tt.png

    交点位置的序号.


交线与基三角形的关系
+++++++++++++++++++++++++

对于 :math:`\mathbf{t_0}` 或 :math:`\mathbf{t_1}`，与 :math:`\mathbf{e_0}` 和 :math:`\mathbf{e_1}` 叉乘关系可以表示其交点的相对位置。 两个交点组成一个线段，线段存在81种可能的组合。


.. table:: 
   :widths: 10 10 10 10 10 10 10 10 10 10

   ====== ====== ====== ====== ===== ===== ====== ======== ===== =====
    交点                             t1
   ------ ------------------------------------------------------------
     t0     0      1      2      3     4     5      6      7     8
   ====== ====== ====== ====== ===== ===== ====== ======== ===== =====
    0     0      a1     a1     a1    4     4      4        4     4
    1            a0a1   a0a1   a0a1  a0a1  a0     a0       a0    a0      
    2                   a0a1   a0a1  a0a1  a0b1   a0c0b0b1 a0b0  a0b0
    3                          a0a1  a0    a0     a0       a0    a0b0
    4                                no    no     no       no    a0a1                 
    5                                      no     no       no    no
    6                                             no       no    no
    7                                                      no    no
    8                                                            no
   ====== ====== ====== ====== ===== ===== ====== ======== ===== =====


a0=C(e0->t0, e0->e1)
a1=C(e0->t1, e0->e1)

b0=C(t0->e0, t0->t1)
b1=C(t0->e1, t0->t1)

c0=C(t0->0, t0->t1)




0 不可能发生


1 共线，叉乘 :math:`(\mathbf{t_1} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})` 


- 如果为+，返回 线接触，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{e_0}`
- 如果为-，返回 线接触，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{t_1}`
- 如果为0，返回 线接触，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{t_1}`


2 叉乘， :math:`(\mathbf{t_1} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`


- 如果为+，返回 相交，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{c_0}`，需要新算。
- 如果为-，返回 相交，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{t_1}`
- 如果为0，返回 点接触，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{t_1}`


3 共线，叉乘， :math:`(\mathbf{t_1} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`


- 如果为+，返回 相交，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{e_1}`
- 如果为-，返回 相交，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{t_1}`
- 如果为0，返回 点接触，2点，:math:`\mathbf{t_0}`，:math:`\mathbf{t_1}`


4 点接触，返回 点接触，1点，:math:`\mathbf{t_0}`


5 1-1 共线，叉乘 :math:`(\mathbf{t_1} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`； 叉乘 :math:`(\mathbf{t_0} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`


- 新计算交点 t0->t1 , e0->e1


6 叉乘 :math:`(\mathbf{t_1} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`； 叉乘 :math:`(\mathbf{t_0} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`


- 新计算交点 t0->t1 与 e0->e1
- 新计算交点 t0->t1 与 e1


7 共线，叉乘， :math:`(\mathbf{t_1} - \mathbf{e_0})` 和 :math:`(\mathbf{e_1} - \mathbf{e_0})`


8 C(e0->t1,e0->e1)，C(e0->t2,e0->e1)

9 C(e0->t0,e0->e1)，C(t0->e1,t0->t1)

10 C(e0->t0,e0->e1)，C(t0->e1,t0->t1)，C(t0->e0,t0->t1)

11 C(e0->t0,e0->e1)，C(t0->e0,t0->t1)








定义 ED(t) = :math:`(\mathbf{e_0} - \mathbf{t}) \times (\mathbf{e_1} - \mathbf{t})` 与 :math:`\mathbf{e_0} \times \mathbf{e_1}` 同向

b :math:`(\mathbf{e_0} - \mathbf{t_1}) \times (\mathbf{e_1} - \mathbf{t_1})` 与  :math:`\mathbf{e_0} \times \mathbf{e_1}`
同向则相交，共线，两点


c 点点接触，:math:`\mathbf{t_0}` 

b11 ED(t0) ED(t1)

e12-4 ED(t0)

b15 ED(t0)

f16 
 ED(t0) ED(t1) 
 +      +      intersect t0->cal0
 +      -      intersect t0->cal2
 +      0      intersect t0->cal0
 -      +      intersect cal2->cal0
 -      -      no
 -      0      no
 0      +      intersect t0->cal0
 0      -      pointouch t0
 0      0      linetouch e1->e0

 f17 
 ED(t0) ED(t1) 
 +      +      intersect t0->t1
 +      -      intersect t0->cal2
 +      0      intersect t0->t1
 -      +      intersect cal2->t1
 -      -      no
 -      0      pointouch t1
 0      +      intersect t0->t1
 0      -      pointouch t0
 0      0      linetouch t0->t1

f26 
 ED(t0) ED(t1) 
 +      +      intersect t0->t1
 +      -      intersect t0->cal2
 +      0      intersect t0->t1
 -      +      intersect cal2->t1
 -      -      no
 -      0      pointouch t1
 0      +      intersect t0->t1
 0      -      pointouch t0
 0      0      linetouch t0->t1






2 - 7
- j1 :math:`\mathbf{t_0}` 或 :math:`\mathbf{t_1}` 在 :math:`\mathbf{e_0}` 或 :math:`\mathbf{e_1}` 上，判断内外。
- j2 :math:`\mathbf{t_0}` 和 :math:`\mathbf{t_1}` 连线是否与 :math:`\mathbf{e_0}` 或 :math:`\mathbf{e_1}` 相交。
- j3 :math:`\mathbf{t_0}` 和 :math:`\mathbf{t_1}` 连线是否与 :math:`\mathbf{e_0}` 和 :math:`\mathbf{e_1}` 连线相交。



- no 不相交
- a 不可能发生；
- b 共线；
- c 有一个点相交，这个点已经知道了；
- d 还需要 点 与:math:`\mathbf{e_0}` :math:`\mathbf{e_1}` 连线的关系
- e 点在线上，还需要 点 与 :math:`\mathbf{e_0}` 或 :math:`\mathbf{e_1}` 的关系 
- f 还需要更多的判断
- g 还需要一个点线判断 和 :math:`\mathbf{e_0}` :math:`\mathbf{e_1}` 连线的关系
