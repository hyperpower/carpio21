import sys
import os
import platform
import shutil
import requests

_FILE_PATH_ = os.path.abspath(__file__)
_FILE_DIR_  = os.path.abspath(os.path.join(_FILE_PATH_,  "../"))
_O_WORKING_ = os.path.abspath(os.getcwd())


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

def check_and_new_folder(foldername):
    if not os.path.isdir("./" + foldername):
        os.mkdir("./" + foldername)

def download_googletest():
    print("git clone googletest")
    d   = "./external/googletest"
    url = "https://github.com/google/googletest.git"
    os.system(" git clone --progress %s %s" % (url, d))

def download_ffmpeg():
    print("download ffmpeg")
    response = requests.get("https://api.github.com/repos/BtbN/FFmpeg-Builds/releases/latest")
    assets   = response.json()["assets"]
    for a in assets:
        fn = a["name"]
        if filter_fn_ffmpeg(fn):
            # Invoke-WebRequest -Uri {download_url} -OutFile {outfile}
            download_url = a["browser_download_url"]
            os.system("powershell.exe Invoke-WebRequest -Uri {} -OutFile {}".format(download_url, "./external/ffmpeg.zip"))
            shutil.unpack_archive("./external/ffmpeg.zip", "./external/ffmpeg")
            return
    print("download ffmpeg failed")

def check_ffmpeg():
    # recursive find bin
    for currentpath, folders, files in os.walk("./external/"):
        for file in files:
            if file == "ffmpeg.exe":
                exe = os.path.abspath(os.path.join(currentpath, file))
                output = os.popen(exe + " -version")
                if "FFmpeg" in output.read():
                    return True
                else:
                    return False
    return False

# filename filter functions
def filter_fn_ffmpeg(fn):
    conditions = [
        lambda x : file_name_has(x, "ffmpeg"),
        lambda x : file_name_has(x, ".zip"),
        lambda x : file_name_has(x, "win"),
        lambda x : file_name_has(x, "latest")
    ]
    for c in conditions:
        if not c(fn):
            return False
    return True

def file_name_has(fn, str):
    if str in fn:
        return True
    else:
        return False

def file_name_not_has(fn, str):
    if str in fn:
        return False
    else:
        return True

    
def main():
    check_path()
    check_python()
    check_and_new_folder("external")
    check_and_new_folder("test_output")
    download_googletest()
    # if not check_ffmpeg():
        # download_ffmpeg()
    restore_path()    

if __name__ == '__main__':
    main()

