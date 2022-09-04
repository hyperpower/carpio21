
Problem Description
===================

B-spline or basis spline is a spline function that has minimal support with respect to a given degree, smoothness, and domain partition. 

The term "B-spline" was coined by Isaac Jacob Schoenberg and is short for basis spline. A spline function of order :math:`n` is a piecewise polynomial function of degree :math:`n-1` in a variable :math:`x`.
The places where the pieces meet are known as **knots**. The key property of spline functions is that they and their derivatives may be continuous, depending on the multiplicities of the knots.

B-splines of order :math:`n` are basis functions for spline functions of the same order defined over the same knots, meaning that all possible spline functions can be built from a linear combination of B-splines, and there is only one unique combination for each spline function.


Definition
===================

Basis Function
-------------------

B-splines can be defined by construction by the Cox–de Boor recursion formula. 

.. math::
    :label: eq_n0

    \displaystyle N_{i,0}(x):=
    {
    \begin{cases}
    1 & {\text{if }}t_{i} \leq x<t_{i+1},\\
    0 & {\text{otherwise}}.
    \end{cases}
    }

.. math::
    :label: eq_nk

    N_{i,k}(x):=
    \frac{t-t_i}{t_{i+k} - t_i} N_{i,k-1}(t) + \frac{t_{i+k+1}-t}{t_{i+k+1} - t_{i+1}}N_{i+1,k-1}(t)

1 order (0 degree) basis function
----------------------------------

When :math:`k=0`, :eq:`eq_n0` is the basis funtion. 

2 order (1 degree) basis function
----------------------------------

When :math:`k=1`, two 1 order basis functions, :math:`N_{i+1,0}(t)` and :math:`N_{i,0}(t)`, are substituted in :eq:`eq_nk`.









