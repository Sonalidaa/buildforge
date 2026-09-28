# BuildForge: Dependency-Aware Parallel Build System

A high-performance, multithreaded build automation engine written in C++20. BuildForge models build targets as a Directed Acyclic Graph (DAG) and executes compilation units concurrently across a configurable worker thread pool while strictly preserving dependency ordering.

---

## ⚡ Key Highlights

- **DAG Dependency Resolution:** Accurately models targets, dependencies, and dependent nodes using adjacency lists and topological invariants.
- **Cycle Detection (3-Color DFS):** Traverses the build graph using White/Gray/Black node coloring to detect circular dependencies prior to execution, preventing runtime deadlocks.
- **Dynamic In-Degree Task Dispatch:** Eliminates static tiering barriers. As soon as a target finishes, it atomically decrements the unresolved dependency count of its dependents, dispatching them the instant their in-degree reaches zero.
- **Thread Pool Architecture:** Custom producer-consumer thread pool built from scratch using `std::mutex`, `std::condition_variable`, and worker threads, coordinating up to 16+ workers with synchronized resource access.
- **High Scalability:** Validated against synthetic workloads exceeding 1,000+ interdependent targets without deadlock or memory leaks.

---

