# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project   = 'carpio-doc'
copyright = '2021, zhou chengsi'
author    = 'zhou chengsi'

# The full version, including alpha/beta/rc tags
release = '2021.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    # 'sphinx.ext.mathbase',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx.ext.githubpages',
    'sphinx.ext.intersphinx',
    'matplotlib.sphinxext.plot_directive',
    # 'jupyter_sphinx'
    # 'sphinx_exec_code',
    #'breathe' 
    # 'exhale'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

pygments_style = None


# -- Options for HTML output -------------------------------------------------

# mathjax_path = 'https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML'
# mathjax_config = {
#     'extensions': ['tex2jax.js'],
#     'jax': ['input/TeX', 'output/HTML-CSS'],
# }

# Figure Number
# If true, figures, tables and code-blocks are automatically numbered if they has caption. 
# For now, it works only with the HTML builder. Default is False.
numfig = True
math_numfig = True
# The scope of figure numbers, that is, the numfig feature numbers figures in which scope. 
# 0 means “whole document”. 1 means “in a section”. 
# Sphinx numbers like x.1, x.2, x.3... 2 means “in a subsection”. 
# Sphinx numbers like x.x.1, x.x.2, x.x.3..., and so on. Default is 1.
numfig_secnum_depth = 1
math_eqref_format   = "Eq.{number}"

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'

html_theme_options = {
    'github_user'     : 'hyperpower', # github icon shows under the logo
    'github_repo'     : 'Carpio21',
    'fixed_sidebar'   : True,
    'logo_name'       : True,
    'logo_text_align' : 'center',
    'show_powered_by' : False,
    'font_size'       : '18px'
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']