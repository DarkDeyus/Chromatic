import networkx as nx
import os
import matplotlib.pyplot as plt


def get_graph_to_file(graph, regular):
    lines = nx.generate_edgelist(graph, data=False)
    k = graph.number_of_edges()
    n = graph.number_of_nodes()
    name = ''
    if regular:
        name = f"{k * 2 //n}-regular_graph_with_{n}_vertices.col"
    else:
        name = f"random_{k}_edges_graph_{n}_vertices.col"
    with open(name, 'w+') as f:
        f.write(f"p edge {n} {k}\n")
        for line in lines:
            nodes = line.split(' ')
            f.write("e " + f"{int(nodes[0])+1} {int(nodes[1]) + 1}" + '\n')


def generate_k_regular_graph_with_n_vertices(k, n):
    G = nx.random_regular_graph(k, n)
    get_graph_to_file(G, True)


def remove_comments():
    path = r".\graphs"
    col_files = []
    for item in os.listdir(path):
        if item.endswith('.col'):
            col_files.append(item)

    for item in col_files:

        with open(os.path.join(path, item)) as f:
            content = f.readlines()
        final_content = []
        for line in content:
            if not line.startswith('c'):
                final_content.append(line)
        with open(os.path.join(path, item), 'w') as f:
            for line in final_content:
                f.write(line)


def generate():
    k = []
    n = []

    for i in range(1000, 10000, 200):
        n.append(i)
    for i in range(100, 1600, 100):
        for j in range(3):
            k.append(i)
    print(len(n))
    print(len(k))
    print(n)
    print(k)
    for i in range(len(n)):
        generate_k_regular_graph_with_n_vertices(k[i], n[i])
    # generate_k_regular_graph_with_n_vertices(k, n)#
    print("done!")


def create_graph_from_file(file_location):
    g = nx.Graph()
    with open(file_location) as f:
        content = f.readlines()
    vertex_count = int(content[0].split(' ')[2])
    for i in range(vertex_count):
        g.add_node(i)
    for line in content:
        if line.startswith('e'):
            parts = line.split(' ')
            u, v = int(parts[1]) - 1, int(parts[2]) - 1
            g.add_edge(u, v)
    return g


def import_colored_graph(file_location):
    main_path = r'.\graphs'
    colors = []
    with open(file_location) as f:
        content = f.readlines()

    filename = content[0].strip('\n')
    content.remove(content[0])
    for line in content:
        m = [int(x) for x in line.split(" ")]
        colors.append(m[1])

    graph = create_graph_from_file(os.path.join(main_path, filename))

    nx.draw_networkx(graph, node_color=colors, cmap=plt.cm.Reds, with_labels=True)
    plt.show()


import_colored_graph(os.path.join(r'.', 'results', 'myciel2.col_Connected Sequential.colored'))