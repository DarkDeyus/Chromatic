import sys
import os
import subprocess


def color_a_graph(path_to_graph, path_to_exe):
    graph_name = path_to_graph.split('\\')[-1]
    graph_name = graph_name[:len(graph_name) - 4]

    for i in range(2):
        start_coloring = [path_to_exe, path_to_graph, str(i), str(1), f"{graph_name}_{i}.colored"]
        subprocess.call(start_coloring)


# Chromatic.exe C:\Users\Dark\source\repos\Chromatic\Chromatic\graphs\anna.col 0 1 output1.txt
def color_all_graphs(path_to_graphs_to_be_colored, path_to_exe):
    if not(os.path.isfile(path_to_exe)):
        print('Wrong path to Chromatic.exe file')
        return

    if not(os.path.isdir(path_to_graphs_to_be_colored)):
        print('Wrong path to the graph folder!')
        return

    for item in os.listdir(path_to_graphs_to_be_colored):
        if item.endswith('.col'):
            color_a_graph(os.path.join(path_to_graphs_to_be_colored, item), path_to_exe)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('usage: python color_all_graphs path_to_folder_with_graphs path_to_chromatic_exe')
    else:
        color_all_graphs(sys.argv[1], sys.argv[2])