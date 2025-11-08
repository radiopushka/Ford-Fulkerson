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
make

# Run the program
./flkr

# Interactive session example:
$ node A
$ node B
$ node n1
$ node n2
$ node n3
$ adde A n1 50
added edge A -> n1 | 50
$ adde A n2 50
added edge A -> n2 | 50
$ adde A n3 50
added edge A -> n3 | 50
$ node s1
$ node s2
$ node s3
$ adde s1 B 50
added edge s1 -> B | 50
$ adde s2 B 50
added edge s2 -> B | 50
$ adde s3 B 50
added edge s3 -> B | 50
$ adde s1 n1 5
added edge s1 -> n1 | 5
$ adde s2 n1 3
added edge s2 -> n1 | 3
$ adde n2 s2 16
added edge n2 -> s2 | 16
$ adde s2 n3 7
added edge s2 -> n3 | 7
$ adde n3 s3 14
added edge n3 -> s3 | 14
$ nsrc A
$ nsnk B
$ flkr
fulkerson run info, max flow: 30
path:
node: A to n3 weight 50
node: n3 to s3 weight 14
node: s3 to B weight 50
path:
node: A to n2 weight 50
node: n2 to s2 weight 16
node: s2 to B weight 50
maxflow: 30
$ clear

```

## Algorithm Details

This implementation uses the Ford-Fulkerson method with Depth-First Search (DFS) to find augmenting paths. The algorithm:

1. Finds an augmenting path from source to sink using DFS
2. Determines the maximum possible flow along that path (bottleneck capacity)
3. Updates the residual capacities along the path
4. Repeats until no more augmenting paths exist

## Building

```bash
make
```

## Notes

- Nodes are identified by string names
- Edges are directed with integer weights/capacities
- The graph is maintained in memory during the session
- Source and sink must be set before running the algorithm
- The program provides feedback on operation success/failure

## Exit

Type `exit` to quit the program and free all allocated memory.
