#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

Node* insertNode(Node* head, int data) {
  if (!head) {
    head = createNode(data);
  } else {
    Node* newNode = createNode(data);
    newNode->next = head;
    head = newNode;
  }
  return head;
}

void printList(Node* head) {
  Node* ptr = head;
  while (ptr) {
    printf("%d ", ptr->data);
    ptr = ptr->next;
  }
  printf("\n");
}

void rearrangeList(Node** headRef) {
  Node* head = *headRef;
  if (!head || !head->next)
    return;

  Node* slow = head;
  Node* fast = head->next;

  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }

  Node* current1 = head;
  Node* current2 = slow->next;
  slow->next = NULL;

  while (current2) {
    Node* temp1 = current1->next;
    Node* temp2 = current2->next;
    current1->next = current2;
    current2->next = temp1;
    current1 = temp1;
    current2 = temp2;
  }
}

void freeList(Node* head) {
  Node* temp = NULL;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}

int main() {
  int data;
  Node* head = NULL;
  printf("Enter the values for the nodes (enter -1 to stop): \n");
  while (1) {
    printf("Enter a value: ");
    scanf("%d", &data);
    if (data == -1)
      break;
    head = insertNode(head, data);
  }
  printf("Original list: ");
  printList(head);
  rearrangeList(&head);
  printf("Rearranged list: ");
  printList(head);
  freeList(head);
  return 0;
}
