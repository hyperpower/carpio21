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
    parser.add_argument("-d", "--document",
                        help="generate document file" ,
                        action="store_true")
    parser.add_argument("-r", "--run",
                        help="run project" ,
                        action="store_true")
    args=parser.parse_args()
    return args


# pytools dir
_RUNTOOL_DIR_   = os.path.abspath(os.path.join(__file__, "../"))

class Runer:
    def __init__(self, run_file, original_files):
        self._orifiles = original_files
        self._path     = Path(run_file)
        self._info     = case_info(self._path.this, original_files)

        self._init_time_record()

    def project_name(self):
        return self._info["name"]

    def show_info(self):
        for k, v in self._info.items():
            print("%10s : " % k, v)

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
            cmd = "cmake --build \""+ os.path.join(self._path.this, "build\"" + " --config Release -j 10")
        print(cmd)
        os.system(cmd)

    def execute(self):
        current = os.getcwd()
        os.chdir(self._path.this)
        print(self._path.this + "/build/Release/main.exe")
        os.system("\"" + self._path.this + "/build/Release/main.exe\"")
        os.chdir(current)
        pass

    def build_doc(self):
        # 
        doc_dir        = os.path.abspath(os.path.join(self._path.this, "doc"))
        doc_source_dir = os.path.abspath(os.path.join(self._path.this, "doc/source"))
        doc_build_dir  = os.path.abspath(os.path.join(self._path.this,  "doc/build"))
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
            shutil.move(os.path.join(this_dir, "fig"), 
                        os.path.join(doc_source_dir, "fig"))
        # revise rst
        revise_report_rst(this_dir, self._info, doc_source_dir)

        # command
        build_format  = 'html'  # singlehtml
        args = f"-b {build_format} " \
               f"{doc_source_dir} {doc_build_dir}"
        sphinx_main(args.split())

    def _init_time_record(self):
        names = [
            "make_wall_time",
            "build_wall_time",
            "execute_wall_time",
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
        print("build ======== ")
        t  = time.perf_counter()
        self.build()
        self._info["build_wall_time"] = time.perf_counter() - t
        print("execute ====== ")
        t  = time.perf_counter()
        self.execute()
        self._info["execute_wall_time"] = time.perf_counter() - t
        print("build doc ====")
        # t  = time.perf_counter()
        self.build_doc()
        # self._info["document_wall_time"] = time.perf_counter() - t


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
        if args.document:
            print(" build document ===== ")
            t  = time.perf_counter()
            record["document"] = True
            self.build_doc()
            self._info["document_wall_time"] = time.perf_counter() - t

        
        if len(record) == 0:
            print("run_all ==== ")
            record["run_all"] = True
            self._run_all()
            
        
        # print(self._info)
