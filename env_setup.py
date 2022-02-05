# python 3 check
from site import abs_paths
import sys
import os

_FILE_PATH_ = os.path.abspath(__file__)
_FILE_DIR_  = os.path.abspath(os.path.join(_FILE_PATH_,  "../"))
_O_WORKING_   = os.path.abspath(os.getcwd())

def check_path():
    cwd = os.path.abspath(os.getcwd())
    if not cwd == _FILE_DIR_:
        print("Change current working directory to ", _FILE_DIR_)
        os.chdir(_FILE_DIR_) 

def restore_path():
    os.chdir(_O_WORKING_)

def check_python():
    print("You are using Python {}.{}.".format(sys.version_info.major, sys.version_info.minor))
    if not sys.version_info.major == 3 and sys.version_info.minor >= 6:
        print("Python 3.6 or higher is required.")
        sys.exit(1)

def exeternal_check():
    if not os.path.isdir("./external"):
        os.mkdir("./external")

def download_googletest():
    print("git clone googletest")
    d   = "./external/googletest"
    url = "https://github.com/google/googletest.git"
    os.system(" git clone --progress %s %s" % (url, d))
    

def main():
    check_path()
    check_python()
    exeternal_check()
    download_googletest()
    restore_path()    

if __name__ == '__main__':
    main()

