OGrid 实现原理
===============

网格的构成
----------------
Carpio 的八叉树网格由两层结构组成：

1. 根网格（Root Grid）
    这是一个规则排列的数组(1,2,3维数组)，每个根节点对应
    空间中的一个大单元，整个计算区域由这些根节点覆盖。根节点之间的关系是规则的，因此可以通过简单的索引计算来找到相邻的根节点。空间分布和排序的原则根据笛卡尔坐标系进行设计，保证了根节点的空间位置与其在数组中的索引之间有直接的映射关系。

2. 八叉树（Octree）
    每个根节点下都可以有一个八叉树结构，用于在该根节点覆盖的空间内进行自适应细分。八叉树允许在局部区域细分成更小的单元，以提高计算精度，同时在不需要细分的区域保持较粗的单元以节省计算资源。


八叉树（Octree）的基本原理
----------------

八叉树是一种递归空间划分结构。它把一个三维空间单元沿 ``x``、
``y``、``z`` 三个方向各切分一次，因此一个父单元最多生成 8 个子单元。
在二维中，同样思想退化为四叉树：一个父单元生成 4 个子单元。在一维中，
则是二叉划分：一个区间生成 2 个子区间。

在这种结构中，常见概念如下：

``root``
    树的根节点，对应最粗层级的空间单元。对应（Root Grid）中一个节点。

``child``
    某个节点细分后生成的子节点。三维中最多有 8 个，二维中最多有 4 个，
    一维中最多有 2 个。

``leaf``
    没有子节点的节点。自适应网格中的实际计算单元通常是叶节点。

``level``
    节点所在层级。根节点为第 0 层，每细分一次，层级加 1。

``neighbor``
    与当前节点在某个面方向相邻的节点，例如 ``x-``、``x+``、``y-``、
    ``y+`` 等方向。

``Morton code``
    一种把树路径编码为整数的方式。每一层的 child index 会被追加到路径中，
    因而可以用一个整数记录节点从根节点走到当前位置的路径。

八叉树适合表达自适应网格（Adaptive Refinement Mesh）。计算区域不需要整体加密，只需要在局部复杂区域继续细分。这样可以在保持局部精度的同时减少节点数量。


Carpio 中的整体设计
-------------------

本项目把八叉树网格拆成三个核心部分：

``OGrid_``
    根网格容器，定义在 ``ogrid.hpp`` 中。它管理规则排列的 root 节点数组，
    负责根节点索引、ghost layer、根节点邻居查找和根节点遍历。

``ONode_``
    树节点，定义在 ``onode.hpp`` 中。它保存父节点指针、子节点指针、
    面邻居指针、几何单元 ``cell`` 和用户数据 ``data``，并实现节点细分、
    Morton 编码和非根节点的邻居查找。

``OCellUniform_`` / ``OCellNonUniform_``
    几何单元，定义在 ``ocell.hpp`` 中。``OCellUniform_`` 描述各方向等长的
    单元，``OCellNonUniform_`` 描述各方向可以不同长度的单元。

``OGrid_`` 的模板声明如下：

.. code-block:: cpp

   template<typename DATA, typename CELL, St DIM>
   class OGrid_;

其中 ``DATA`` 是每个节点携带的数据类型，``CELL`` 是几何单元类型，
``DIM`` 是维度，支持 1D、2D 和 3D。项目通过 ``if constexpr`` 在编译期
裁剪不同维度下的逻辑。


根网格与 Ghost Layer
--------------------

``OGrid_`` 内部有两个关键成员：

.. code-block:: cpp

   std::array<St, Dim> _len;
   std::vector<Node> _roots;

``_roots`` 是所有 root 节点的一维连续存储。``_len`` 是每个方向的内部存储
长度。构造函数接收用户给出的有效网格数量 ``nx``、``ny``、``nz``，然后在
每个方向加上两层额外空间：

.. code-block:: cpp

   _len[0] = nx + 2;

这是因为 ``OGrid_`` 固定使用一层 ghost layer：

.. code-block:: cpp

   static const St GhostLayer = 1;

因此，每个方向的内部存储包含：

* 左侧或负方向的一层 ghost root；
* 中间的真实计算区 root；
* 右侧或正方向的一层 ghost root。

``size()`` 返回真实计算区的 root 数量，不包含 ghost layer。
``storage_size()`` 返回 ``_roots`` 的真实存储数量，包含 ghost layer。
例如二维网格 ``nx * ny`` 的计算区，在内部实际存储为
``(nx + 2) * (ny + 2)`` 个 root 节点。

构造完成后，``_set_root_indices()`` 会把每个 root 节点在 ``_roots`` 中的
storage index 写入节点自身：

.. code-block:: cpp

   void _set_root_indices() {
       for (St i = 0; i < _roots.size(); ++i) {
           _roots[i].set_root_idx(i);
       }
   }

这个 ``root_idx`` 后续会被邻居查找和 ``IsNodeInGrid`` 使用。


索引映射
--------

``OGrid_`` 同时维护两套索引概念。

逻辑索引
    面向计算区的索引，不包含 ghost 偏移。例如有效 root 的 ``i`` 范围是
    ``[0, size_i())``。

storage 索引
    面向 ``_roots`` 实际存储的索引，包含 ghost layer。逻辑索引会通过
    ``_to_storage_idx()`` 加上 ``GhostLayer``。

``to_1d_idx(i, j, k)`` 把计算区逻辑索引转换成一维编号，不包含 ghost。
``to_1d_storage_idx(i, j, k)`` 先把逻辑索引转换为 storage 索引，再映射到
``_roots`` 的一维数组位置。

.. code-block:: cpp

   St _to_storage_idx(Int idx, St dim) const {
       return St(idx + Int(GhostLayer));
   }

反向转换由 ``to_indices()`` 和 ``storage_1d_idx_to_indices()`` 完成。
前者从计算区的一维编号得到逻辑多维索引，后者从 storage 一维编号得到扣除
ghost 偏移后的逻辑索引。

这种设计让外部访问可以使用自然的 ``0`` 起始计算区索引，同时内部仍然保留
ghost root，方便边界邻居和边界条件处理。


节点细分与 Morton 编码
----------------------

``ONode_`` 是真正表达树结构的类。每个节点保存：

.. code-block:: cpp

   pNode father;
   pNode child[NumChildren];
   pNode neighbor[NumNeighbors];
   Cell cell;
   Data data;

其中 ``NumChildren`` 等于当前维度下的子节点数量。三维中是 8，二维中是 4，
一维中是 2。

创建单个子节点使用 ``new_child(idx)``：

.. code-block:: cpp

   void new_child(St idx) {
       set_child(idx, new Node(idx, this, MakeSubCell(cell, idx), data));
   }

``MakeSubCell(cell, idx)`` 会调用 ``cell.make_sub_cell(idx)``。对于 uniform
cell，子单元半边长是父单元的一半，子单元中心则由 ``idx`` 的 bit 位决定。
例如三维中，``idx`` 的第 0 位表示 ``x`` 方向，第 1 位表示 ``y`` 方向，
第 2 位表示 ``z`` 方向。bit 为 1 时向正方向偏移，bit 为 0 时向负方向偏移。

``new_full_children()`` 会一次性创建所有子节点：

.. code-block:: cpp

   void new_full_children() {
       for (St i = 0; i < NumChildren; ++i) {
           new_child(i);
       }
   }

节点还保存一个 ``MortonCode_``。当子节点被创建或重新挂接到父节点下时，
``_reset_code_from_parent(idx)`` 会把父节点编码复制过来，并追加当前
``idx``。因此，节点的 ``level()`` 来自 Morton code 的层级字段，
``idx()`` 则表示该节点在父节点中的 child index。


邻居查找
--------

``OGrid_`` 和 ``ONode_`` 分别处理不同层级的邻居问题。

root 节点的邻居
~~~~~~~~~~~~~~~

root 节点排列在规则数组中，因此邻居查找可以直接在 storage index 上移动。
``connect_neighbors()`` 会遍历所有 root 节点，并为每个面方向设置
``node.neighbor[n]``：

.. code-block:: cpp

   node.neighbor[n] = _find_face_neighbor(idx, axis, ori);

``_find_face_neighbor()`` 的核心是把当前 storage index 沿指定轴和方向
移动一个位置。如果移动后仍在 ``_len`` 范围内，就返回对应 root；
否则返回 ``nullptr``。

非 root 节点的邻居
~~~~~~~~~~~~~~~~~~

非 root 节点的邻居查找由 ``ONode_::_find_face_neighbor()`` 递归完成。
逻辑可以分为三种情况：

1. 当前节点不是目标方向上的边界子节点。
   这说明邻居在同一个父节点内部，只需要通过 child index 翻转对应方向的
   bit 位即可找到兄弟节点。

2. 当前节点位于目标方向边界。
   这时同一父节点内部没有目标方向的邻居，需要先递归查找父节点在该方向的
   邻居，也就是 coarse neighbor。

3. 找到 coarse neighbor 后。
   如果 coarse neighbor 比当前父节点更粗，则直接返回它。如果它有对应的
   子节点，则返回 fine neighbor；如果没有对应子节点，则返回 coarse
   neighbor。

这种实现允许相邻区域处在不同细化层级。也就是说，一个细节点的面邻居可以是
同层级节点，也可以是更粗的节点。


Uniform 与 NonUniform Grid
--------------------------

``OGridUniform_`` 和 ``OGridNonUniform_`` 都继承自 ``OGrid_``，区别在于 root
cell 的几何生成方式。

``OGridUniform_`` 使用统一的 ``cell_len``。第 ``i`` 个 storage root 在某个
方向的中心坐标为：

.. code-block:: cpp

   origin + (storage_index - GhostLayer + 0.5) * cell_len

因此 ghost root 会自然落在计算区外侧。

``OGridNonUniform_`` 使用每个方向的 cell length 数组。它会在每个方向的
cell size 序列两端各复制一个边界长度，作为 ghost layer 的长度：

.. code-block:: cpp

   _cell_size[dim].push_back(normal.front());
   ...
   _cell_size[dim].push_back(normal.back());

然后通过累加前面所有 cell size 来计算当前 storage root 的中心坐标。

两种 grid 都提供 ``locate_root(p)`` 和 ``locate(p)``：

``locate_root(p)``
    根据点坐标找到所在 root 节点。如果点不在可定位范围内，则返回
    ``nullptr``。

``locate(p)``
    先找到 root，再沿着已有 child 逐层向下查找。如果目标 child 不存在，
    返回当前节点；如果一直存在子节点，则返回最细叶节点。


代码阅读总结
------------

Carpio 的 ``OGrid`` 实现可以理解为“规则 root 网格 + 每个 root 上的自适应树”。
``OGrid_`` 负责全局根网格、ghost layer 和 root 级索引映射；
``ONode_`` 负责局部树结构、节点细分和跨层级邻居查找；
``OCellUniform_`` 与 ``OCellNonUniform_`` 负责几何范围；
``MortonCode_`` 负责记录节点在树中的路径。

这种设计把拓扑结构、几何信息和节点数据分离开来。根层保持规则数组，便于
定位和边界处理；根以下允许自适应细分，便于在局部区域提高分辨率。
