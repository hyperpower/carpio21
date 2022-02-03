class TextFile:
    def __init__(self, fullname):
        self._fullname = fullname
        self._config  = {}
        self._content = []
    
    def read_file(self):
        with open(self._fullname, "r") as f:
            self._content = f.readlines()
    
    @property
    def content(self):
        return self._content
    
    @property
    def config(self):
        return self._config
    
    def parse_content_a_line(self, line):
        tl = line.strip(" ,;")
        s_comma = tl.split("#")
        s_comma = s_comma[0].split(",")
        s_comma = list(map(lambda x: x.strip(), s_comma))
        if len(s_comma) == 0:
            return None
        elif len(s_comma) == 1:
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
    
    def parse_config_a_line(self, line):
        tl = line.strip()
        if not tl.startswith("##"):
            return False
        else:
            s_colon = tl[2:].split(":")
            if len(s_colon) < 2:
                return False
            s_colon = list(map(lambda x: x.strip(), s_colon))
            # parse value
            v = self.parse_content_a_line(s_colon[1])
            self._config[s_colon[0]] = v 
            return True

    def parse(self):
        tcontent = []
        for line in self._content:
            if not self.parse_config_a_line(line):
                pl = self.parse_content_a_line(line)
                tcontent.append(pl)
        self._content = tcontent

if __name__=='__main__':
    tf = TextFile("./test_output/grid.txt")
    tf.read_file()
    tf.parse()
    for k, v in tf.config.items():
        print(k , " = ", v)
