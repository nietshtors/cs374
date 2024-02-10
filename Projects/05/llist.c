#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Insert node n at the head of the list. The node should have been 
 * previously allocated with node_alloc(). */
void llist_insert_head(node **head, node *n) {
    if (*head) {
        n->next = *head;
    }
    *head = n;
}

/* Remove a node from the head of the list and return a pointer to it. 
 * Return NULL if the list is empty. This function does not free the 
 * node; it merely uncouples it from the list. */
node *llist_delete_head(node **head) {
    if (*head == NULL) {
        return NULL;
    }
    node *n = *head;
    *head = (**head).next;
    return n;
}

/* Insert node n at the tail of the list. The node should have been 
 * previously allocated with node_alloc(). */
void llist_insert_tail(node **head, node *n) {
    if (*head == NULL) {
        *head = n;
    }
    else {
        node *nd = *head;
        while (nd->next) {
            nd = nd->next;
        }
        nd->next = n;
    }
}

/* Print a list to standard output in this form:
 *      1 -> 12 -> 3490 -> 7
 * with no trailing or leading spaces on the line. If the list is 
 * empty, the string [empty] (including square brackets) should 
 * be printed. */
void llist_print(node *head) {
    if (head == NULL) {
        printf("[empty]\n");
    }
    else {
        node *n = head;
        while (1) {
            printf("%d", n->value);
            if (n->next) {
                printf(" -> ");
                n = n->next;
            }
            else {break;}
        }
        printf("\n");
    }
}

/* Call node_free() for every node in the list. head will be set to 
 * NULL. */
void llist_free(node **head) {
    if (*head) {
        node *n1 = *head;
        node *n2;

        while (n1->next) {
            n2 = n1->next;
            node_free(n1);
            n1 = n2;
        }

        *head = NULL;
    }
}

/* Returns the length of the list */
int llist_length(node *head) {
    int length = 0;
    node *n = head;
    if (n) {
        while (n->next) {
            n = n->next;
            length++;
        }
    }
    return length;
}

/* Print the length of list to standard output */
void llist_print_length(node *head) {
    printf("Length: %d\n", llist_length(head));
}

/* Remove the first node with the value from the list and return a 
 * pointer to it. Return NULL if the list is empty or if the value 
 * is not found. This function does not free the node; it merely 
 * uncouples it from the list. */
node *llist_delete_value(node **head, int value) {
    node *n = *head;
    node *nprev = NULL;
    while (n) {
        if (n->value == value) {
            if (!nprev) {
                return llist_delete_head(head); 
            }
            nprev->next = n->next;
            return n;
        }
        nprev = n;
        n = n->next;
    }
    return NULL;
}

/* Allocate a new struct node with value () and a next of NULL. */
node *node_alloc(int value) {
    node *n = malloc(sizeof *n);
    n->value = value;
    n->next = NULL;
    return n;
}

/* Free a node previously allocated with node_alloc(). */
void node_free(node *n) {
    free(n);
}

/* Validate input commands */
void validate(int argc, char *argv[]) {
    int cmdsc = 6;
    char *cmds[] = {"ih", "it", "dv", "dh", "f", "p", "lp", NULL};
    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < cmdsc + 1; j++) {
            if (!strcmp(argv[i], cmds[j])) {
                if (j < 3) {i++;}
                break;
            }
            else if (j == cmdsc) {
                printf("Argument Error: Command '%s' is not valid\n", argv[i]);
                exit(1);
            }
        }
    }
}


int main(int argc, char *argv[]) {
    validate(argc, argv);

    node *head = NULL; // Linked list

    char *end;
    int num;
    for (int i = 1; i < argc; i++) {
        // Insert head
        if (!strcmp(argv[i], "ih")) {
            num = strtol(argv[++i], &end, 10);
            node *n = node_alloc(num);
            llist_insert_head(&head, n);
        }
        // Insert tail
        else if (!strcmp(argv[i], "it")) {
            num = strtol(argv[++i], &end, 10);
            node *n = node_alloc(num);
            llist_insert_tail(&head, n);
        }
        // Delete value
        else if (!strcmp(argv[i], "dv")) {
            num = strtol(argv[++i], &end, 10);
            node_free(llist_delete_value(&head, num));
        }
        // Delete head
        else if (!strcmp(argv[i], "dh")) {
            node_free(llist_delete_head(&head));
        }
        // Free list
        else if (!strcmp(argv[i], "f")) {
            llist_free(&head);
        }
        // Print list
        else if (!strcmp(argv[i], "p")) {
            llist_print(head);
        }
        // Print length of list
        else if (!strcmp(argv[i], "lp")) {
            llist_print(head);
        }
        else {
            printf("Error in commands. This shouldn't print.\n");
        }
    }
    

}