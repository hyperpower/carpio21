import argparse
import os
import shutil
import platform
import sys
import argparse
import time
from path import *
# sphinx should be installed
from sphinx.cmd.build import main as sphinx_main

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

def clean(path, original_files):
    print("clean ====== ")
    files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
    dfs   = []
    for f in files:
        if f not in original_files:
            dfs.append(f)
    for df in dfs:
        print("Remove file -> ", df)
        os.remove(os.path.join(path,df))

    dirs = [f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f))]
    for d in dirs:
        print("Remove dir -> ", d)
        shutil.rmtree(os.path.join(path, d))


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
    parser.add_argument("-r", "--run",
                        help="run project" ,
                        action="store_true")
    args=parser.parse_args()
    return args



class Runer:
    def __init__(self, run_file, original_files):
        self._orifiles = original_files
        self._path     = Path(run_file)
        self._info     = case_info(self._path.this, original_files)

    def project_name(self):
        return self._info["name"]

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
        cmd = f"cmake -S \"{self._path.this}\"" \
              f" -B \"{path_build}\""
        print(cmd)
        os.system(cmd)

    def build(self):
        project_name = self._info["name"]
        if platform.system() == "Windows":
            cmd = "MSBuild \""+ os.path.join(self._path.this, "build/" + project_name + ".sln\"")
        print(cmd)
        os.system(cmd)

    def execute(self):
        current = os.getcwd()
        os.chdir(self._path.this)
        os.system(self._path.this + "/build/Debug/main.exe")
        os.chdir(current)
        pass

    def build_doc(self):
        output_foldername   = 'build-doc'
        build_doc = os.path.abspath(os.path.join(self._path.this,  output_foldername))
        self.mkdir(build_doc)
        
        build_format  = 'html'  # singlehtml, ...
        master_doc    = 'report'
        source_suffix = '.rst'
        html_theme    = 'alabaster'

        args  = f". -b {build_format} -D extensions=sphinx.ext.autodoc " \
                f"-D master_doc={master_doc} " \
                f"-D source_suffix={source_suffix} " \
                f"-D html_theme={html_theme} " \
                f"-C {output_foldername} "

        sys.path.append(os.path.abspath(self._path.this))
        sphinx_main(args.split())

    def _init_time_record(self):
        names = [
            "make_wall_time",
            "build_wall_time",
            "execute_wall_time",
            "doc_wall_time",
        ]
        for n in names:
            self._info[n] = 0.0

    def _run_all(self):
        self.clean()
        print("make ===== ")
        t  = time.perf_counter()
        #
        self.mkdir_all()
        self.cmake()
        #
        self._info["make_wall_time"] = time.perf_counter() - t
        print("build ===== ")
        t  = time.perf_counter()
        self.build()
        self._info["build_wall_time"] = time.perf_counter() - t




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
            record["execute"] = True
            print("execute")
        
        if len(record) == 0:
            record["run_all"] = True
            print("run_all")
        
        print(self._info)
