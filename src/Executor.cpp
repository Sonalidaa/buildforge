#include "Executor.hpp"
#include <iostream>
#include <cstdlib>

Executor::Executor(BuildGraph& g, size_t num_threads) 
    : graph(g), pool(num_threads) {}

void Executor::dispatch_node(std::shared_ptr<Node> node) {
    pool.enqueue([this, node]() {
        if (failed.load()) return;

        {
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cout << "[RUNNING] " << node->id << " -> " << node->command << "\n";
        }

        int ret = std::system(node->command.c_str());
        if (ret != 0) {
            std::lock_guard<std::mutex> lock(log_mutex);
            std::cerr << "[FAILED] " << node->id << "\n";
            failed.store(true);
            return;
        }

        for (const auto& dep_id : node->dependents) {
            auto dep_node = graph.get_nodes()[dep_id];
            if (dep_node->unresolved_deps.fetch_sub(1) == 1) {
                dispatch_node(dep_node);
            }
        }
    });
}

bool Executor::run() {
    for (auto& [id, node] : graph.get_nodes()) {
        if (node->unresolved_deps.load() == 0) {
            dispatch_node(node);
        }
    }

    pool.wait_until_idle();
    return !failed.load();
}
