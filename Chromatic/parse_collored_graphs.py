import csv
import os
import sys

wrong_graphs = []
def parse_graph(path_to_graph, filewriter):
    graph_name = ''
    max_color = 0
    vertex_count = 0
    edges_count = 0
    coloring_name = ''
    zajac_steps = 0
    with open(path_to_graph) as graph:
        base_graph = graph.readline().strip('\n')
        if not base_graph.endswith('.col'):
            wrong_graphs.append(f'wrong name? {path_to_graph} {base_graph}')
            return
        graph_name = base_graph.split('\\')[-1]
        graph_name = graph_name[:len(graph_name) - 4]
        with open(base_graph) as base_graph:
            parts = base_graph.readline().strip('\n').split(' ')
            vertex_count = parts[2]
            edges_count = parts[3]

        coloring_name = graph.readline().strip('\n')
        zajac_steps = graph.readline().strip('\n')
        time = graph.readline().strip('\n')
        lines = graph.readlines()
        for line in lines:
            if line.isspace():
                continue
            parts = line.split(' ')
            if int(parts[1]) > max_color:
                max_color = int(parts[1])

    filewriter.writerow([graph_name, vertex_count, edges_count, coloring_name, max_color, time, zajac_steps])

def parse_colored_graphs_int_csv(path_to_folder):
    if not os.path.isdir(path_to_folder):
        print("Provide correct path to directory")
        return
    with open('graphs_results.csv', 'w') as csvfile:
        filewriter = csv.writer(csvfile, delimiter=',',
                                quotechar='|', quoting=csv.QUOTE_MINIMAL)
        filewriter.writerow(['Name of graph', 'Vertices count', 'Edges count', 'Coloring name', 'Colors used', 'Execution time', 'Zajac step count'])

        for item in os.listdir(path_to_folder):
            if item.endswith('.colored'):
                parse_graph(os.path.join(path_to_folder, item), filewriter)

    return

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: python parse_colored_graphs.py path_to_folder_with_colored_graphs')
    else:
        parse_colored_graphs_int_csv(sys.argv[1])

    print(wrong_graphs)



