from os.path import abspath, join, dirname
import sys, os


p = join(abspath(dirname(dirname(__file__))), "script")
sys.path.append(p)
# print(join(abspath(dirname(dirname(__file__))), "script"))
os.environ['PATH']+=";"+p
# print(os.environ['PATH'])