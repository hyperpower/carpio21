
import argparse
import os,sys
# sphinx should be installed
from sphinx.cmd.build import main as sphinx_main


_DOC_ = os.path.abspath(os.path.join(__file__, "../"))


def build_doc(source_dir, build_dir):
    build_format  = 'html'  # singlehtml
    args = ["-b", str(build_format), str(source_dir), str(build_dir)]
    sphinx_main(args)

def get_single_folder(num):
    """
    Get the folder name for the given number.
    """
    path_source = os.path.join(_DOC_, "source")
    # find the folder name starting with the number
    for folder in os.listdir(path_source):
        if folder.startswith(str(num) + "-"):
            path_source = os.path.join(path_source, folder)
            break
    else:
        raise FileNotFoundError(f"Folder starting with {num} not found in {path_source}")
    return path_source

def parse_args():
    """
    Parse command line arguments.
    """
    parser = argparse.ArgumentParser(description="Build documentation.")
    parser.add_argument(
        "--num",
        type=int,
        default=-1,
        help="Number of the folder to build (default: 1)",
    )
    return parser.parse_args()

def run_figpy(path):
    """
    Run figX.py to generate figures.
    """
    for file in os.listdir(path):
        # check if the file starts with "fig" and ends with ".py"
        if file.startswith("fig") and file.endswith(".py"):
            # save the current working directory
            original_path = os.getcwd()
            # change directory to the script's directory
            os.chdir(path)
            # run the script
            cmd = f"{sys.executable} {file}"
            os.system(cmd)
            # change back to the original directory
            os.chdir(original_path)


if __name__ == '__main__':
    args = parse_args()
    if args.num == -1:
        # build all
        build_doc(os.path.join(_DOC_, "source"), os.path.join(_DOC_, "build"))
    else:
        # build single folder
        path_single = get_single_folder(args.num)
        print(f"Plot figures in {path_single}")
        run_figpy(path_single)
        build_doc(os.path.join(_DOC_, "source"), os.path.join(_DOC_, "build"))


