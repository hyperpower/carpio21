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
        return "File not Found"
    except json.JSONDecodeError:
        return "JSON file format error"


def get_data_frame(data):
    df = pd.DataFrame(columns=['Name', 'Size', 'Time', 'TimeUnit'])
    for benchmark in data['benchmarks']:
        an = benchmark["name"].split("/")
        df.loc[len(df)] = [an[0], an[1], benchmark['real_time'], benchmark['time_unit']]
    return df 

def select_by_name(df, name_op):
    groups = []
    for name, group in df.groupby('Name'):
        if name.startswith("BM_" + name_op):
            groups.append(group)
    return groups

def select_ref_group(groups, name_op, name_ref):
    for group in groups:
        sn = group["Name"].unique()[0]
        if(sn.startswith("BM_" + name_op + name_ref)):
            return group

def plot_data_groups_ref(groups, name_op, name_ref):
    plt.figure(figsize=(6, 5))
    bar_width = 0.18    

    refg = select_ref_group(groups, name_op, name_ref)
    refg.sort_values(by='Size')
    reft = refg['Time'].to_numpy() 
    refs = refg['Size'].astype(float).unique()
    refrange = range(len(refs))
    for group in groups:
        name = group["Name"].unique()
        group.sort_values(by='Size')
        t = group['Time'].to_numpy()
        ratio = t / reft  

        plt.plot(refs, ratio, marker='o', label = name )
        
    plt.xlabel('Vector Size')
    plt.ylabel('Time (μs)')
    # plt.xlim(10, 1e7)
    # plt.yscale('log')
    plt.xscale('log')
    plt.legend()
    plt.grid(True) 

    plt.savefig(PATH_FIG + "/" +"BM_" + name_op + "_ratio")


def plot_data_frame(df, name_op):
    plt.figure(figsize=(6, 5))
    
    for name, group in df.groupby('Name'):
        if name.startswith("BM_" + name_op):
            x = group["Size"].astype(float)
            y = group["Time"].astype(float)
            plt.plot(x.values, y.values, marker='o', label=name)

    plt.xlabel('Vector Size')
    plt.ylabel('Time (μs)')
    # plt.xlim(10, 1e7)
    plt.yscale('log')
    plt.xscale('log')
    plt.legend()
    plt.grid(True) 

    plt.savefig(PATH_FIG + "/" +"BM_" + name_op)


if __name__ == "__main__":
    data = read_json_from_file("./data/bm_result.txt")

    name_op = "BLAS0Add"
    df = get_data_frame(data)
    plot_data_frame(df, name_op)
    gs = select_by_name(df, name_op)
    plot_data_groups_ref(gs, name_op, "_Normal")


    name_op = "VectorAdd"
    plot_data_frame(df, name_op)
    gs = select_by_name(df, name_op)
    plot_data_groups_ref(gs, name_op, "Raw")
    