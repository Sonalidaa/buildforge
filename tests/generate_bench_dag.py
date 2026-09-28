import random

NUM_NODES = 1000
LEVELS = 20
nodes_per_level = NUM_NODES // LEVELS

with open("bench_graph.txt", "w") as f:
    for lvl in range(LEVELS):
        for i in range(nodes_per_level):
            node_id = f"task_{lvl}_{i}"
            deps = []
            if lvl > 0:
                deps = [f"task_{lvl-1}_{r}" for r in random.sample(range(nodes_per_level), 2)]
            cmd = "true"
            f.write(f"{node_id}|{cmd}|{','.join(deps)}\n")

print(f"Generated bench_graph.txt with {NUM_NODES} targets.")
