#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <memory>

struct Node {
    std::string id;
    std::string command;
    std::vector<std::string> dependencies;
    std::vector<std::string> dependents;
    std::atomic<int> unresolved_deps{0};
};

class BuildGraph {
public:
    bool load_from_file(const std::string& filepath);
    void add_target(const std::string& id, const std::string& command, const std::vector<std::string>& deps);
    bool validate_and_prepare();
    std::unordered_map<std::string, std::shared_ptr<Node>>& get_nodes();

private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    bool dfs_cycle_detect(const std::string& node_id, std::unordered_map<std::string, int>& visited);
};
