
import sys
import os
import platform
import importlib

# os info
OsName    = platform.system()
OsVersion = platform.version()
OsInfo    = platform.platform()
OsRelease    = platform.release()
OsArchitecture = platform.architecture()

# define color of terminal
class TermColor():
    BLACK  = '\033[30m'
    RED    = '\033[31m'
    GREEN  = '\033[32m'
    YELLOW = '\033[33m'
    BLUE   = '\033[34m'
    MAGENTA = '\033[35m'
    CYAN    = '\033[36m'
    WHITE   = '\033[37m'
    UNDERLINE = '\033[4m'
    RESET     = '\033[0m'

CMSG_YES = TermColor.GREEN + "YSE" + TermColor.RESET
CMSG_NO  = TermColor.RED   + "NO" + TermColor.RESET

# pip package 
def check_pip_package(name):
    spec = importlib.util.find_spec(name)
    if spec is not None:
        print("%-15s %-5s" % (name, CMSG_YES))
        return True
    else:
        print("%-15s %-5s" % (name, CMSG_NO))
        return False

if __name__ == '__main__':
    check_pip_package("platform")
    check_pip_package("platform3")
