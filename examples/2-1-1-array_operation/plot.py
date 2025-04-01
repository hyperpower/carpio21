import json, os
import pandas as pd
import matplotlib.pyplot as plt

PATH_CASES    = os.path.abspath(os.path.join(__file__, "../.."))
PATH_THIS     = os.path.abspath(os.path.join(__file__, "../"))
PATH_DATA     = os.path.abspath(os.path.join(PATH_THIS, "data"))
PATH_FIG      = os.path.abspath(os.path.join(PATH_THIS, "fig"))
PATH_PROJECT  = os.path.abspath(os.path.join(PATH_CASES, "../"))
PATH_PYTOOLS  = os.path.abspath(os.path.join(PATH_CASES, "pytools"))

plt.style.use(os.path.join(PATH_PYTOOLS, "web.mplstyle"))

def read_json_from_file(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            data = json.load(file)
            return data
    except FileNotFoundError:
        return "文件未找到"
    except json.JSONDecodeError:
        return "JSON格式错误"

def get_test_name_set(data):
    name_set = set()
    for t in data["benchmarks"]:
        an = t["name"].split("/")
        test_name = an[0]
        test_n    = an[1]
        test_time = t["real_time"]
        unit      = t["time_unit"]
        name_set.add(test_name)
    return name_set

def get_data_frame(data):
    df = pd.DataFrame(columns=['Name', 'Size', 'Time', 'TimeUnit'])
    for benchmark in data['benchmarks']:
        an = benchmark["name"].split("/")
        df.loc[len(df)] = [an[0], an[1], benchmark['real_time'], benchmark['time_unit']]
    return df 

def plot_data_frame(df, name_op):
    plt.figure(figsize=(6, 5))
    bar_width = 0.2    
    
    for name, group in df.groupby('Name'):
        x = group["Size"]
        y = group["Time"]
        plt.plot(x, y, marker='o', label=name)

    plt.xlabel('Vector Size')
    plt.ylabel('Time (μs)')
    plt.legend()
    plt.grid(True) 

    plt.savefig(PATH_FIG + "/" +"bm_" + name_op)

def plot_data_frame_box(df, name_op):
    plt.figure(figsize=(6, 5))
    bar_width = 0.18   

    sizes        = df['Size'].unique()
    series_names = df['Name'].unique()
    series_names.sort()

    index = range(len(sizes))

    for i, sn in enumerate(series_names):
        group = df[df['Name'] == sn]
        x_offset = len(index) * bar_width * 0.5 - 0.5 * bar_width
        x_pos = [(j - x_offset) + bar_width * i for j in index]
        bars = plt.bar(x_pos, group['Time'], bar_width, label=sn)

    plt.xticks(index, sizes)
    plt.xlabel('Vector Size (n)')
    plt.ylabel('Time (μs)')
    plt.legend()
    plt.grid(True) 

    plt.savefig(PATH_FIG + "/" +"bm_" + name_op + "_box")

def plot_data_frame_box_r(df, name_op, name_ref):
    plt.figure(figsize=(6, 5))
    bar_width = 0.18   

    sizes        = df['Size'].unique()
    series_names = df['Name'].unique()
    series_names.sort()

    index = range(len(sizes))
    
    groupr = df[df["Name"] == name_ref]
    groupr.sort_values(by='Size')
    ar = groupr['Time'].to_numpy()
    for i, sn in enumerate(series_names):
        group = df[df['Name'] == sn]
        group.sort_values(by='Size')
        av = group['Time'].to_numpy()
        ratio = av / ar 
       
        x_offset = len(index) * bar_width * 0.5 - 0.5 * bar_width
        x_pos = [(j - x_offset) + bar_width * i for j in index]
        bars = plt.bar(x_pos, ratio, bar_width, label=sn)

    plt.xticks(index, sizes)
    plt.xlabel('Vector Size (n)')
    plt.ylabel('Speed Ratio to Raw')
    plt.legend()
    plt.grid(True) 

    plt.savefig(PATH_FIG + "/" +"bm_" + name_op + "_boxr") 

if __name__ == "__main__":
    name_op = "vector_add"
    data = read_json_from_file("./data/bm_" + name_op+".txt")

    df = get_data_frame(data)
    plot_data_frame(df, name_op)
    plot_data_frame_box_r(df, name_op, "BM_RawVectorAdd")
    