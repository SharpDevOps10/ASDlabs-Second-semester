#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
  int data;
  struct Node *next;
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

void rearrangeList(Node** headRef, int n) {
  Node* head = *headRef;
  Node* mid = head;
  Node* prev = NULL;
  for (int i = 0; i < n; i++) {
    prev = mid;
    mid = mid->next;
  }
  prev->next = NULL;
  Node* current1 = head;
  Node* current2 = mid;
  Node* next1, * next2;
  while (current1  && current2) {
    next1 = current1->next;
    next2 = current2->next;
    current2->next = next1;
    current1->next = current2;
    current1 = next1;
    current2 = next2;
  }
  *headRef = head;
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
  int n, data;
  Node *head = NULL;
  printf("Enter the value of n: ");
  scanf("%d", &n);
  for (int i = 0; i < 2 * n; i++) {
    printf(" Enter a value for node %d : ", 2*n - i);
    scanf("%d", &data);
    head = insertNode(head, data);
  }
  printf("Original list: ");
  printList(head);
  rearrangeList(&head, n);
  printf("Rearranged list: ");
  printList(head);
  freeList(head);
  system("pause");
  return 0;
}
