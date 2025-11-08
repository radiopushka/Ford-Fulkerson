# Ford-Fulkerson Algorithm CLI

A command-line interface implementation of the Ford-Fulkerson algorithm for computing maximum flow in flow networks. This interactive tool allows you to build graphs, set source and sink nodes, and calculate maximum flow between them.

## Features

- **Graph Management**: Add and remove nodes and edges
- **Flow Calculation**: Compute maximum flow using Ford-Fulkerson algorithm
- **Interactive CLI**: Simple command-line interface for graph manipulation
- **Path Visualization**: Display the augmenting paths found during execution

## Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `node`  | `node <name>` | Add a new node with the given name |
| `adde`  | `adde <node1> <node2> <weight>` | Add a directed edge from node1 to node2 with given weight |
| `rmed`  | `rmed <node1> <node2>` | Remove edge between node1 and node2 |
| `rmnd`  | `rmnd <node>` | Remove a node and all its connected edges |
| `nsrc`  | `nsrc <node>` | Set the source node for maximum flow calculation |
| `nsnk`  | `nsnk <node>` | Set the sink node for maximum flow calculation |
| `flkr`  | `flkr` | Run Ford-Fulkerson algorithm to compute maximum flow |
| `list`  | `list` | List all nodes in the graph |
| `cler`  | `cler` | Clear the entire graph and reset |
| `help`  | `help` | Show available commands |
| `exit`  | `exit` | Exit the program |

## Usage Example

```bash
# Build the project (assuming gcc)
gcc -o ford_fulkerson_cli main.c ford_fulkerson.c

# Run the program
./ford_fulkerson_cli

# Interactive session example:
$ node A
$ node B  
$ node C
$ node D
$ adde A B 10
$ adde A C 5
$ adde B D 8
$ adde C D 7
$ nsrc A
$ nsnk D
$ flkr
# Output shows maximum flow and augmenting paths
```

## Algorithm Details

This implementation uses the Ford-Fulkerson method with Depth-First Search (DFS) to find augmenting paths. The algorithm:

1. Finds an augmenting path from source to sink using DFS
2. Determines the maximum possible flow along that path (bottleneck capacity)
3. Updates the residual capacities along the path
4. Repeats until no more augmenting paths exist

## Dependencies

- C standard library
- Custom `ford_fulkerson.h` header (implementation of the algorithm)
- The code expects accompanying `ford_fulkerson.c` file with graph data structures and algorithm implementation

## Building

```bash
gcc -o ford_fulkerson_cli main.c ford_fulkerson.c
```

## Notes

- Nodes are identified by string names
- Edges are directed with integer weights/capacities
- The graph is maintained in memory during the session
- Source and sink must be set before running the algorithm
- The program provides feedback on operation success/failure

## Exit

Type `exit` to quit the program and free all allocated memory.
