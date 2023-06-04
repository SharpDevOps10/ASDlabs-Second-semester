#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#define vertices 12
#define IDC_BUTTON 1
#define IDC_BUTTON2 2
#include "props.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[] = "Lab 6";

struct coordinates {
  double nx[vertices];
  double ny[vertices];
  double loopX[vertices];
  double loopY[vertices];
};

struct vertexNode {
  double edgeWeight;
  struct vertexNode* nextNode;
  int vertexIndex;
};

struct newEmpGraph {
  struct vertexNode* head;
  int vertexIndex;
  struct newEmpGraph* upcomingVertex;
};




void releaseMemory(struct newEmpGraph* pNewEmpGraph) {
  while (pNewEmpGraph != NULL)
  {
    struct vertexNode* pVertexNode = pNewEmpGraph->head;
    while (pVertexNode != NULL)
    {
      struct vertexNode* next_node = pVertexNode->nextNode;
      free(pVertexNode);
      pVertexNode = next_node;
    }
    struct newEmpGraph* nextVertex = pNewEmpGraph->upcomingVertex;
    free(pNewEmpGraph);
    pNewEmpGraph = nextVertex;
  }
}


struct newEmpGraph* discoverVertex(struct newEmpGraph* pEmpGraph, int desiredIndex) {
  struct newEmpGraph* target = pEmpGraph;
  while (target->vertexIndex != desiredIndex) {
    target = target->upcomingVertex;
  }
  return target;
}


struct newEmpGraph* generateNewGraph(double** adjacencyMatrix, double** weightMatrix, int amountOfEdges) {
  struct newEmpGraph* pNewEmpGraph = (struct newEmpGraph*)malloc(sizeof(struct newEmpGraph));
  pNewEmpGraph->head = NULL;
  pNewEmpGraph->upcomingVertex = NULL;
  struct newEmpGraph* ongoing = pNewEmpGraph;

  for (int i = 0; i < vertices - 1; i++) {
    struct newEmpGraph* upcoming = (struct newEmpGraph*)malloc(sizeof(struct newEmpGraph));
    upcoming->head = NULL;
    upcoming->upcomingVertex = NULL;
    ongoing->upcomingVertex = upcoming;
    ongoing->vertexIndex = i;
    ongoing = ongoing->upcomingVertex;
  }
  ongoing->vertexIndex = vertices - 1;

  for (int i = 0; i < amountOfEdges; i++) {
    int source = -1;
    int terminus = -1;
    double weight = -1;
    getEdge(i, adjacencyMatrix, weightMatrix, &source, &terminus, &weight);

    struct newEmpGraph* ongoingVertex = pNewEmpGraph;
    ongoingVertex = discoverVertex(ongoingVertex, source);

    struct vertexNode* newVertexNode = (struct vertexNode*)malloc(sizeof(struct vertexNode));
    newVertexNode->vertexIndex = terminus;
    newVertexNode->edgeWeight = weight;
    newVertexNode->nextNode = ongoingVertex->head;
    ongoingVertex->head = newVertexNode;

    if (source != terminus) {
      struct newEmpGraph* terminusVertex = pNewEmpGraph;
      terminusVertex = discoverVertex(terminusVertex, terminus);

      newVertexNode = (struct vertexNode*)malloc(sizeof(struct vertexNode));
      newVertexNode->vertexIndex = source;
      newVertexNode->edgeWeight = weight;
      newVertexNode->nextNode = terminusVertex->head;
      terminusVertex->head = newVertexNode;
      terminusVertex = terminusVertex->upcomingVertex;
    }
  }
  return pNewEmpGraph;
}

void updateVisitedEdges(int visitedEdges[][vertices], int source, int terminus, HDC hdc, struct coordinates coordinates, struct vertexNode* pVertexNode, int radiusOfVertex, int dtx)
{
  if (visitedEdges[source][terminus] == 0 && visitedEdges[terminus][source] == 0)
  {
    visitedEdges[source][terminus] = 1;
    visitedEdges[terminus][source] = 1;

    LineTo(hdc, coordinates.nx[terminus], coordinates.ny[terminus]);
    SetDCBrushColor(hdc, RGB(155, 180, 250));
    HPEN linePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    char weight[4];
    _itoa_s(pVertexNode->edgeWeight, weight, sizeof(weight), 10);

    RECT textRect = {0};
    DrawTextA(hdc, weight, strlen(weight), &textRect, DT_CALCRECT);

    int textWidth = textRect.right - textRect.left;
    int textHeight = textRect.bottom - textRect.top;

    int srcX = coordinates.nx[source];
    int srcY = coordinates.ny[source];

    int destX = coordinates.nx[terminus];
    int destY = coordinates.ny[terminus];

    double differenceX = destX - srcX;
    double differenceY = destY - srcY;

    double centerX = srcX + differenceX / 3.0;
    double centerY = srcY + differenceY / 3.0;

    int offsetX = -textWidth / 2;
    int offsetY = -textHeight / 2;

    TextOutA(hdc, (int)centerX + offsetX, (int)centerY + offsetY, weight, strlen(weight));
    SelectObject(hdc, linePen);
  }
}



void depictUndirectedGraph(int centerX, int centerY, int radiusOfGraph, int radiusOfVertex, int radiusOfLoop, int dtx, struct newEmpGraph* newEmpGraph, struct coordinates coordinates,
                           HPEN PPen, HPEN BPen, HDC hdc)
{
  static int visitedEdges[vertices][vertices] = {0};

  while (newEmpGraph != NULL)
  {
    struct vertexNode* node = newEmpGraph->head;
    while (node != NULL)
    {
      int source = newEmpGraph->vertexIndex;
      int terminus = node->vertexIndex;
      MoveToEx(hdc, coordinates.nx[source], coordinates.ny[source], NULL);

      HPEN ChangePen = CreatePen(PS_SOLID, 2, RGB(155, 180, 250));
      SelectObject(hdc, ChangePen);
      if (source == terminus)
      {

        SelectObject(hdc, PPen);
        Ellipse(hdc, coordinates.loopX[source] - radiusOfLoop, coordinates.loopY[source] - radiusOfLoop, coordinates.loopX[source] + radiusOfLoop, coordinates.loopY[source] + radiusOfLoop);
        SelectObject(hdc, ChangePen);
      }
      else
      {
        updateVisitedEdges(visitedEdges, source, terminus, hdc, coordinates, node, radiusOfVertex, dtx);
      }
      node = node->nextNode;
    }
    newEmpGraph = newEmpGraph->upcomingVertex;
  }

  SelectObject(hdc, BPen);
  SelectObject(hdc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hdc, RGB(204, 204, 255));
  for (int i = 0; i < vertices; i++) {
    Ellipse(hdc, coordinates.nx[i] - radiusOfVertex, coordinates.ny[i] - radiusOfVertex, coordinates.nx[i] + radiusOfVertex, coordinates.ny[i] + radiusOfVertex);
    wchar_t buffer[5];
    swprintf(buffer, 5, L"%d", i + 1);
    TextOut(hdc, coordinates.nx[i] - dtx, coordinates.ny[i] - radiusOfVertex / 2, buffer, 3);
  }
}

int checkChordExistence(struct newEmpGraph* sourceVertex, struct newEmpGraph* targetVertex, struct newEmpGraph* previousVertex) {
  struct vertexNode* currentNode = sourceVertex->head;
  while (currentNode != NULL) {
    if (currentNode == previousVertex) {
      currentNode = currentNode->nextNode;
      continue;
    }
    if (currentNode == targetVertex) return 1;
    if (checkChordExistence(currentNode->nextNode, targetVertex, sourceVertex) == 1) return 1;
    currentNode = currentNode->nextNode;
  }
  return 0;
}


struct vertexNode* addVisited(struct vertexNode* visited, int vertexIndex) {
  struct vertexNode* newNode = (struct vertexNode*)malloc(sizeof(struct vertexNode));
  newNode->vertexIndex = vertexIndex;
  newNode->nextNode = NULL;

  if (visited == NULL) {
    visited = newNode;
  } else {
    struct vertexNode* current = visited;
    while (current->nextNode != NULL) {
      current = current->nextNode;
    }
    current->nextNode = newNode;
  }

  return visited;
}


struct vertexNode* getMinEdge(struct newEmpGraph* g) {
  struct vertexNode* minEdge = NULL;
  double minWeight = __DBL_MAX__;
  struct newEmpGraph* curr = g;
  while (curr != NULL) {
    struct vertexNode* destNode = curr->head;
    int from = curr->vertexIndex;
    while (destNode != NULL) {
      int to = destNode->vertexIndex;
      if (destNode->edgeWeight < minWeight && from != to) {
        minEdge = destNode;
        minWeight = destNode->edgeWeight;
      }
      destNode = destNode->nextNode;
    }
    curr = curr->upcomingVertex;
  }
  return minEdge;
}


struct Element {
  int vertex;
  struct Element *next;
};

struct Edge {
  int terminus;
  int source;
  double weights;
};

struct Queue {
  struct Element *first;
  struct Element *last;
};

struct Queue* initializeQueue() {
  struct Queue* queue = malloc(sizeof(struct Queue));
  queue->first = NULL;
  queue->last = NULL;
  return queue;
}

void enqueue(struct Queue* structQueue, int vertex) {
  struct Element* pElement = malloc(sizeof(struct Element));
  pElement->vertex = vertex;
  pElement->next = NULL;

  struct Element* last = structQueue->last;

  if (!last) {
    structQueue->first = pElement;
    structQueue->last = pElement;
  } else {
    structQueue->last->next = pElement;
    structQueue->last = pElement;
  }
}

void dequeue(struct Queue* structQueue) {
  struct Element* pElement = structQueue->first;
  if (structQueue->first == structQueue->last) {
    structQueue->first = NULL;
    structQueue->last = NULL;
  } else {
    structQueue->first = structQueue->first->next;
  }
  free(pElement);
}

void freeQueue(struct Queue* structQueue) {
  while (structQueue->first) {
    dequeue(structQueue);
  }
  free(structQueue);
}

void freeVisited(struct vertexNode* visited) {
  struct vertexNode* curr = visited;
  while (curr != NULL) {
    struct vertexNode* temp = curr;
    curr = curr->nextNode;
    free(temp);
  }
}

int checkVisitedVertices(struct vertexNode* visited, int vertexIndex) {
  struct vertexNode* curr = visited;
  while (curr != NULL) {
    if (curr->vertexIndex == vertexIndex) {
      return 1;
    }
    curr = curr->nextNode;
  }
  return 0;
}


struct vertexNode* getVertex(struct newEmpGraph* empGraph, int num) {
  struct newEmpGraph* pEmpGraph = empGraph;
  while (pEmpGraph != NULL) {
    if (pEmpGraph->vertexIndex == num) {
      return pEmpGraph->head;
    }
    pEmpGraph = pEmpGraph->upcomingVertex;
  }
  return NULL;
}

void removeEdge(struct newEmpGraph* graph, int from, int to) {
  struct newEmpGraph* curr = graph;
  while (curr != NULL) {
    if (curr->vertexIndex == from) {
      struct vertexNode* prev = NULL;
      struct vertexNode* currNode = curr->head;
      while (currNode != NULL) {
        if (currNode->vertexIndex == to) {
          if (prev == NULL) {
            curr->head = currNode->nextNode;
          } else {
            prev->nextNode = currNode->nextNode;
          }
          free(currNode);
          return;
        }
        prev = currNode;
        currNode = currNode->nextNode;
      }
    }
    curr = curr->upcomingVertex;
  }
}

void addEdge(struct newEmpGraph** graph, int from, int to, double weight) {
  struct newEmpGraph* curr = *graph;
  struct vertexNode* newEdge = malloc(sizeof(struct vertexNode));
  newEdge->vertexIndex = to;
  newEdge->edgeWeight = weight;
  newEdge->nextNode = NULL;

  while (curr != NULL) {
    if (curr->vertexIndex == from) {
      if (curr->head == NULL) {
        curr->head = newEdge;
      } else {
        struct vertexNode* last = curr->head;
        while (last->nextNode != NULL) {
          last = last->nextNode;
        }
        last->nextNode = newEdge;
      }
      return;
    }
    curr = curr->upcomingVertex;
  }

  struct newEmpGraph* newVertex = malloc(sizeof(struct newEmpGraph));
  newVertex->vertexIndex = from;
  newVertex->head = newEdge;
  newVertex->upcomingVertex = *graph;
  *graph = newVertex;
}


int isChord(struct newEmpGraph* tree, int vertexIndex, int v) {
  struct vertexNode* visited = NULL;
  struct vertexNode* queue = NULL;

  visited = addVisited(visited, vertexIndex);
  enqueue(&queue, vertexIndex);

  while (queue != NULL) {
    struct vertexNode* curr = getVertex(tree, queue->vertexIndex);
    while (curr != NULL) {
      if (curr->vertexIndex == v) {
        freeQueue(&queue);
        freeVisited(&visited);
        return 1;
      } else if (!checkVisitedVertices(visited, curr->vertexIndex)) {
        visited = addVisited(visited, curr->vertexIndex);
        enqueue(&queue, curr->vertexIndex);
      }
      curr = curr->nextNode;
    }
    dequeue(&queue);
  }
  freeQueue(&queue);
  freeVisited(&visited);
  return 0;
}


struct newEmpGraph* KruskalMST(struct newEmpGraph* mainGraph) {
  int counter = 0;
  struct newEmpGraph* spanningTree = NULL;
  while (counter != vertices - 1) {
    struct vertexNode* minEdge = getMinEdge(mainGraph);
    removeEdge(mainGraph, minEdge->vertexIndex, minEdge->nextNode->vertexIndex);
    if (!isChord(spanningTree, minEdge->vertexIndex, minEdge->nextNode->vertexIndex)) {
      addEdge(&spanningTree, minEdge->vertexIndex, minEdge->nextNode->vertexIndex, minEdge->edgeWeight);
      counter++;
    }
  }
  releaseMemory(mainGraph);
  return spanningTree;
}

void depictKruskal(int tree, struct Edge* edge, struct coordinates coordinates, double radiusOfVertex, double dtx, HDC hdc) {
  for (int i = 0; i < tree; ++i) {
    int terminus = edge[i].terminus;
    int source = edge[i].source;

    double weights = edge[i].weights;
    MoveToEx(hdc, coordinates.nx[source], coordinates.ny[source], NULL);
    LineTo(hdc, coordinates.nx[terminus], coordinates.ny[terminus]);

    double x = fabs(coordinates.nx[source] + coordinates.nx[terminus]) / 2.;
    double y = fabs(coordinates.ny[source] + coordinates.ny[terminus]) / 2.;

    Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);
    wchar_t buffer[10];
    swprintf(buffer, 5, L"%lf", weights);
    TextOut(hdc, x - 15, y - 15, buffer, 4);

  }

  for (int i = 0; i < tree; i++)
  {
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(219, 56, 56));
    int dest = edge[i].terminus;
    Ellipse(hdc, coordinates.nx[dest] - radiusOfVertex, coordinates.ny[dest] - radiusOfVertex, coordinates.nx[dest] + radiusOfVertex, coordinates.ny[dest] + radiusOfVertex);
    wchar_t buffer2[5];
    swprintf(buffer2, 5, L"%d", dest + 1);
    TextOut(hdc, coordinates.nx[dest] - dtx, coordinates.ny[dest] - radiusOfVertex / 2, buffer2, 3);
  }
}


int kruskalCounter = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
  WNDCLASS w;
  w.lpszClassName = ProgName;
  w.hInstance = hInstance;
  w.lpfnWndProc = WndProc;
  w.hCursor = LoadCursor(NULL, IDC_ARROW);
  w.hIcon = 0;
  w.lpszMenuName = 0;
  w.hbrBackground = WHITE_BRUSH;
  w.style = CS_HREDRAW | CS_VREDRAW;
  w.cbClsExtra = 0;
  w.cbWndExtra = 0;
  if (!RegisterClass(&w)) {
    return 0;
  }
  HWND hWnd;
  MSG lpMsg;
  hWnd = CreateWindow(ProgName,
                      (LPCSTR) "Lab 6.  by Daniil Timofeev IM-22",
                      WS_OVERLAPPEDWINDOW,
                      100,
                      100,
                      1000,
                      700,
                      (HWND) NULL,
                      (HMENU) NULL,
                      (HINSTANCE) hInstance,
                      (HINSTANCE) NULL);
  ShowWindow(hWnd, nCmdShow);
  while (GetMessage(&lpMsg, hWnd, 0, 0)) {
    TranslateMessage(&lpMsg);
    DispatchMessage(&lpMsg);
  }
  return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
  HDC hdc;
  PAINTSTRUCT ps;
  HWND kruskalButton;
  int state = 0;
  switch (messg) {
    case WM_CREATE: {
      kruskalButton = CreateWindow(
        (LPCSTR) "BUTTON",
        (LPCSTR) "Next Step: Kruskal",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        700,
        30,
        160,
        50,
        hWnd,
        (HMENU) IDC_BUTTON,
        (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
      return 0;
    }
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {

        case IDC_BUTTON:
          state = 1;
          if (kruskalCounter < vertices) kruskalCounter++;
          InvalidateRect(hWnd, NULL, FALSE);
          break;

      }
    }
    case WM_PAINT :
      hdc = BeginPaint(hWnd, &ps);
      SetGraphicsMode(hdc, GM_ADVANCED);


      double** T = randm(vertices);
      double coefficient = 1.0 - 0.01 - 0.005 - 0.05;
      T = mulmr(coefficient, T, vertices);
      double** A = symmetricMatrix(T);

      double** R = randm(vertices);
      multiplyByCoefficientMatrix(R, A);
      double** Wt = roundm(R);

      double** bMatrix = createBMatrix(Wt);
      double** cMatrix = createCMatrix(bMatrix);
      double** dMatrix = createDMatrix(bMatrix);
      double** trMatrix = createTrMatrix();
      Wt = createWtMatrix(Wt, cMatrix, dMatrix, trMatrix);
      double** W = symmetricMatrix(Wt);

      HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
      HPEN PPen = CreatePen(PS_SOLID, 2, RGB(153, 0, 153));
      HPEN RPen = CreatePen(PS_SOLID, 4, RGB(219, 56, 56));
      HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

      SelectObject(hdc, NoPen);
      Rectangle(hdc, 0, 0, 670, 700);


      struct coordinates coordinates;
      double circleRadius = 280;
      double vertexRadius = circleRadius / 12;
      double loopRadius = vertexRadius;
      double dtx = vertexRadius / 2.5;
      double circleCenterX = 370;
      double circleCenterY = 340;
      double angleAlpha = 2.0 * M_PI / (double)vertices;
      for (int i = 0; i < vertices; i++)
      {
        double sinAlpha = sin(angleAlpha * (double)i);
        double cosAlpha = cos(angleAlpha * (double)i);
        coordinates.nx[i] = circleCenterX + circleRadius * sinAlpha;
        coordinates.ny[i] = circleCenterY - circleRadius * cosAlpha;
        coordinates.loopX[i] = circleCenterX + (circleRadius + loopRadius) * sinAlpha;
        coordinates.loopY[i] = circleCenterY - (circleRadius + loopRadius) * cosAlpha;
      }



      int amountOfEdges = countAmountOfEdges(A);
      struct newEmpGraph* pNewEmpGraph = generateNewGraph(A, W, amountOfEdges);

      SetBkMode(hdc, TRANSPARENT);
      depictUndirectedGraph(circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, dtx, pNewEmpGraph, coordinates, PPen, BPen, hdc);


      EndPaint(hWnd, &ps);

      freeMatrix(A, vertices);
      freeMatrix(T, vertices);
      freeMatrix(R, vertices);
      freeMatrix(Wt, vertices);
      freeMatrix(W, vertices);
      freeMatrix(bMatrix, vertices);
      freeMatrix(cMatrix, vertices);
      freeMatrix(dMatrix, vertices);
      freeMatrix(trMatrix, vertices);
      releaseMemory(pNewEmpGraph);
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return (DefWindowProc(hWnd, messg, wParam, lParam));
  }
  return 0;
}
