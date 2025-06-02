Triangle intersection method
==============================

Basic Vector Operations
----------------------

Definition of a Vector
+++++++++++++++++++++

A vector :math:`\mathbf{a}` belongs to :math:`\mathbf{R^3}`

.. math::
   :label: vector_define

   \mathbf{a} = (a_0, a_1, a_2)

Cross Product of Vectors
++++++++++++++++++++++++
The cross product of two vectors :math:`\mathbf{a}` and :math:`\mathbf{b}` 
in three-dimensional space is denoted as :math:`\mathbf{a} \times \mathbf{b}`.

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


Definition of a Triangle
++++++++++++++++++++++++
A triangle is usually defined by three points, :math:`T=(P_0, P_1, P_2)`. The normal of the triangle is defined as: :math:`\mathbf{n}= \mathbf{v_1} \times \mathbf{v_2}`, where :math:`\mathbf{v_1} = (P_1 - P_0)`, :math:`\mathbf{v_2} = (P_2 - P_0)`.


Problem Description
----------------------
Given two triangles, :math:`U=(P_0, P_1, P_2)`, :math:`V=(Q_0, Q_1, Q_2)`. 

- Do triangles :math:`U` and :math:`V` intersect?
- If they intersect, what is the result?
   
.. raw:: html
   :file: fig1_ttdefine_ply.div

.. figure:: 0.svg
   :align: center

   Triangles Definition

The plane containing triangle :math:`U` is defined as :math:`\pi_1`, 
and the plane containing triangle :math:`V` is defined as :math:`\pi_2`。

Method
----------------------

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
