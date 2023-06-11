#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define vertices 12
#define IDC_BUTTON 1

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
    struct vertexNode *nextNode;
    int vertexIndex;
};

struct newEmpGraph {
    struct vertexNode *head;
    int vertexIndex;
    struct newEmpGraph *upcomingVertex;
};

struct Edge {
    int terminus;
    int source;
    double weights;
};


void releaseMemory(struct newEmpGraph *pNewEmpGraph) {
  while (pNewEmpGraph != NULL) {
    struct vertexNode *pVertexNode = pNewEmpGraph->head;
    while (pVertexNode != NULL) {
      struct vertexNode *next_node = pVertexNode->nextNode;
      free(pVertexNode);
      pVertexNode = next_node;
    }
    struct newEmpGraph *nextVertex = pNewEmpGraph->upcomingVertex;
    free(pNewEmpGraph);
    pNewEmpGraph = nextVertex;
  }
}


struct newEmpGraph *discoverVertex(struct newEmpGraph *pEmpGraph, int desiredIndex) {
  struct newEmpGraph *target = pEmpGraph;
  while (target->vertexIndex != desiredIndex) {
    target = target->upcomingVertex;
  }
  return target;
}

struct vertexNode *discoverNode(struct vertexNode *nodeVertex, int indexOfNode) {
  while (nodeVertex->vertexIndex != indexOfNode) {
    nodeVertex = nodeVertex->nextNode;
  }
  return nodeVertex;
}

int getMinIndex(double *weights, int *visitedVertex) {
  double min = (double) INT_MAX;
  int minIndex;

  for (int i = 0; i < vertices; i++) {
    if (visitedVertex[i] == 0 && weights[i] < min) {

      min = weights[i];
      minIndex = i;
    }
  }
  return minIndex;
}

void depictVisited(int source, int terminus, double weight) {
  wchar_t buffer[32];
  swprintf(buffer, 32, L"%d - %d \t%lf \n", source + 1, terminus + 1, weight);
  OutputDebugString(buffer);
}

struct newEmpGraph *generateNewGraph(double **adjacencyMatrix, double **weightMatrix, int amountOfEdges) {
  struct newEmpGraph *pNewEmpGraph = (struct newEmpGraph *) malloc(sizeof(struct newEmpGraph));
  pNewEmpGraph->head = NULL;
  pNewEmpGraph->upcomingVertex = NULL;
  struct newEmpGraph *ongoing = pNewEmpGraph;

  for (int i = 0; i < vertices - 1; i++) {
    struct newEmpGraph *upcoming = (struct newEmpGraph *) malloc(sizeof(struct newEmpGraph));
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

    struct newEmpGraph *ongoingVertex = pNewEmpGraph;
    ongoingVertex = discoverVertex(ongoingVertex, source);

    struct vertexNode *newVertexNode = (struct vertexNode *) malloc(sizeof(struct vertexNode));
    newVertexNode->vertexIndex = terminus;
    newVertexNode->edgeWeight = weight;
    newVertexNode->nextNode = ongoingVertex->head;
    ongoingVertex->head = newVertexNode;

    if (source != terminus) {
      struct newEmpGraph *terminusVertex = pNewEmpGraph;
      terminusVertex = discoverVertex(terminusVertex, terminus);

      newVertexNode = (struct vertexNode *) malloc(sizeof(struct vertexNode));
      newVertexNode->vertexIndex = source;
      newVertexNode->edgeWeight = weight;
      newVertexNode->nextNode = terminusVertex->head;
      terminusVertex->head = newVertexNode;
      terminusVertex = terminusVertex->upcomingVertex;
    }
  }
  return pNewEmpGraph;
}

void updateVisitedEdges(int visitedEdges[][vertices], int source, int terminus, HDC hdc, struct coordinates coordinates,
                        struct vertexNode *pVertexNode, int radiusOfVertex, int dtx) {
  if (visitedEdges[source][terminus] == 0 && visitedEdges[terminus][source] == 0) {
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

    TextOutA(hdc, (int) centerX + offsetX, (int) centerY + offsetY, weight, strlen(weight));
    SelectObject(hdc, linePen);
  }
}

void findSpanningTree(int vertex, struct newEmpGraph *graph, struct Edge *structEdge) {
  int *source = (int *) malloc(vertices * sizeof(int));
  double *weights = (double *) malloc(vertices * sizeof(double));
  int *visited = (int *) malloc(vertices * sizeof(int));

  for (int i = 0; i < vertices; i++) {
    source[i] = 0;
    weights[i] = (double) INT_MAX;
    visited[i] = 0;
  }

  weights[vertex] = 0;

  for (int i = 0; i < vertices; i++) {
    int minIndex = getMinIndex(weights, visited);
    visited[minIndex] = 1;
    //find the edge for structEdge
    struct newEmpGraph *temp = graph;
    temp = discoverVertex(temp, minIndex);
    struct vertexNode *node = temp->head;
    while (node != NULL) {
      int dest = node->vertexIndex;
      double weight = node->edgeWeight;

      if (visited[dest] == 0 && weight < weights[dest]) {
        source[dest] = minIndex;
        weights[dest] = weight;
      }
      node = node->nextNode;
    }

    //fill MST_Edge
    struct Edge *currMst = &structEdge[i];
    if (minIndex == vertex) {
      currMst->source = vertex;
      currMst->terminus = vertex;
      currMst->weights = 0;
    } else {
      temp = graph;
      temp = discoverVertex(temp, source[minIndex]);
      node = temp->head;
      node = discoverNode(node, minIndex);

      currMst->source = temp->vertexIndex;
      currMst->terminus = node->vertexIndex;
      currMst->weights = node->edgeWeight;
    }
    depictVisited(currMst->source, currMst->terminus, currMst->weights);
  }
  free(source);
  free(weights);
  free(visited);
}


void depictUndirectedGraph(int centerX, int centerY, int radiusOfGraph, int radiusOfVertex, int radiusOfLoop, int dtx,
                           struct newEmpGraph *newEmpGraph, struct coordinates coordinates,
                           HPEN PPen, HPEN BPen, HDC hdc) {
  static int visitedEdges[vertices][vertices] = {0};

  while (newEmpGraph != NULL) {
    struct vertexNode *node = newEmpGraph->head;
    while (node != NULL) {
      int source = newEmpGraph->vertexIndex;
      int terminus = node->vertexIndex;
      MoveToEx(hdc, coordinates.nx[source], coordinates.ny[source], NULL);

      HPEN ChangePen = CreatePen(PS_SOLID, 2, RGB(155, 180, 250));
      SelectObject(hdc, ChangePen);
      if (source == terminus) {

        SelectObject(hdc, PPen);
        Ellipse(hdc, coordinates.loopX[source] - radiusOfLoop, coordinates.loopY[source] - radiusOfLoop,
                coordinates.loopX[source] + radiusOfLoop, coordinates.loopY[source] + radiusOfLoop);
        SelectObject(hdc, ChangePen);
      } else {
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
    Ellipse(hdc, coordinates.nx[i] - radiusOfVertex, coordinates.ny[i] - radiusOfVertex,
            coordinates.nx[i] + radiusOfVertex, coordinates.ny[i] + radiusOfVertex);
    wchar_t buffer[5];
    swprintf(buffer, 5, L"%d", i + 1);
    TextOut(hdc, coordinates.nx[i] - dtx, coordinates.ny[i] - radiusOfVertex / 2, buffer, 3);
  }
}


void depictSpanningTree(int tree, struct Edge *spanningTree, struct coordinates coordinates, double vertexRad, double dtx, HDC hdc) {
  for (int i = 0; i < tree; i++) {
    int src = spanningTree[i].source;
    int terminus = spanningTree[i].terminus;
    double weight = spanningTree[i].weights;
    MoveToEx(hdc, coordinates.nx[src], coordinates.ny[src], NULL);
    LineTo(hdc, coordinates.nx[terminus], coordinates.ny[terminus]);

    double x = fabs(coordinates.nx[src] + coordinates.nx[terminus]) / 2.;
    double y = fabs(coordinates.ny[src] + coordinates.ny[terminus]) / 2.;
    double textAlign = 10;

    wchar_t buffer[10];
    swprintf(buffer, 5, L"%lf", weight);
    TextOut(hdc, x - textAlign, y - textAlign, buffer, 4);
  }

  for (int i = 0; i < tree; i++) {
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(255, 165, 0));
    int terminus = spanningTree[i].terminus;
    Ellipse(hdc, coordinates.nx[terminus] - vertexRad, coordinates.ny[terminus] - vertexRad, coordinates.nx[terminus] + vertexRad,
            coordinates.ny[terminus] + vertexRad);
    wchar_t buffer2[5];
    swprintf(buffer2, 5, L"%d", terminus + 1);
    TextOut(hdc, coordinates.nx[terminus] - dtx, coordinates.ny[terminus] - vertexRad / 2, buffer2, 4);
  }
}


int spanningCounter = 0;

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
  HWND spanningButton;
  int state = 0;
  switch (messg) {
    case WM_CREATE: {
      spanningButton = CreateWindow(
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
          if (spanningCounter < vertices) spanningCounter++;
          InvalidateRect(hWnd, NULL, FALSE);
          break;

      }
    }
    case WM_PAINT :
      hdc = BeginPaint(hWnd, &ps);
      SetGraphicsMode(hdc, GM_ADVANCED);


      double **T = randm(vertices);
      double coefficient = 1.0 - 0.01 - 0.005 - 0.05;
      T = mulmr(coefficient, T, vertices);
      double **A = symmetricMatrix(T);

      double **R = randm(vertices);
      multiplyByCoefficientMatrix(R, A);
      double **Wt = roundm(R);

      double **bMatrix = createBMatrix(Wt);
      double **cMatrix = createCMatrix(bMatrix);
      double **dMatrix = createDMatrix(bMatrix);
      double **trMatrix = createTrMatrix();
      Wt = createWtMatrix(Wt, cMatrix, dMatrix, trMatrix);
      double **W = symmetricMatrix(Wt);

      HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
      HPEN PPen = CreatePen(PS_SOLID, 2, RGB(153, 0, 153));
      HPEN OPen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
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
      double angleAlpha = 2.0 * M_PI / (double) vertices;
      for (int i = 0; i < vertices; i++) {
        double sinAlpha = sin(angleAlpha * (double) i);
        double cosAlpha = cos(angleAlpha * (double) i);
        coordinates.nx[i] = circleCenterX + circleRadius * sinAlpha;
        coordinates.ny[i] = circleCenterY - circleRadius * cosAlpha;
        coordinates.loopX[i] = circleCenterX + (circleRadius + loopRadius) * sinAlpha;
        coordinates.loopY[i] = circleCenterY - (circleRadius + loopRadius) * cosAlpha;
      }


      int amountOfEdges = countAmountOfEdges(A);

      struct newEmpGraph *pNewEmpGraph = generateNewGraph(A, W, amountOfEdges);
      SetBkMode(hdc, TRANSPARENT);
      depictUndirectedGraph(circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, dtx, pNewEmpGraph,
                            coordinates, PPen, BPen, hdc);

      struct Edge *minSpanningTree = (struct Edge *) malloc(vertices * sizeof(struct Edge));
      int startVertex = (double) rand() / (double) RAND_MAX * (vertices - 1);
      findSpanningTree(startVertex, pNewEmpGraph, minSpanningTree);

      SelectObject(hdc, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hdc, RGB(255, 165, 0));
      SelectObject(hdc, OPen);
      if (state == 1) {
        depictSpanningTree(spanningCounter, minSpanningTree, coordinates, vertexRadius, dtx, hdc);
      }

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
      //free(minSpanningTree);
      releaseMemory(pNewEmpGraph);
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return (DefWindowProc(hWnd, messg, wParam, lParam));
  }
  return 0;
}
