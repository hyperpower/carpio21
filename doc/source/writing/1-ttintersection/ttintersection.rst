Triangle intersection method
==============================

基础向量运算
----------------------

向量的定义
+++++++++++++++++

向量 :math:`\mathbf{a}` 属于 :math:`\mathbf{R^3}`

.. math::
   :label: vector_define

   \mathbf{a} = (a_0, a_1, a_2)

向量的叉乘
+++++++++++++++++
两个向量 :math:`\mathbf{a}` 和 :math:`\mathbf{b}` 的叉乘在三维空间表示为 :math:`\mathbf{a} \times \mathbf{b}`.

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


三角形定义
++++++++++++++++++++
三角形通常由三个点组成, :math:`T=(P_0, P_1, P_2)`. 三角形的法向定义为： :math:`\mathbf{n}= \mathbf{v_1} \times \mathbf{v_2}`, 其中，:math:`\mathbf{v_1} = (P_1 - P_0)`, :math:`\mathbf{v_2} = (P_2 - P_0)`.


问题描述
----------------------
已知两个三角形, :math:`U=(P_0, P_1, P_2)`, :math:`V=(Q_0, Q_1, Q_2)`. 

- 三角形 :math:`U` 是否与 :math:`V` 相交?
- 如果他们相交，结果是什么?

.. jupyter-execute:: ./fig1_tt_ply.py
   :hide-code:

上图为三维坐标系下的两个三角形。

方法
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

以 :math:`P_0` 为原点三角形 :math:`U` 作为基准三角形。可以计算出三角形 :math:`V` 的三个顶点的相对位置。

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

============  ===========  =========== ============
:math:`D_0`   :math:`D_1`  :math:`D_2`    Case
============  ===========  =========== ============
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
============  ===========  =========== ============





