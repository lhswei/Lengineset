#include <stdlib.h>
#include <stdio.h>
#include <memory>

// g++ -c main.cpp
// g++ -o main main.o

struct Node
{
    int data;
    Node *next;
};

int initlist(Node *header, int count)
{
    if (!header)
    {
        return 0;
    }

    Node *temp = header;

    for (int i = 0; i < count; ++i)
    {
        Node *p = new Node();
        if (p)
        {
            p->data = i;
            temp->next = p;
            temp = p;
        }
    }

    return 1;
}

void printlist(Node *header)
{
    if (!header)
    {
        printf("list is empty!\n");
        return;
    }

    Node *temp = header->next;
    while(temp)
    {
        printf("%d\t", temp->data);
        temp = temp->next;
    }
    printf("\n");
}


int reverlist(Node *header)
{
    if (!header)
    {
        return 0;
    }

    Node *node = header->next;
    header->next = NULL;

    while (node)
    {
        Node *next = node->next;
        Node *temp = header->next;
        header->next = node;
        node->next = temp;
        node  = next;
    }


    return 1;
}

void freelist(Node *header)
{
    Node *per = header;
    Node *next;
    while(per)
    {
        next = per->next;
        delete per;
        per = next;
    }
    header = NULL;
}

int main(int argc, char *argv[])
{
    Node *header = new Node;
    header->data = -1;
    initlist(header, 5);
    printlist(header);
    reverlist(header);
    printlist(header);
    freelist(header);
    return 0;
}

