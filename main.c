#include <stdio.h>
#include <stdlib.h>


struct a_list_node{

    int info;

    struct a_list_node *next;
};

struct a_graph{

    int number_of_nodes;
    int is_initialized;
    int *adjacent_matrix;
    int *node_value;
};

int is_adjacent(struct a_graph *my_graph, int node1, int node2){
    if(*(my_graph->adjacent_matrix + node1*my_graph->number_of_nodes + node2)==1){
        return 1;
    }else{
        return 0;
    }
}

void print_graph(struct a_graph *my_graph){

    int iterator_rows;
    int iterator_columns;
    printf("\nAdjacent matrix of our graph :\n");
    for(iterator_rows=1; iterator_rows<=my_graph->number_of_nodes; iterator_rows++){
       for(iterator_columns=1; iterator_columns<=my_graph->number_of_nodes; iterator_columns++){
            printf("%d ", *(my_graph->adjacent_matrix + iterator_rows*my_graph->number_of_nodes + iterator_columns));
        }
        printf("\n");
    }
}

void read_graph(struct a_graph *my_graph){


    int iterator_rows;

    int iterator_columns;

    char input_option;
    printf("\nNumber of nodes ");
    printf("\n");

    scanf("%d", &my_graph->number_of_nodes);
    my_graph->is_initialized = 1;
    my_graph->adjacent_matrix = calloc(my_graph->number_of_nodes*my_graph->number_of_nodes, sizeof(*my_graph));

    for(iterator_rows = 1; iterator_rows <= my_graph->number_of_nodes; iterator_rows++){
        for(iterator_columns = iterator_rows + 1; iterator_columns <= my_graph->number_of_nodes; iterator_columns++){
            printf("Node %d is adjacent with node %d ? (Y/N) ", iterator_rows , iterator_columns  );
            scanf(" %c", &input_option);
            if(input_option == 'y' || input_option == 'Y'){
                *(my_graph->adjacent_matrix + iterator_rows*my_graph->number_of_nodes + iterator_columns)=1;
                *(my_graph->adjacent_matrix + iterator_columns*my_graph->number_of_nodes + iterator_rows)=1;
            }
        }
    }
}

void push_begining_list(struct a_list_node *head, int new_element_value){
    struct a_list_node *next_element;
    struct a_list_node *new_element;

    next_element = head->next;
    new_element = malloc(sizeof(struct a_list_node));
    new_element->info = new_element_value;
    new_element->next = head->next;
    head->next = new_element;
}

int pop_end_list(struct a_list_node *head) {
    struct a_list_node *deleted_element;
    struct a_list_node *iterator;
    int aux;

    if (head->next != NULL) {
        for (iterator = head; iterator->next->next != NULL; iterator = iterator->next);
        deleted_element = iterator->next;
        aux = deleted_element->info;
        iterator->next = deleted_element->next;
        free(deleted_element);
        return aux;
    } else {
        printf("\n The list is empty");
        return -1;
    }
}

void graph_bfs(struct a_graph *my_graph, int start_node, int *bfs_order_vector){
    struct a_list_node *head_queue;
    int *visited;
    int column_index;
    int aux;
    int current_node;
    int i=1;

    head_queue = calloc(1, sizeof(struct a_list_node));
    head_queue->next = NULL;
    head_queue->info = 0;
    visited = calloc((my_graph->number_of_nodes + 1), sizeof(int));
    push_begining_list(head_queue, start_node);
    visited[start_node] = 1;
    printf("\nBFS traversal: ");
    while (head_queue->next != NULL){
        current_node = pop_end_list(head_queue);
        printf("%d,",current_node);
        *(bfs_order_vector + i)=current_node;
        i++;
        for(column_index = 1; column_index <= my_graph->number_of_nodes; column_index++){
            aux = *(my_graph->adjacent_matrix + current_node * my_graph->number_of_nodes + column_index);
            if ( (aux != 0) && ( (visited[column_index]) == 0 )){
                push_begining_list(head_queue, column_index);
                visited[column_index] = 1;
            }
        }
    }

    free(head_queue);
    free(visited);
}

void find_color_greedy_alg(struct a_graph *my_graph){


    int i;
    int j;
    int *colors;
    colors=calloc(my_graph->number_of_nodes, sizeof(*colors));
    for(i=1;i<=my_graph->number_of_nodes;i++){
        *(colors + i)=1;
        for(j=1;j<=i;j++){
            if(is_adjacent(my_graph, i, j)==1 && *(colors + i)==*(colors + j)){
                *(colors + i)=*(colors + j)+1;
            }
        }
    }

    printf("\n");
    for(i=1;i<=my_graph->number_of_nodes;i++){
        printf("Node[%d] is colored with color %d \n",i , *(colors + i));
    }
}


void find_color_greedy_alg2(struct a_graph *my_graph, int *bfs_order_vector){


    int i;
    int j;
    int *colors;
    int maximum_number_of_colors=1;
    colors=calloc(my_graph->number_of_nodes, sizeof(*colors));
    for(i=1;i<=my_graph->number_of_nodes;i++){
        *(colors + i)=1;
        for(j=1;j<=i;j++){
            if(is_adjacent(my_graph, *(bfs_order_vector + i) , *(bfs_order_vector + j ))==1 && *(colors + i)==*(colors + j)){
                *(colors + i)=*(colors + j)+1;
                if((*(colors + j)+1) > maximum_number_of_colors){
                    maximum_number_of_colors=*(colors + j)+1;
                }
            }
        }
    }

    printf("\n");
    for(i=1;i<=my_graph->number_of_nodes;i++){
        printf("Node[%d] is colored with color %d \n",*(bfs_order_vector + i) , *(colors + i));
    }
    printf("\n");
    printf("Colors needed %d", maximum_number_of_colors);
}

int main(){

    struct a_graph *my_graph;
    int *bfs_order_vector;

    my_graph=calloc(1, sizeof(struct a_graph));
    my_graph->node_value = calloc(my_graph->number_of_nodes, sizeof(*my_graph));
    read_graph(my_graph);
    bfs_order_vector=(int*)calloc((my_graph->number_of_nodes+1), sizeof(*bfs_order_vector));

    graph_bfs(my_graph, 1, bfs_order_vector);

    print_graph(my_graph);

    find_color_greedy_alg2(my_graph, bfs_order_vector);

    free(my_graph->adjacent_matrix);
    free(my_graph->node_value);
    free(my_graph);
    free(bfs_order_vector);

    return 0;
}
