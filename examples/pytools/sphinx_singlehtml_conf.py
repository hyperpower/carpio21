import sys, os, datetime

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
copyright = u'2024, Chengsi ZHOU'

math_numfig = True
math_eqref_format   = "Eq.{number}"

version = ''
release = ''

# -- Options for HTML output ---------------------------------------------------
html_sidebars = {
    '**': [
        'about.html',
        'localtoc.html'
        # 'navigation.html',
#         'relations.html',
#         'searchbox.html',
#         'donate.html',
    ]
}
html_theme = 'alabaster'
html_theme_options = {
    "font_family": 'Palatino, "Times New Roman", Times, serif;',
    'page_width': '900px',  # text with 650 figure width 634
    'sidebar_width': '250px',
    "font_size" : '16px'
}
html_context = {
    'current_time': datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
}
# html_theme_path = ['themes']
# html_title = "Music for Geeks and Nerds"
#html_short_title = None
#html_logo = None
#html_favicon = None
# html_static_path = ['_static']
# html_domain_indices = False
# html_use_index = False
# html_show_sphinx = False
html_show_sourcelink = False
