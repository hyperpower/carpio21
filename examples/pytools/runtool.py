import argparse
import os
import shutil
import platform
import sys
import errno
import argparse
import subprocess
import time
from path import *
import pathlib
# sphinx should be installed
from sphinx.cmd.build import main as sphinx_main
from env_para import *

# 
from rst_supplement import *

def parse_name(name):
    sn = name.split("-")
    # delete underscore in name 
    # Make the first letter upper case
    # sn[3] = sn[3].replace("_", " ");
    # sn[3] = sn[3].title()
    return sn

def case_info(path, origianl_files):
    info_dict = {}
    # folder name is case name
    info_dict["folder_name"] = os.path.basename(path)
    parsed_name         = parse_name(info_dict["folder_name"])
    info_dict["level1"] = int(parsed_name[0])
    info_dict["level2"] = int(parsed_name[1])
    info_dict["level3"] = int(parsed_name[2])
    info_dict["name"]   = parsed_name[3]
    info_dict["level"]  = parsed_name[0] + "-" + parsed_name[1] + "-" + parsed_name[2]

    return info_dict

def path_is_parent(parent_path, child_path):
    # Smooth out relative path names, note: if you are concerned about symbolic links, you should use os.path.realpath too
    parent_path = os.path.abspath(parent_path)
    child_path = os.path.abspath(child_path)

    # Compare the common path of the parent and child path with the common path of just the parent path. Using the commonpath method on just the parent path will regularise the path name in the same way as the comparison that deals with both paths, removing any trailing path separator
    return os.path.commonpath([parent_path]) == os.path.commonpath([parent_path, child_path])

def is_sub_path(subpath, paths):
    for p in paths:
        if path_is_parent(p, subpath):
            return True
    return False


# code files in path
def is_code_file_in(path, fn):
    pdir = pathlib.Path(fn).resolve().parent
    code_ext = [".cpp", ".hpp", ".py", "rst"]
    file_name, file_extension = os.path.splitext(fn)
    if file_extension in code_ext and pdir == path:
        return True
    else:
        return False

def print_bar(name):
        print(TermColor.BLUE + '===== {:^10s} ====='.format(name)+ TermColor.RESET )

def clean(path, original_files):
    print_bar("clean")
    ori_ff = []
    ori_fd = []
    for f in original_files:
        fn = os.path.join(path, f)
        if os.path.isfile(fn):
            ori_ff.append(fn)
        elif os.path.isdir(fn):
            ori_fd.append(fn)
    
    for (dirpath, dirnames, filenames) in os.walk(path, topdown=False):
        for name in filenames:
            ff = os.path.join(dirpath, name)
            if ff not in ori_ff and not is_sub_path(os.path.dirname(ff), ori_fd) and not is_code_file_in(path, ff):
                os.remove(ff)
    
    for (dirpath, dirnames, filenames) in os.walk(path, topdown=False):
        for name in dirnames:
            fd = os.path.join(dirpath, name)
            if fd not in ori_fd:
                try:
                    os.rmdir(fd)
                except OSError as e:
                    if e.errno != errno.ENOTEMPTY:
                        raise
    

def parse_args():
    parser=argparse.ArgumentParser(description="a script to do stuff")
    parser.add_argument('-c', "--clean", 
                        help="clean build files", 
                        action="store_true")
    parser.add_argument('-m', "--make", 
                        help="make files", 
                        action="store_true")
    parser.add_argument("-b", "--build",
                        help="build project" ,
                        action="store_true")
    parser.add_argument("-e", "--execute",
                        help="execute main file" ,
                        action="store_true")
    parser.add_argument("-p", "--plot",
                        help="plot by python" ,
                        action="store_true")
    parser.add_argument("-d", "--document",
                        help="generate document file" ,
                        action="store_true")
    parser.add_argument("-r", "--run",
                        help="run project" ,
                        action="store_true")
    parser.add_argument('--mpi', action='store_true',
                         help='run by mpi')
    parser.add_argument('--html', action='store_true',
                         help='open html')
    args=parser.parse_args()
    return args


# pytools dir
_RUNTOOL_DIR_   = os.path.abspath(os.path.join(__file__, "../"))

class Runer:
    def __init__(self, run_file, original_files, n_mp = 1, n_mpi = 1):
        self._orifiles = original_files
        self._path     = Path(run_file)
        self._info     = case_info(self._path.this, original_files)
        self._num_openmp =n_mp
        self._num_mpi    =n_mpi

        self._init_time_record()

    def project_name(self):
        return self._info["name"]

    def show_info(self):
        for k, v in self._info.items():
            print("%15s : " % k, v)

    def mkdir_all(self):
        # creat the last level of folder
        self.mkdir(self._path.fig)
        self.mkdir(self._path.data)
    
    def mkdir(self, abspath):
        if not os.path.isdir(abspath):
            os.mkdir(abspath)

    def clean(self):
        clean(self._path.this, self._orifiles)

    def cmake(self):
        path_build = os.path.join(self._path.this, "build")
        if platform.system() == "Darwin":
            cmd = f"cmake --no-warn-unused-cli" \
                  f" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE" \
                  f" -DCMAKE_BUILD_TYPE:STRING=Release" \
                  f" -S \"{self._path.this}\"" \
                  f" -B \"{path_build}\"" \
                  f" -G \"Unix Makefiles\""
        else:
            cmd = f"cmake -S \"{self._path.this}\"" \
                  f" -B \"{path_build}\"" 
        print(cmd)
        # os.system(cmd)
        result = os.popen(cmd)
        print(result.read())

    def build(self):
        project_name = self._info["name"]
        if platform.system() == "Windows":
            cmd = "cmake --build \""+ os.path.join(self._path.this, "build\"" + " --config Release -j 4 --clean-first")
        elif platform.system() == "Linux":
            cmd = "cmake --build \""+ os.path.join(self._path.this, "build\"" + " --config Release -j 4 --clean-first")
        elif platform.system() == "Darwin":
            cmd = "cmake --build \""+ os.path.join(self._path.this, "build\"" + " --config Release -j 4 --clean-first")
        print(cmd)
        result = os.popen(cmd)
        # result = subprocess.Popen(cmd)
        # print(result.stdout)
        print(result.read())
    
    def print_bar(self, name):
        print(TermColor.BLUE + '===== {:^10s} ====='.format(name)+ TermColor.RESET )

    def execute(self):
        current = os.getcwd()
        os.chdir(self._path.this)
        exe = self._path.this + "/build/Release/main.exe"
        if os.path.exists(exe):
            cmd = "\"" + exe +"\""
        elif os.path.exists(self._path.this + "/build/main"):
            cmd = self._path.this + "/build/main"
        else:
            print("! executable file not found !")
            os.chdir(current)
            return False

        exit_code = os.system(cmd)
        os.chdir(current)
        if exit_code == 0:
            return True
        else:
            print("! cmd         : %s", cmd)
            print("! return code :", exit_code)
            return False

    def execute_mpi(self):
        current = os.getcwd()
        os.chdir(self._path.this)
        exe = self._path.this + "/build/Release/main.exe"
        if os.path.exists(exe):
            cmd = "mpiexec -n %d \"" % (self._num_mpi) + exe +"\""
        elif os.path.exists(self._path.this + "/build/main"):
            cmd = "mpiexec -n %d " % (self._num_mpi) + self._path.this + "/build/main"
        else:
            print("! executable file not found !")
            os.chdir(current)
            return False

        exit_code = os.system(cmd)
        os.chdir(current)
        if exit_code == 0:
            return True
        else:
            print("! cmd         : %s", cmd)
            print("! return code :", exit_code)
            return False
        

    def plot(self):
        current = os.getcwd()
        os.chdir(self._path.this)
        if os.path.isfile("./plot.py"):
            if platform.system() == "Windows":
                os.system("python3 ./plot.py")
            # exec(open('./plot.py').read())
            elif platform.system() == "Darwin":
                os.system("python3 ./plot.py")
        os.chdir(current)

    def clean_doc(self):
        doc_dir = os.path.abspath(os.path.join(self._path.this, "doc"))
        if os.path.isdir(doc_dir):
            shutil.rmtree(doc_dir)

    def build_doc(self):
        # 
        doc_dir        = os.path.abspath(os.path.join(self._path.this, "doc"))
        doc_source_dir = os.path.abspath(os.path.join(self._path.this, "doc/source"))
        doc_build_dir  = os.path.abspath(os.path.join(self._path.this, "doc/build"))
        self.mkdir(doc_dir)
        self.mkdir(doc_source_dir)
        self.mkdir(doc_build_dir)
        # doc_source prepare
        this_dir  = os.path.abspath(self._path.this)
        # copy file
        # copy conf file to source dir
        confpy = os.path.join(_RUNTOOL_DIR_, "sphinx_singlehtml_conf.py")
        shutil.copy(confpy, os.path.join(doc_source_dir, "conf.py"))
        # copy fig folder
        if os.path.isdir(os.path.join(this_dir, "fig")):
            # delete fig in doc
            if os.path.isdir(os.path.join(doc_source_dir, "fig")):
                shutil.rmtree(os.path.join(doc_source_dir, "fig"))
            shutil.copytree(os.path.join(this_dir, "fig"), 
                            os.path.join(doc_source_dir, "fig"))
            generate_0_svg(os.path.join(doc_source_dir, "fig"))
        # revise rst
        revise_report_rst(this_dir, self._info, doc_source_dir)

        # command
        build_format  = 'html'  # singlehtml
        args = ["-b", str(build_format), str(doc_source_dir), str(doc_build_dir)]
        sphinx_main(args)

    def _init_time_record(self):
        names = [
            "make_wall_time",
            "build_wall_time",
            "execute_wall_time",
        ]
        for n in names:
            self._info[n] = 0.0

    def _run_all(self):
        run_flag = True
        self.clean()
        print_bar("make")
        t  = time.perf_counter()
        #
        self.mkdir_all()
        self.cmake()
        #
        self._info["make_wall_time"] = time.perf_counter() - t
        print_bar("build")
        t  = time.perf_counter()
        self.build()
        self._info["build_wall_time"] = time.perf_counter() - t
        print_bar("execute")
        t  = time.perf_counter()
        if(self._num_mpi > 1):
            run_flag = self.execute_mpi()
        else:
            run_flag = self.execute()
        self._info["execute_wall_time"] = time.perf_counter() - t
        if run_flag is not True:
            return run_flag
        print_bar("plot")
        t  = time.perf_counter()
        self.plot()
        self._info["plot_wall_time"] = time.perf_counter() - t
        print_bar("build doc")
        self.build_doc()
        print_bar("info")
        self.show_info()


    def run(self, args):
        record = {} 
        if args.clean:
            record["clean"] = True
            self.clean()
        if args.make:
            print("make ===== ")
            t  = time.perf_counter()
            record["make"] = True
            #
            self.mkdir_all()
            self.cmake()
            #
            self._info["make_wall_time"] = time.perf_counter() - t
        if args.build:
            t  = time.perf_counter()
            record["build"] = True
            self.build()
            self._info["build_wall_time"] = time.perf_counter() - t
        if args.execute:
            print("execute ===== ")
            t  = time.perf_counter()
            record["execute"] = True
            self.execute()
            self._info["execute_wall_time"] = time.perf_counter() - t
        if args.mpi:
            print("execute by mpi ===== ")
            t  = time.perf_counter()
            record["execute"] = True
            self.execute_mpi()
            self._info["execute_wall_time"] = time.perf_counter() - t
        if args.plot:
            print("plot ======== ")
            t  = time.perf_counter()
            record["plot"] = True
            self.plot()
            self._info["plot_wall_time"] = time.perf_counter() - t
        if args.document:
            print(" build document ===== ")
            t  = time.perf_counter()
            record["document"] = True
            self.clean_doc()
            self.build_doc()
            self._info["document_wall_time"] = time.perf_counter() - t
        if args.html:
            print(" open document ===== ")
            os.system("open ./doc/build/report.html")
            record["html"] = True
            

        if len(record) == 0:
            self.print_bar("run all")
            record["run_all"] = True
            self._run_all()

def delete_png_series(fn_prefix):
    # delete files
    for file_name in os.listdir("./fig"):
        if file_name.endswith('.png') and file_name.startswith("%s_" % fn_prefix):
            # print("./fig/" + file_name)
            os.remove("./fig/" + file_name)

def rename_last_png(fn_prefix):
    lf = []
    for file_name in os.listdir("./fig"):
        if file_name.endswith('.png') and file_name.startswith("%s_" % fn_prefix):
            lf.append(file_name)
    lf.sort()
    print("The last fig = ", lf[-1])

    cmd = "cp " + "./fig/" + lf[-1] + " ./fig/%s-last.png" % fn_prefix
    print(cmd) 
    os.system(cmd)


def make_gif(fn_prefix, gifname):
    # make gif
    print("making gif ...")
    cmd = "{} {} {}".format(
            "ffmpeg -i ./fig/{}_%03d.png".format(fn_prefix),
            "-vf \"fps=10,scale=800:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\"",
            "-y -loop 0 ./fig/%s.gif" % gifname
        )
    os.system(cmd)
    # os.system("ffmpeg -i ./fig/lb_%02d.png -vf \"fps=10,scale=800:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 ./fig/%s.gif" % (fn_prefix, filename))

    # delete files
    # for file_name in os.listdir("./fig"):
    #     if file_name.endswith('.png') and file_name.startswith("%s_" % fn_prefix):
    #         # print("./fig/" + file_name)
    #         os.remove("./fig/" + file_name)