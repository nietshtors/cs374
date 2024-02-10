// llist.h

#ifndef LLIST_H
#define LLIST_H

typedef struct node {
    int value;
    struct node *next;
} node;

void llist_insert_head(node **head, node *n);
node *llist_delete_head(node **head);
void llist_insert_tail(node **head, node *n);
void llist_print(node *head);
void llist_free(node **head);

int llist_length(node *head);
void llist_print_length(node *head);
node *llist_delete_value(node **head, int value);

node *node_alloc(int value);
void node_free(node *n);

void validate(int argc, char *argv[]);

#endif