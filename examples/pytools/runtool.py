import os
import shutil
import time

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
        os.remove(df)

    dirs = [f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f))]
    for d in dirs:
        print("Remove dir -> ", d)
        shutil.rmtree(d)


class Path:
    def __init__(self, run_file):
        self._pfile    = run_file;
        self._pthis    = os.path.abspath(os.path.join(run_file,   "../"))
        self._pcases   = os.path.abspath(os.path.join(run_file,   "../.."))
        self._pproject = os.path.abspath(os.path.join(self._pcases, "../"))
        self._pdata    = os.path.abspath(os.path.join(self._pthis,  "data"))
        self._pfig     = os.path.abspath(os.path.join(self._pthis,  "fig"))
        self._ppytools = os.path.abspath(os.path.join(self._pcases, "pytools"))

        self._pdoc       = os.path.abspath(os.path.join(self._pproject, "doc"))
        self._prstsource = os.path.abspath(os.path.join(self._pdoc, "source"))
    
    @property
    def file(self):
        return self._pfile

    @property
    def this(self):
        return self._pthis

    @property
    def cases(self):
        return self._cases

    @property
    def rstsource(self):
        return self._prstsource

    @property
    def doc(self):
        return self._pdoc

    @property
    def fig(self):
        return self._pfig

    @property
    def data(self):
        return self._pdata
    
    @property
    def project(self):
        return self._pproject


class Runer:
    def __init__(self, run_file, original_files):
        self._orifiles = original_files
        self._path     = Path(run_file)
        self._info     = case_info(self._path.this, original_files)

    def project_name(self):
        return self._info["name"]

    def mkdir(self):
        # creat the last level of folder
        os.mkdir(self._path.fig)
        os.mkdir(self._path.data)

    def clean(self):
        clean(self._path.this, self._orifiles)

    def cmake(self):
        os.system("cmake .")

    def build(self):
        project_name = self._info["name"]
        os.system("msbuild "+ project_name + ".sln")

    def run(self):
        
        pass
