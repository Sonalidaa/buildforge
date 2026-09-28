#include "BuildGraph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void BuildGraph::add_target(const std::string& id, const std::string& command, const std::vector<std::string>& deps) {
    if (!nodes.count(id)) {
        nodes[id] = std::make_shared<Node>();
        nodes[id]->id = id;
    }
    nodes[id]->command = command;
    nodes[id]->dependencies = deps;

    for (const auto& dep : deps) {
        if (!nodes.count(dep)) {
            nodes[dep] = std::make_shared<Node>();
            nodes[dep]->id = dep;
        }
        nodes[dep]->dependents.push_back(id);
    }
}

bool BuildGraph::dfs_cycle_detect(const std::string& node_id, std::unordered_map<std::string, int>& visited) {
    visited[node_id] = 1;

    for (const auto& next : nodes[node_id]->dependents) {
        if (visited[next] == 1) return true;
        if (visited[next] == 0 && dfs_cycle_detect(next, visited)) return true;
    }

    visited[node_id] = 2;
    return false;
}

bool BuildGraph::validate_and_prepare() {
    std::unordered_map<std::string, int> visited;
    for (const auto& [id, _] : nodes) visited[id] = 0;

    for (const auto& [id, _] : nodes) {
        if (visited[id] == 0) {
            if (dfs_cycle_detect(id, visited)) {
                std::cerr << "Error: Cycle detected in build graph!\n";
                return false;
            }
        }
    }

    for (auto& [id, node] : nodes) {
        node->unresolved_deps.store(static_cast<int>(node->dependencies.size()));
    }
    return true;
}

bool BuildGraph::load_from_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Cannot open graph file: " << filepath << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, cmd, dep_str;

        if (std::getline(ss, id, '|') && std::getline(ss, cmd, '|')) {
            std::getline(ss, dep_str, '|');
            std::vector<std::string> deps;
            std::stringstream dep_ss(dep_str);
            std::string d;
            while (std::getline(dep_ss, d, ',')) {
                if (!d.empty()) deps.push_back(d);
            }
            add_target(id, cmd, deps);
        }
    }
    return validate_and_prepare();
}

std::unordered_map<std::string, std::shared_ptr<Node>>& BuildGraph::get_nodes() {
    return nodes;
}
