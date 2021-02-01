reStructureText Examples
===========================

This file used for testing reStructureText file

H3 -- Subsection
----------------

H4 -- Subsubsection
+++++++++++++++++++


Talbes
----------------
COMPLEX TABLE:

+------------+------------+-----------+
| Header 1   | Header 2   | Header 3  |
+============+============+===========+
| body row 1 | column 2   | column 3  |
+------------+------------+-----------+
| body row 2 | Cells may span columns.|
+------------+------------+-----------+
| body row 3 | Cells may  | - Cells   |
+------------+ span rows. | - contain |
| body row 4 |            | - blocks. |
+------------+------------+-----------+

SIMPLE TABLE:

=====  =====  ======
   Inputs     Output
------------  ------
  A      B    A or B
=====  =====  ======
False  False  False
True   False  True
False  True   True
True   True   True
=====  =====  ======

Subject Subtitle
----------------
Subtitles are set with '-' and are required to have the same length 
of the subtitle itself, just like titles.
 
Lists can be unnumbered like:
 
 * Item Foo
 * Item Bar
 
Or automatically numbered:
 
 #. Item 1
 #. Item 2

Inline Markup
-------------
Words can have *emphasis in italics* or be **bold** and you can define
code samples with back quotes, like when you talk about a command: ``sudo`` 
gives you super user powers!

Math
-------------

here is a inline math. Since Pythagoras, we know that :math:`a^2 + b^2 = c^2`.


.. math::
   :label: math1

   (a + b)^2 = a^2 + 2ab + b^2

   (a - b)^2 = a^2 - 2ab + b^2

here, we can find :eq:`math1`

Paragraph Markup
--------------------
This is a statement.

.. warning::

   Never, ever, use this code!

.. versionadded:: 0.0.1

It's okay to use this code.

Code
---------------------
Here is something I want to talk about::

    def my_fn(foo, bar=True):
        """A really useful function.

        Returns None
        """


Embed Plotly
---------------------------
The Plotly code show as following

.. raw:: html
	
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
    <div id="33366951-e206-465b-afba-31a4cff7492f" style="height: 100%; width: 100%;" class="plotly-graph-div"></div><script type="text/javascript">window.PLOTLYENV=window.PLOTLYENV || {};window.PLOTLYENV.BASE_URL="https://plot.ly";Plotly.newPlot("33366951-e206-465b-afba-31a4cff7492f", [{"mode": "lines", "x": [0.0, 0.9297764882555776, 0.9685831621948102, 0.0, null, 0.0, 0.72896856285627, 0.6374239187270574, 0.0, null, 0.0, 0.8090170006748053, 0.8763066841745963, 0.0, null, 0.0, 0.535826718963432, 0.42577921204230046, 0.0, null, 0.0, 0.8763066841745963, 0.9297764882555776, 0.0, null, 0.0, 0.6374239187270574, 0.535826718963432, 0.0, null, 0.0, -0.929776468527858, -0.8763066583575159, 0.0, null, 0.0, 0.8090169376762217, 0.72896856285627, 0.0, null, 0.0, 0.8763066325404327, 0.8090169376762217, 0.0, null, 0.0, 0.5358268094581513, 0.6374240013103503, 0.0, null, 0.0, 0.9685831355403259, 0.9297764488001358, 0.0, null, 0.0, 0.9297764488001358, 0.8763066325404327, 0.0, null, 0.0, -0.9921147082997216, -0.9999999999999986, 0.0, null, 0.0, -0.9685831488675696, -0.929776468527858, 0.0, null, 0.0, -0.9921146948665578, -0.9685831488675696, 0.0, null, 0.0, 0.9921146881499716, 0.9685831355403259, 0.0, null, 0.0, 1.0, 0.9921146881499716, 0.0, null, 0.0, 0.7289686362257485, 0.8090170006748053, 0.0, null, 0.0, -0.9999999999999986, -0.9921146948665578, 0.0, null, 0.0, 0.6374240013103503, 0.7289686362257485, 0.0, null, 0.0, 0.4257793090212906, 0.5358268094581513, 0.0, null, 0.0, -0.9685831755220482, -0.9921147082997216, 0.0, null, 0.0, -0.9297765079832945, -0.9685831755220482, 0.0, null, 0.0, 0.309017014761716, 0.4257793090212906, 0.0, null, 0.0, -0.8763067099916744, -0.9297765079832945, 0.0, null, 0.0, 0.18738133774757554, 0.309017014761716, 0.0, null, 0.0, 0.06279054520165538, 0.18738133774757554, 0.0, null, 0.0, -0.8090170321740936, -0.8763067099916744, 0.0, null, 0.0, -0.7289686729104845, -0.8090170321740936, 0.0, null, 0.0, -0.06279049171760964, 0.06279054520165538, 0.0, null, 0.0, -0.5358268547055087, -0.6374240426019939, 0.0, null, 0.0, -0.18738128510700514, -0.06279049171760964, 0.0, null, 0.0, -0.6374240426019939, -0.7289686729104845, 0.0, null, 0.0, -0.3090169637947943, -0.18738128510700514, 0.0, null, 0.0, -0.42577935751078394, -0.5358268547055087, 0.0, null, 0.0, -0.3090170657286372, -0.42577935751078394, 0.0, null, 0.0, -0.42577926053179604, -0.3090169637947943, 0.0, null, 0.0, -0.18738139038814564, -0.3090170657286372, 0.0, null, 0.0, -0.5358267642107923, -0.42577926053179604, 0.0, null, 0.0, -0.06279059868570115, -0.18738139038814564, 0.0, null, 0.0, 0.06279043823356417, -0.06279059868570115, 0.0, null, 0.0, -0.6374239600187046, -0.5358267642107923, 0.0, null, 0.0, 0.1873812324664346, 0.06279043823356417, 0.0, null, 0.0, 0.30901691282787175, 0.1873812324664346, 0.0, null, 0.0, -0.7289685995410105, -0.6374239600187046, 0.0, null, 0.0, -0.8090169691755148, -0.7289685995410105, 0.0, null, 0.0, 0.42577921204230046, 0.30901691282787175, 0.0, null, 0.0, 0.9921147015831411, 1.0, 0.0, null, 0.0, -0.8763066583575159, -0.8090169691755148, 0.0, null, 0.0, 0.9685831621948102, 0.9921147015831411, 0.0], "y": [0.0, 0.36812454670549444, 0.2486898830123611, 0.0, null, 0.0, -0.6845471746835745, -0.7705133015299876, 0.0, null, 0.0, 0.5877852436214624, 0.4817536665879403, 0.0, null, 0.0, -0.8443279737429545, -0.9048270898865914, 0.0, null, 0.0, 0.4817536665879403, 0.36812454670549444, 0.0, null, 0.0, -0.7705133015299876, -0.8443279737429545, 0.0, null, 0.0, 0.3681245965320234, 0.4817537135490333, 0.0, null, 0.0, -0.5877853303315662, -0.6845471746835745, 0.0, null, 0.0, -0.48175376051012514, -0.5877853303315662, 0.0, null, 0.0, 0.844327916313264, 0.7705132332111516, 0.0, null, 0.0, -0.24868998682470217, -0.36812464635855147, 0.0, null, 0.0, -0.36812464635855147, -0.48175376051012514, 0.0, null, 0.0, -0.12533317827039392, 5.3589792725968036e-08, 0.0, null, 0.0, 0.24868993491853192, 0.3681245965320234, 0.0, null, 0.0, 0.12533328460483678, 0.24868993491853192, 0.0, null, 0.0, -0.12533333777205702, -0.24868998682470217, 0.0, null, 0.0, 0.0, -0.12533333777205702, 0.0, null, 0.0, 0.6845470965530219, 0.5877852436214624, 0.0, null, 0.0, 5.3589792725968036e-08, 0.12533328460483678, 0.0, null, 0.0, 0.7705132332111516, 0.6845470965530219, 0.0, null, 0.0, 0.9048270442517466, 0.844327916313264, 0.0, null, 0.0, -0.2486898311061897, -0.12533317827039392, 0.0, null, 0.0, -0.36812449687896437, -0.2486898311061897, 0.0, null, 0.0, 0.9510565096710907, 0.9048270442517466, 0.0, null, 0.0, -0.48175361962684576, -0.36812449687896437, 0.0, null, 0.0, 0.982287246310329, 0.9510565096710907, 0.0, null, 0.0, 0.9980267268131043, 0.982287246310329, 0.0, null, 0.0, -0.5877852002664082, -0.48175361962684576, 0.0, null, 0.0, -0.6845470574877428, -0.5877852002664082, 0.0, null, 0.0, 0.9980267301780352, 0.9980267268131043, 0.0, null, 0.0, -0.8443278875984149, -0.7705131990517303, 0.0, null, 0.0, 0.9822872563520547, 0.9980267301780352, 0.0, null, 0.0, -0.7705131990517303, -0.6845470574877428, 0.0, null, 0.0, 0.951056526231247, 0.9822872563520547, 0.0, null, 0.0, -0.9048270214343204, -0.8443278875984149, 0.0, null, 0.0, -0.9510564931109314, -0.9048270214343204, 0.0, null, 0.0, 0.9048270670691704, 0.951056526231247, 0.0, null, 0.0, -0.9822872362686005, -0.9510564931109314, 0.0, null, 0.0, 0.8443279450281105, 0.9048270670691704, 0.0, null, 0.0, -0.9980267234481706, -0.9822872362686005, 0.0, null, 0.0, -0.9980267335429632, -0.9980267234481706, 0.0, null, 0.0, 0.7705132673705709, 0.8443279450281105, 0.0, null, 0.0, -0.9822872663937775, -0.9980267335429632, 0.0, null, 0.0, -0.9510565427914007, -0.9822872663937775, 0.0, null, 0.0, 0.684547135618299, 0.7705132673705709, 0.0, null, 0.0, 0.587785286976515, 0.684547135618299, 0.0, null, 0.0, -0.9048270898865914, -0.9510565427914007, 0.0, null, 0.0, 0.12533323143761538, 0.0, 0.0, null, 0.0, 0.4817537135490333, 0.587785286976515, 0.0, null, 0.0, 0.2486898830123611, 0.12533323143761538, 0.0], "z": [0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0, null, 0.0, 0.0, 0.0, 0.0], "type": "scatter3d", "uid": "f9c95228-05ef-11e9-8196-00e01a68001a"}], {}, {"showLink": true, "linkText": "Export to plot.ly"})</script>


Inserting matplotlib plots
---------------------------

Inserting automatically-generated plots is easy.  Simply put the
script to generate the plot in the :file:`pyplots` directory, and
refer to it using the ``plot`` directive.  First make a
:file:`pyplots` directory at the top level of your project (next to
:``conf.py``) and copy the :file:`ellipses.py`` file into it::

    home:~/tmp/sampledoc> mkdir pyplots
    home:~/tmp/sampledoc> cp ../sampledoc_tut/pyplots/ellipses.py pyplots/


You can refer to this file in your sphinx documentation; by default it
will just inline the plot with links to the source and PF and high
resolution PNGS.  To also include the source code for the plot in the
document, pass the ``include-source``


In the HTML version of the document, the plot includes links to the
original source code, a high-resolution PNG and a PDF.  In the PDF
version of the document, the plot is included as a scalable PDF.

.. raw::html

   .. literalinclude:: test.div 

You can also inline code for plots directly, and the code will be
executed at documentation build time and the figure inserted into your
docs; the following code::

   .. plot::

      import matplotlib.pyplot as plt
      import numpy as np
      x = np.random.randn(1000)
      plt.hist( x, 20)
      plt.grid()
      plt.title(r'Normal: $\mu=%.2f, \sigma=%.2f$'%(x.mean(), x.std()))
      plt.show()

produces this output:

.. plot::

    import matplotlib.pyplot as plt
    import numpy as np
    x = np.random.randn(1000)
    plt.hist( x, 20)
    plt.grid()
    plt.title(r'Normal: $\mu=%.2f, \sigma=%.2f$'%(x.mean(), x.std()))
    plt.show()


See the matplotlib `pyplot tutorial
<http://matplotlib.sourceforge.net/users/pyplot_tutorial.html>`_ and
the `gallery <http://matplotlib.sourceforge.net/gallery.html>`_ for
lots of examples of matplotlib plots.

