import sys, os

sys.path.insert(0, os.path.abspath('extensions'))

extensions = ['sphinx.ext.autodoc',
              'sphinx.ext.mathjax',
              'sphinx.ext.graphviz']

todo_include_todos = True
# templates_path = ['_templates']
master_doc    = 'report'
source_suffix = '.rst'

exclude_patterns = []
# add_function_parentheses = True
#add_module_names = True
# A list of ignored prefixes for module index sorting.
#modindex_common_prefix = []
numfig = True
project = u'Carpio'
copyright = u'2022, Chengsi ZHOU'

version = ''
release = ''

# -- Options for HTML output ---------------------------------------------------
html_sidebars = {
    '**': [
        'about.html',
        'navigation.html',
#         'relations.html',
#         'searchbox.html',
#         'donate.html',
    ]
}
html_theme = 'alabaster'
html_theme_options = {
    # "globaltoc_maxdepth": -1,
    # "sidebar_collapse" : False,
}
page_width = 800
# html_theme_path = ['themes']
# html_title = "Music for Geeks and Nerds"
#html_short_title = None
#html_logo = None
#html_favicon = None
# html_static_path = ['_static']
# html_domain_indices = False
# html_use_index = False
# html_show_sphinx = False
# htmlhelp_basename = 'MusicforGeeksandNerdsdoc'
html_show_sourcelink = False
