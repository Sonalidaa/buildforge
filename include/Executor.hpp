#pragma once
#include "BuildGraph.hpp"
#include "ThreadPool.hpp"

class Executor {
public:
    Executor(BuildGraph& graph, size_t num_threads);
    bool run();

private:
    void dispatch_node(std::shared_ptr<Node> node);

    BuildGraph& graph;
    ThreadPool pool;
    std::atomic<bool> failed{false};
    std::mutex log_mutex;
};
