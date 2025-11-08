#include "ford_fulkerson.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void* args[4];

int get_args(char* input) {
    int len = strlen(input);
    int arg_count = 0;
    int start = -1;

    // Initialize args to NULL
    for (int i = 0; i < 4; i++) {
        args[i] = NULL;
    }

    for (int i = 0; i <= len; i++) {
        if (input[i] != ' ' && input[i] != '\0') {
            if (start == -1) {
                start = i;
            }
        } else {
            if (start != -1) {
                int arg_len = i - start;
                args[arg_count] = malloc(arg_len + 1);
                strncpy((char*)args[arg_count], input + start, arg_len);
                ((char*)args[arg_count])[arg_len] = '\0';
                arg_count++;
                start = -1;
                if (arg_count >= 4) {
                    break;
                }
            }
        }
    }
    return arg_count;
}

struct trackable_object{
    char* name;
    int max;
    int cur;
    struct trackable_object* next;
};

void print_DFS_path(struct DFS_path* path){
    while(path){
        if(path->conn){
            struct trackable_object* fr = path->from->descriptor;
            struct trackable_object* to = ((struct node*)(path->conn->pointer))->descriptor;
            printf("node: %s to %s weight %d\n",fr->name,to->name,path->conn->weight);
        }
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

typedef struct trackable_object TO;

TO* nsrc = NULL;
TO* nsink = NULL;

TO* list = NULL;
TO* find_TO(char* name){
    TO* lcp = list;
    while(lcp){
        if(strcmp(lcp->name,name) == 0)
            return lcp;
        lcp = lcp->next;
    }

    return 0;
}
int add_TO(char* name,struct graph* g){
    if(find_TO(name))
        return 0;
    TO* entry = malloc(sizeof(TO));
    entry->name = malloc(sizeof(char)*(strlen(name)+1));
    memmove(entry->name,name,strlen(name)+1);
    entry->next = list;
    list = entry;
    graph_add(g,entry);
    return 1;
}

int remove_TO(char* name,struct graph* g){
    TO* lcp = list;
    TO* prev = 0;
    while(lcp){
        if(strcmp(lcp->name,name) == 0){
            graph_dump_node(g,lcp);
            if(!prev)
                list = lcp->next;
            else
                prev->next = lcp->next;
            if(nsink == lcp)
                nsink = 0;
            if(nsrc == lcp)
                nsrc = 0;
            free(lcp->name);
            free(lcp);
            return 1;
        }
        prev = lcp;
        lcp = lcp->next;
    }
    return 0;
}
void free_system(struct graph* g){
    free_graph(g);
    TO* lcp = list;
    TO* tmp;
    while(lcp){
        tmp = lcp->next;
        free(lcp->name);
        free(lcp);
        lcp = tmp;
    }
    nsink = 0;
    nsrc = 0;
    list = 0;
}

int rm_edge(struct graph* g, char* src1,char* src2){
  TO* s1 = find_TO(src1);
  TO* s2 = find_TO(src2);
    if(!s1)
        return 0;
    if(!s2)
        return 0;
    struct  edge* ept = graph_remove_edge(g,s1,s2);
    if(!ept)
        return 0;
    free(ept);
    return 1;

}

int add_edge(struct graph* g,char* src1,char* src2,int weight){
    TO* s1 = find_TO(src1);
    TO* s2 = find_TO(src2);
    if(!s1)
        return 0;
    if(!s2)
        return 0;
    return graph_add_edge(g,s1,s2,weight);
}

int set_src(char* src){
    TO* s =find_TO(src);
    if(!s)
        return 0;
    nsrc = s;
    return 1;

}
int set_sink(char* snk){
    TO* s =find_TO(snk);
    if(!s)
        return 0;
    nsink = s;
    return 1;

}

void run_fulkerson(struct graph* g){
    if(!nsrc){
        printf("source node not set\n");
        return;
    }
    if(!nsink){
        printf("sink node not set\n");
        return;
    }
    struct fulkerson_info f_info = ford_fulkerson(g,nsrc,nsink);
    print_fulkerson(f_info);
    printf("maxflow: %d\n", f_info.max_flow);
    free_fulkerson(f_info);
}
void list_TO(){
    TO* lcp = list;
    while(lcp){
        printf("%s\n",lcp->name);
        lcp = lcp->next;
    }


}

char line[4096];
void read_line(){
    char c;
    int stp =0;
    while((c = getchar())!='\n'){
        line[stp] = c;
        stp++;
    }
    line[stp] = 0;
}

int ncmd = 10;
char* commands[]={
    "node",
    "cler",
    "adde",
    "rmnd",
    "flkr",
    "help",
    "list",
    "nsrc",
    "nsnk",
    "rmed"
};
char* hint[]={
    "node grnode (expects 1 string)",
    "cler -> clears everything",
    "adde grnode1 grnode2 weight (expects 2 string and 1 int)",
    "rmnd grnode1 (expects 1 string)",
    "flkr -> run fulkerson",
    "help -> print this screen",
    "list -> print the nodes",
    "nsrc grnode1 (expects 1 string)",
    "nsnk grnode1 (expects 1 string)",
    "rmed grnode1 grnode2 (expects 2 strings)"
};
int get_cmd_id(char* input){
    int len = strlen(input);
    for(int i = 0;i<ncmd;i++){
        if(strcmp(input,commands[i]) == 0)
            return i;
    }
    return -1;
}
void help(){
    printf("commands: \n");
    for(int i = 0;i<ncmd;i++){
        printf("%s | %s\n",commands[i],hint[i]);
    }
    printf("exit | exit\n");
}

int main() {


    struct graph* g = new_graph();

    while(strcmp(line,"exit")!=0){
        printf("$ ");
        read_line();
        int argc = get_args(line);
        int cmd = get_cmd_id(args[0]);
        int ec;
        switch(cmd){
            case 0:
                if(argc !=2){
                    printf("bad args\n");
                    break;
                }
                if(!add_TO(args[1],g))
                    printf("failed to add node\n");
                break;
            case 1:
                free_system(g);
                printf("graphs cleared\n");
                g = new_graph();

                break;
            case 2:
                if(argc !=4){
                    printf("bad args\n");
                    break;
                }

                ec = atoi(args[3]);
                if(!add_edge(g,args[1],args[2],ec))
                        printf("failed to add edge\n");
                else
                    printf("added edge %s -> %s | %d\n",(char*)args[1],(char*)args[2],ec);
                break;
            case 3:
                if(argc !=2){
                    printf("bad args\n");
                    break;
                }

                if(! remove_TO(args[1],g))
                        printf("node removal failed\n");
                break;
            case 4:
                run_fulkerson(g);
                break;


            case 5:
                help();
                break;

            case 6:
                list_TO();
                break;
            case 7:
                if(argc !=2){
                    printf("bad args\n");
                    break;
                }

                if(!set_src(args[1]))
                    printf("failed to set source node\n");
                break;
            case 8:
                if(argc !=2){
                    printf("bad args\n");
                    break;
                }

                if(!set_sink(args[1]))
                        printf("failed to set sink node\n");
                break;
            case 9:
                if(argc !=3){
                    printf("bad args\n");
                    break;
                }

                if(!rm_edge(g,args[1],args[2]))
                        printf("failed to remove edge\n");
                else
                    printf("removed edge %s -> %s\n",(char*)args[1],(char*)args[2]);
                break;
        }
        for(int i = 0;i<argc;i++){
            free(args[i]);
        }

        if(cmd ==  -1 && strcmp(line,"exit")!=0)
            printf("unknown command\n");

    }

    free_system(g);
    return 0;
}

