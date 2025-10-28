#include "ford_fulkerson.h"
#include<stdio.h>

void print_DFS_path(struct DFS_path* path){
    while(path){
        if(path->conn)
            printf("node: %s to %s weight %d\n",(char*)path->from->descriptor,(char*)((struct node*)path->conn->pointer)->descriptor,path->conn->weight);
        path = path->next;

    }
}

void print_fulkerson(struct fulkerson_info f_info){
    struct fulkerson_paths* path = f_info.paths;
    printf("fulkerson run info, max flow: %d\n",f_info.max_flow);
    while(path){
        printf("path:\n");
        print_DFS_path(path->path);
        path = path->next;
    }

}
struct trackable_object{
    char* name;
    int max;
    int cur;
};
typedef struct trackable_object TO;

int main() {
    // Create source and sink
    TO source = {"SOURCE", 0, 0};
    TO sink = {"SINK", 0, 0};

    // Engineers
    TO s1 = {"s1",1,0};
    TO s2 = {"s2",1,0};
    TO s3 = {"s3",2,0};
    TO s4 = {"s4",1,0};
    TO s5 = {"s5",1,0};

    // Projects
    TO p1 = {"p1",1,0};
    TO p2 = {"p2",2,0};
    TO p3 = {"p3",1,0};
    TO p4 = {"p4",1,0};
    TO p5 = {"p5",1,0};


    struct graph* g = new_graph();

    // Add all nodes to graph
    graph_add(g, &source);
    graph_add(g, &sink);
    graph_add(g, &s1);
    graph_add(g, &s2);
    graph_add(g, &s3);
    graph_add(g, &s4);
    graph_add(g, &s5);
    graph_add(g, &p1);
    graph_add(g, &p2);
    graph_add(g, &p3);
    graph_add(g, &p4);
    graph_add(g, &p5);

    // Connect SOURCE to engineers with capacity based on assignment limits
    graph_add_edge(g, &source, &s1, s1.max);  // s1 can work on 1 project
    graph_add_edge(g, &source, &s2, s2.max);  // s2 can work on 1 project
    graph_add_edge(g, &source, &s3, s3.max);  // s3 can work on 2 projects
    graph_add_edge(g, &source, &s4, s4.max);  // s4 can work on 1 project
    graph_add_edge(g, &source, &s5, s5.max);  // s5 can work on 1 project

    // Connect engineers to projects based on skill compatibility
    // s1 can work on p1, p2
    graph_add_edge(g, &s1, &p1, 1);
    graph_add_edge(g, &s1, &p2, 1);

    // s2 can work on p2
    graph_add_edge(g, &s2, &p2, 1);

    // s3 can work on p1, p3, p4
    graph_add_edge(g, &s3, &p1, 1);
    graph_add_edge(g, &s3, &p3, 1);
    graph_add_edge(g, &s3, &p4, 1);

    // s4 can work on p2, p4
    graph_add_edge(g, &s4, &p2, 1);
    graph_add_edge(g, &s4, &p4, 1);

    // s5 can work on p2, p5
    graph_add_edge(g, &s5, &p2, 1);
    graph_add_edge(g, &s5, &p5, 1);

    // Connect projects to SINK with capacity based on project needs
    graph_add_edge(g, &p1, &sink, p1.max);  // p1 needs 1 engineer
    graph_add_edge(g, &p2, &sink, p2.max);  // p2 needs 2 engineers
    graph_add_edge(g, &p3, &sink, p3.max);  // p3 needs 1 engineer
    graph_add_edge(g, &p4, &sink, p4.max);  // p4 needs 1 engineer
    graph_add_edge(g, &p5, &sink, p5.max);  // p5 needs 1 engineer

    // Run Ford-Fulkerson to find maximum assignments
    struct fulkerson_info f_info = ford_fulkerson(g, &source, &sink);

    printf("Maximum number of assignments: %d\n", f_info.max_flow);

    // Print the actual assignments by examining the flow
    printf("Assignments:\n");
    //print_fulkerson(f_info);

    struct fulkerson_paths* path = f_info.paths;
    while(path){
        struct DFS_path* path2 = path->path;
        while(path2){
            if(path2->conn && path2->from ){
                TO* pobj1 = path2->from->descriptor;
                if(pobj1->name[0] == 's'){

                    TO* pobj2 = ((struct node*)path2->conn->pointer)->descriptor;
                    if(pobj1->cur < pobj1->max && pobj2->cur < pobj2->max){
                        printf("%s -> %s\n",pobj1->name,pobj2->name);
                        pobj1->cur++;
                        pobj2->cur++;
                    }
                }
            }
            path2 = path2->next;

        }

        path = path->next;
    }


    free_fulkerson(f_info);
    free_graph(g);
    return 0;
}

