# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'carpio-doc'
copyright = '2025, HyperPower'
author = 'HyperPower'
release = '0.0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['breathe', 'exhale']

exhale_args = {
    "containmentFolder": "./api",  # API 文档生成的目录
    "rootFileName": "library_root.rst",  # 根 RST 文件名
    "doxygenStripFromPath":  "..",
    # "doxygenPath": "../xml",  # Doxygen XML 输出路径
    # 
    "rootFileTitle": "C++ API 文档",  # 
    "createTreeView": True,  # 
    "exhaleExecutesDoxygen": False,  # 是否由 Exhale 运行 Doxygen
    "afterTitleDescription": "以下是项目的 C++ API 文档。",
    "fullApiSubSectionTitle": "完整 API 列表",
    # "exhaleDoxygenStdin": """
    #     INPUT = <你的C++代码目录>
    #     RECURSIVE = YES
    #     GENERATE_XML = YES
    # """  # 如果 exhaleExecutesDoxygen=True，则需提供 Doxygen 配置
}

breathe_projects = {"carpio-doc": "../xml"}  # Doxygen XML 输出路径
breathe_default_project = "carpio-doc"

templates_path = ['_templates']
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']
