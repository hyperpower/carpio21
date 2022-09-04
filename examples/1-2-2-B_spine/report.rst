
Problem Description
===================

B-spline or basis spline is a spline function that has minimal support with respect to a given degree, smoothness, and domain partition. 

The term "B-spline" was coined by Isaac Jacob Schoenberg and is short for basis spline. A spline function of order :math:`n` is a piecewise polynomial function of degree :math:`n-1` in a variable :math:`x`.
The places where the pieces meet are known as **knots**. The key property of spline functions is that they and their derivatives may be continuous, depending on the multiplicities of the knots.

B-splines of order :math:`n` are basis functions for spline functions of the same order defined over the same knots, meaning that all possible spline functions can be built from a linear combination of B-splines, and there is only one unique combination for each spline function.


Definition
===================

B-splines can be defined by construction by the Cox–de Boor recursion formula. 


.. math::
     :label: eq_a

     e^i + 1 = 0

Which can be referenced in the text using Eq.:eq:`eq_a` and so on.
Note the blank lines and the back-tics in the reference. 
Also, there may not be a space between :eq: and `eq_a` in the reference!
Further, no back-tics around the eq_a at the :label: line!




