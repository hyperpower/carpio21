import unicodedata
# from scipy import interpolate as inter
from env_para import *

class DataBlock:
    def __init__(self, count):
        self._count   = count
        self._config  = {}
        self._content = []
        self._content_col = None

    @property
    def content(self):
        return self._content
    
    @property
    def config(self):
        return self._config

    def sort_increase(self, idx):
        self._content = sorted(self._content, key=lambda r: r[idx])
        pass
    
    def row(self, idx):
        return self._content[idx]

    def col(self, idx):
        if self._content_col is None:
            self.content_to_column_wise()
        return self._content_col[idx]

    def get_col_min_max(self, col_idx):
        if self._content_col is None:
            self.content_to_column_wise()
        
        dc = self._content_col[col_idx]
        return min(dc), max(dc)

    def content_to_column_wise(self):
        self._content_col = []
        cc = len(self.content[0])
        for c in range(0, cc):
            ac = []       #  new a column
            for row in self._content:
                ac.append(row[c])
            self._content_col.append(ac)

    def parse_content_a_line(line):
        line = unicodedata.normalize('NFKC',line)
        tl = line.strip(' ,;\t\n')
        s_comma = tl.split("#")
        s_comma = s_comma[0].split(",")
        s_comma = list(map(lambda x: x.strip(' ,;\t\n'), s_comma))
        
        if len(s_comma) == 0:
            return None
        elif len(s_comma) == 1:
            if s_comma[0] == '':
                return None
            try:
                v = float(s_comma[0])
                return v
            except:
                return s_comma[0]
        else:
            try:
                s_comma = list(map(lambda x: float(x), s_comma))
                return s_comma
            except:
                return s_comma
    
    def is_config_line(line):
        line = unicodedata.normalize('NFKC',line)
        tl = line.strip()
        if not tl.startswith("##"):
            return False
        else:
            s_colon = tl[2:].split(":")
            if len(s_colon) < 2:
                return False
        return True
    
    def parse_config_a_line(self, line):
        line = unicodedata.normalize('NFKC',line)
        tl = line.strip()
        if not tl.startswith("##"):
            return False
        else:
            s_colon = tl[2:].split(":")
            if len(s_colon) < 2:
                return False
            s_colon = list(map(lambda x: x.strip(), s_colon))
            # parse value
            v = DataBlock.parse_content_a_line(s_colon[1])
            self._config[s_colon[0]] = v 
            return True
    
    def show(self):
        print(">>Block Count : ", self._count)
        print("  Num Config  : ", len(self._config))
        print("  Num Content : ", len(self._content))
        print("Config ---- ")
        for k, v in self._config.items():
            print(k, " : ", v)
        print("Data   ---- ")
        for v in self._content:
            print(v)
        print("<<<<<<<")

class DataFile:
    def __init__(self, fullname):
        self._fullname = fullname
        self._block  = []

    def read_file(self):
        with open(self._fullname, "r", encoding="utf8") as f:
            self._block = f.readlines()

    @property
    def block(self):
        return self._block
    
    def sort_block_by(self, k):
        self._block.sort(key=lambda b : b.config[k])
    
    def sort_block_data_by(self, idx):
        for b in self._block:
            b.sort_increase(idx)
    
    def get_col_min_max(self, c):
        arr = []
        for b in self._block:
            mi, ma = b.get_col_min_max(c)
            arr.append(mi)
            arr.append(ma)
        return min(arr), max(arr)
        
    
    def get_block_list_by(self, k):
        res = []
        for b in self._block:
            res.append(b.config[k])
        res.sort()
        return res
    
    def list_block_config_by(self, key):
        res = []
        for b in self._block:
            res.append(b.config[key])
        return res
    
    def parse(self):
        tblock = []
        count  = 0 
        bflag  = 0
        for line in self._block:
            if DataBlock.is_config_line(line):
                if bflag == 0:
                    bflag = 1
                    tblock.append(DataBlock(count))
                tblock[-1].parse_config_a_line(line)
            else:
                if bflag == 1:
                    bflag = 0
                    count = count + 1
                cl = DataBlock.parse_content_a_line(line)
                if cl is not None:
                    tblock[-1].content.append(cl)
        
        self._block = tblock


if __name__=='__main__':
    tf = DataFile(os.path.join(DIR_PROJECT, "test/input_files/textfile.txt"))
    tf.read_file()
    tf.parse()
    print(len(tf.block))
    for b in tf.block:
        b.show()
    # tf.sort_block_by("Series")
    # print(tf.list_block_config_by("Series"))
    