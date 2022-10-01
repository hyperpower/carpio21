
import argparse
import os
# sphinx should be installed
from sphinx.cmd.build import main as sphinx_main


_CUR_ = os.path.abspath(os.path.join(__file__, "../"))


def build_doc(path):
        # 
    doc_source_dir = os.path.abspath(os.path.join(path, "source"))
    doc_build_dir  = os.path.abspath(os.path.join(path, "build"))

    # command
    build_format  = 'html'  # singlehtml
    args = ["-b", str(build_format), str(doc_source_dir), str(doc_build_dir)]
    sphinx_main(args)


if __name__ == '__main__':
    build_doc(_CUR_)