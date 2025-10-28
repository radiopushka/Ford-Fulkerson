//ford fulkerson algorithm has uses in GPS, Telecommunications, City Planning
//it can also be used in ML and projects like music generation

//Evan Nikitin Tue Oct 28 2025

#ifndef FORDFULKERSON
#define FORDFULKERSON

//structures
struct edge{
    void* pointer;
    int weight;
    int original;
    int reversed;
    struct edge* next;
};
struct node{
    void* descriptor;
    struct edge* connections;
    struct node* tracker;

};

struct graph{
    struct node* verts;
};


//DFS structures:
struct DFS{
    struct node* visted;
    struct DFS* next;
};
struct DFS_path{
    struct node* from;
    struct edge* conn;
    struct DFS_path* next;
};


//fulkerson information structures
struct fulkerson_paths{
    struct DFS_path* path;
    int limit;
    struct fulkerson_paths* next;
};
struct fulkerson_info{
    struct fulkerson_paths* paths;
    struct graph* rem_graph;
    int max_flow;
};


//function calls
void free_fulkerson(struct fulkerson_info f_info);// free memory associated with fulkerson structure
struct fulkerson_info ford_fulkerson(struct graph* g,void* source_d,void* sink_d);// run the folkerson algorithm from source to sink
int graph_add_edge(struct graph* g,void* source,void* sink,int weight); //add edge to the graph
int graph_add(struct graph* g,void* object);//add a node to the graph
void copy_graph(struct graph* g,struct graph* g2);//copy graph g to graph g2
struct graph* new_graph();//create a new graph
void free_graph(struct graph* g);//free graph memory
struct node* find_node(struct graph* g,void* object);//find graph node
struct edge* graph_remove_edge(struct graph* g,void* source,void* sink);//remove edge from graph, returns a pointer to be freed
struct edge* graph_find_edge(struct graph* g,void* source,void* sink);//find an edge on the graph
int graph_dump_node(struct graph* g,void* node);//remove node from graph and free memory


//DFS calls
struct DFS_path* DFS(struct graph* g,struct node* start,struct node* dest);//find path from start to dest

void free_DFS_path(struct DFS_path* path);//free DFS path without descending nodes, to be used when no DFS copy was created
void free_DFS_path_full(struct DFS_path* path); // free DFS path only when copy is created with copy_dfs
void copy_dfs(struct DFS_path* path,struct DFS_path** n_path);//make a copy of the DFS structure including node and path copy
int find_bottleneck(struct DFS_path* path);//find the bottleneck of current path



#endif
