#include <stdlib.h>
#include <limits.h>
#include "ford_fulkerson.h"
//Evan Nikitin Tue Oct 28 2025


void cpcn_node(struct node* nd,struct edge* cn){
   struct edge* copy = malloc(sizeof(struct edge));
   copy->pointer = cn->pointer;
   copy->weight =  cn->weight;
   copy->original = cn->original;
   copy->next = nd->connections;
   copy->reversed = cn->reversed;
   nd->connections = copy;
}

void free_graph(struct graph* g){
    struct node* current = g->verts;
    struct node* next;
    while(current){
        next = current->tracker;
        struct edge* cn = current->connections;
        while(cn){
            struct edge* next_edge = cn->next;
            free(cn);
            cn = next_edge;
        }
        free(current);
        current = next;
    }
    free(g);

}
struct graph* new_graph(){
    struct graph* ngr = malloc(sizeof(struct graph));
    ngr->verts = 0;
    return ngr;
}
struct node* find_node(struct graph* g,void* object){
    struct node* n_top = g->verts;
    while(n_top){
        if(n_top->descriptor == object){
            return n_top;
        }
        n_top = n_top->tracker;
    }
    return 0;
}

void copy_graph(struct graph* g,struct graph* g2){
    struct node* current = g->verts;
    struct node* next;
    while(current){
        next = current->tracker;
        struct node* nnode = malloc(sizeof(struct node));
        nnode->connections = 0;
        nnode->descriptor = current->descriptor;
        nnode->tracker = g2->verts;
        g2->verts = nnode;
        struct edge* cn = current->connections;
        while(cn){
            struct edge* next_edge = cn->next;
            cpcn_node(nnode,cn);
            cn = next_edge;
        }
        current = next;
    }
    current = g2->verts;
    while(current){

        struct edge* cn = current->connections;
        while(cn){
            cn->pointer = find_node(g2,((struct node*)cn->pointer)->descriptor);
            cn = cn->next;
        }
        current = current->tracker;
    }


}


int graph_add(struct graph* g,void* object){
    if(find_node(g,object)){
        return 0;
    }
    struct node* nd = malloc(sizeof(struct node));
    nd->descriptor = object;
    nd->connections = NULL;
    nd->tracker = g->verts;
    g->verts = nd;
    return 1;
}

struct edge* graph_find_edge(struct graph* g,void* source,void* sink){
 struct node* n_first = find_node(g,source);
 struct node* n_second = find_node(g,sink);
 if(!n_first)
    return 0;
 if(!n_second)
    return 0;

 struct edge* elist = n_first->connections;
 while(elist){
     if(elist->pointer == n_second)
         return elist;
     elist = elist->next;
 }
 return 0;
}
struct edge* g_rm_next_edge_to_sink(struct graph* g,struct node* sink){
    struct node* nlist = g->verts;
    while(nlist){

        struct edge* elist = nlist->connections;
        struct edge* prev = NULL;
        while(elist){
            if(elist->pointer == sink){
                struct edge* next = elist->next;
                if(!prev)
                    nlist->connections = next;
                else
                    prev->next = next;

                return elist;

            }
            prev = elist;
            elist = elist->next;
        }

        nlist = nlist->tracker;
    }
    return 0;
}
int graph_dump_node(struct graph* g,void* node){
//first find all edges pointing to it

 struct node* n_second = find_node(g,node);
 if(!n_second)
     return 0;
 struct edge* el = g_rm_next_edge_to_sink(g,n_second);
 while(el){
    free(el);
    el = g_rm_next_edge_to_sink(g,n_second);
 }

 struct edge* elist = n_second->connections;
 while(elist){
     struct edge* next = elist->next;
     free(elist);
     elist = next;
 }
 struct node* nl = g->verts;
 struct node* prev = NULL;
 while(nl){
     if(nl == n_second){
         if(prev == NULL)
             g->verts = nl->tracker;
         else
             prev->tracker = nl->tracker;
         free(nl);
         return 1;
     }
     prev = nl;
     nl = nl->tracker;
 }
 return 0;
}

struct edge* graph_remove_edge(struct graph* g,void* source,void* sink){
    struct node* targ = find_node(g,source);
    struct node* sink_t = find_node(g,sink);
    struct edge* elist = targ->connections;
    struct edge* prev = NULL;
    while(elist){
        if(elist->pointer == sink_t){
            struct edge* next = elist->next;
            if(!prev)
                targ->connections = next;
            else
                prev->next = next;

            return elist;
        }
        prev = elist;
        elist = elist->next;
    }
    return 0;

}

struct edge* graph_reverse_edge_direction(struct graph* g,struct node* source,struct node* sink){

 struct edge* elist = source->connections;
 struct edge* prev = NULL;
 while(elist){
     if(elist->pointer == sink){
        elist->pointer = source;
        cpcn_node(sink,elist);
        struct edge* next = elist->next;
        if(!prev)
            source->connections = next;
        else
            prev->next = next;
        return elist;
     }
     prev = elist;
     elist = elist->next;
 }
    return 0;
}

int graph_add_edge(struct graph* g,void* source,void* sink,int weight){
    struct node* n_first = find_node(g,source);
    struct node* n_dest = find_node(g,sink);
    if(!n_first)
        return 0;
    if(!n_dest)
        return 0;
    if(graph_find_edge(g,source,sink))
        return 0;
    struct edge* ed = malloc(sizeof(struct edge));
    ed->next = n_first->connections;
    ed->pointer = n_dest;
    ed->weight = weight;
    ed->original = weight;
    ed->reversed = 0;
    n_first->connections = ed;
    return 1;
}



struct node* bfs_visited(struct DFS* bfs_struct,struct node* nd){
    while(bfs_struct){
        if(bfs_struct->visted == nd)
            return nd;
        bfs_struct = bfs_struct->next;
    }
    return 0;
}

struct DFS_path* DFS_rec(struct DFS** bfs_struct,struct DFS_path* path_struct,struct node* start,struct node* dest){

    if(bfs_visited(*bfs_struct,start))
        return 0;
    if(start ==  dest){
        struct DFS_path* new_path = malloc(sizeof(struct DFS_path));
        new_path->from = start;
        new_path->next = path_struct;
        new_path->conn = 0;
        return new_path;
    }

    //printf("DFS: %s\n",(char*)start->descriptor);
    struct DFS* new_bfs = malloc(sizeof(struct DFS));
    new_bfs->visted = start;
    new_bfs->next = *bfs_struct;
    *bfs_struct = new_bfs;

    //now list the node connections and prepare the path structure
    struct DFS_path* new_path = malloc(sizeof(struct DFS_path));
    new_path->from = start;
    new_path->next = path_struct;

    struct edge* ned = start->connections;
    while(ned){
        new_path->conn = ned;
        if(ned->pointer == start){
            return new_path;
        }
        //printf("node: %s -> %s\n",(char*)start->descriptor,(char*)((struct node*)ned->pointer)->descriptor);

        struct DFS_path* next = DFS_rec(bfs_struct,new_path,ned->pointer,dest);
        if(next)
            return next;

        ned = ned->next;
    }
    free(new_path);
    return NULL;
}

struct DFS_path* DFS(struct graph* g,struct node* start,struct node* dest){
    struct DFS* bfs_struct = 0;
    struct DFS_path* ret = DFS_rec(&bfs_struct,0,start,dest);
    while(bfs_struct){
        struct DFS* next = bfs_struct->next;
        free(bfs_struct);
        bfs_struct = next;
    }
    return ret;
}
void free_DFS_path(struct DFS_path* path){
    while(path){
        struct DFS_path* next = path->next;
        free(path);
        path = next;
    }
}
void free_DFS_path_full(struct DFS_path* path){
    while(path){
        struct DFS_path* next = path->next;
        free(path->conn);
        free(path->from);
        free(path);
        path = next;
    }
}

int find_bottleneck(struct DFS_path* path){

    int max = INT_MAX;
    while(path){
        struct DFS_path* next = path->next;
        if(path->conn){
            if(path->conn->weight < max)
                max = path->conn->weight;
        }
        path = next;
    }
    return max;

}
void fulkerson_update(struct graph* original,struct graph* g,struct DFS_path* path,int bottleneck){
    while(path){
        if(path->conn){
            path->conn->weight -= bottleneck;

            if(path->conn->weight <= 0){
                struct node* cndest = path->conn->pointer;
                struct node* source = path->from;
                //printf("path:%d %s\n",path->conn->weight,(char*)cndest->descriptor);
                path->conn->reversed = ~path->conn->reversed;

                path->conn->weight = path->conn->original;
                //printf("path:%d\n",path->conn->weight);
                struct edge* rev = graph_reverse_edge_direction(g,source,cndest);
                free(rev);


            }
        }
        struct DFS_path* next = path->next;
        path = next;
    }
}



struct node* find_node_bfs(struct DFS_path* path,struct node* nd){
    while(path){
        if(path->from->descriptor == nd->descriptor)
            return path->from;
        path = path->next;
    }
    return NULL;
}

void copy_dfs(struct DFS_path* path,struct DFS_path** n_path){
    while(path){
        struct DFS_path* new_path = malloc(sizeof(struct DFS_path));
        struct node* nd_cp = malloc(sizeof(struct node));
        nd_cp->connections = 0;
        nd_cp->descriptor = path->from->descriptor;
        nd_cp->tracker = 0;

        new_path->from = nd_cp;
        new_path->conn = path->conn;
        new_path->next = *n_path;
        *n_path = new_path;

        path = path->next;
    }

    struct DFS_path* bcp = *n_path;
    while(bcp){
        struct edge* conn = bcp->conn;
        if(conn){
            struct node* bfs_node = find_node_bfs(*n_path,conn->pointer);
            struct edge* nnc = malloc(sizeof(struct edge));

            /*if(!bfs_node)
                printf("DFS copy failed dest:%s\n",(char*)((struct node*)(conn->pointer))->descriptor);
            */
            nnc->pointer = bfs_node;
            nnc->weight = conn->weight;
            nnc->original = conn->original;
            nnc->reversed = conn->reversed;
            nnc->next = 0;
            bcp->conn = nnc;
        }
        struct DFS_path* next = bcp->next;
        bcp = next;
    }

}

void free_fulkerson(struct fulkerson_info f_info){
    while(f_info.paths){
        struct fulkerson_paths* next = f_info.paths->next;
        free_DFS_path_full(f_info.paths->path);
        free(f_info.paths);
        f_info.paths = next;
    }
    free_graph(f_info.rem_graph);

}

struct fulkerson_info ford_fulkerson(struct graph* g,void* source_d,void* sink_d){



    struct fulkerson_info f_info;
    f_info.paths = 0;
    int max_flow = 0;
    struct graph* copy = new_graph();
    copy_graph(g,copy);
    f_info.rem_graph = copy;

    struct node* source = find_node(copy,source_d);
    struct node* sink = find_node(copy,sink_d);

    struct DFS_path* path = DFS(copy,source,sink);
    //printf("bfs path\n");
    while(path){

        int flow =find_bottleneck(path);
        //printf("max flow path:%d\n",flow);

        struct fulkerson_paths* n_path = malloc(sizeof(struct fulkerson_paths));
        n_path->path = NULL;
        n_path->limit = flow;
        n_path->next = f_info.paths;
        f_info.paths = n_path;

        copy_dfs(path,&(n_path->path));
        //printf("copy bfs\n");

        fulkerson_update(g,copy,path,flow);
        //printf("fulkerson update\n");
        max_flow += flow;
        free_DFS_path(path);
        path = DFS(copy,source,sink);
    }

    f_info.max_flow = max_flow;
    return f_info;
}


