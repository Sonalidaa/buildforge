#include "BuildGraph.hpp"
#include "Executor.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    std::string filepath = (argc > 1) ? argv[1] : "bench_graph.txt";
    size_t threads = (argc > 2) ? std::stoul(argv[2]) : 8;

    BuildGraph graph;
    if (!graph.load_from_file(filepath)) {
        std::cerr << "Failed to load/validate graph from " << filepath << "\n";
        return 1;
    }

    std::cout << "Loaded " << graph.get_nodes().size() 
              << " targets. Running with " << threads << " worker threads...\n";

    auto start = std::chrono::high_resolution_clock::now();
    Executor executor(graph, threads);
    bool ok = executor.run();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    if (ok) {
        std::cout << "\nBuild succeeded in " << elapsed.count() << " ms.\n";
    } else {
        std::cerr << "\nBuild failed.\n";
    }

    return ok ? 0 : 1;
}
