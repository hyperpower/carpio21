import os

class Path:
    def __init__(self, run_file):
        self._pfile    = run_file
        # example case dir
        self._pthis    = os.path.abspath(os.path.join(run_file,   "../"))
        # example dir
        self._pcases   = os.path.abspath(os.path.join(run_file,   "../.."))
        # carpio dir
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