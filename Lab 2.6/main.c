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
  struct newEmpGraph* futureVertex;
};




void arrow(double fi, double px, double py, HDC hdc) {
  double lx, ly, rx, ry;
  lx = px + 15 * cos(fi + 0.3);
  rx = px + 15 * cos(fi - 0.3);
  ly = py + 15 * sin(fi + 0.3);
  ry = py + 15 * sin(fi - 0.3);
  MoveToEx(hdc, lx, ly, NULL);
  LineTo(hdc, px, py);
  LineTo(hdc, rx, ry);
}


void depictArch(int startX, int startY, int finalX, int finalY, int archInterval, HDC hdc) {
  XFORM transformMatrix;
  XFORM initialMatrix;
  GetWorldTransform(hdc, &initialMatrix);

  double angle = atan2(finalY - startY, finalX - startX) - M_PI / 2.0;
  transformMatrix.eM11 = (FLOAT) cos(angle);
  transformMatrix.eM12 = (FLOAT) sin(angle);
  transformMatrix.eM21 = (FLOAT) (-sin(angle));
  transformMatrix.eM22 = (FLOAT) cos(angle);
  transformMatrix.eDx = (FLOAT) startX;
  transformMatrix.eDy = (FLOAT) startY;
  SetWorldTransform(hdc, &transformMatrix);

  const double archWidthRatio = 0.75;
  double archLength = sqrt((finalX - startX) * (finalX - startX) + (finalY - startY) * (finalY - startY));
  double radiusOfVertex = 15.0;
  double semiMinorAxis = archWidthRatio * archLength;
  double semiMajorAxis = archLength / 2;

  double ellipseStartY = semiMajorAxis;

  double vertexAreaSquared = semiMajorAxis * semiMajorAxis * radiusOfVertex * radiusOfVertex;
  double semiAxesSquared = semiMinorAxis * semiMinorAxis * semiMajorAxis * semiMajorAxis;
  double distanceFromCenter = semiMinorAxis * semiMinorAxis * ellipseStartY * ellipseStartY;
  double distanceFromVertex = semiMinorAxis * semiMinorAxis * radiusOfVertex * radiusOfVertex;
  double semiMinorAxisPow = pow(semiMinorAxis, 4);

  double intersection = semiMajorAxis *
                        sqrt(vertexAreaSquared - semiAxesSquared + distanceFromCenter - distanceFromVertex +
                             semiMinorAxisPow);
  double semiMinorAxisSquaredEllipseStartY = semiMinorAxis * semiMinorAxis * ellipseStartY;
  double denominator = -semiMajorAxis * semiMajorAxis + semiMinorAxis * semiMinorAxis;


  double contactYRightTop = (semiMinorAxisSquaredEllipseStartY - intersection) / denominator;
  double contactXRightTop = sqrt(radiusOfVertex * radiusOfVertex - contactYRightTop * contactYRightTop);
  double contactYBottom = archLength - contactYRightTop;
  double contactXLeftBottom = -contactXRightTop;

  if (archInterval <= vertices / 2) {
    Arc(hdc, -archWidthRatio * archLength, archLength, archWidthRatio * archLength, 0, 0, 0, 0, archLength);
    double angleOfArrow = -atan2(archLength - contactYBottom, contactXLeftBottom) + 0.3 / 3;
    arrow(angleOfArrow, contactXLeftBottom, contactYBottom, hdc);
  } else {
    Arc(hdc, -archWidthRatio * archLength, archLength, archWidthRatio * archLength, 0, 0, archLength, 0, 0);
    double angleOfArrow = -atan2(archLength - contactYBottom, -contactXLeftBottom) - 0.3 / 3;
    arrow(angleOfArrow, -contactXLeftBottom, contactYBottom, hdc);
  }

  SetWorldTransform(hdc, &initialMatrix);

}



void depictUndirectedGraph(int centerX, int centerY, int radiusOfGraph, int radiusOfVertex, int radiusOfLoop, double angle,
                           struct coordinates coordinates, double **matrix,
                           HPEN KPen, HPEN GPen, HDC hdc) {
  for (int i = 0; i < vertices; ++i) {

    for (int j = 0; j < vertices; ++j) {
      MoveToEx(hdc, coordinates.nx[i], coordinates.ny[i], NULL);

      if (matrix[i][j] == 1) {

        if (i == j) {
          SelectObject(hdc, GPen);
          Ellipse(hdc, coordinates.loopX[i] - radiusOfLoop, coordinates.loopY[i] - radiusOfLoop,
                  coordinates.loopX[i] + radiusOfLoop, coordinates.loopY[i] + radiusOfLoop);
          SelectObject(hdc, KPen);
        } else {
          LineTo(hdc, coordinates.nx[j], coordinates.ny[j]);
        }

      }
    }
  }

}


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
                      (LPCSTR) "Lab 3.  by Daniil Timofeev IM-22",
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
  HWND Button_directed;
  HWND Button_undirected;
  int state = 0;
  switch (messg) {
    case WM_CREATE: {
      Button_directed = CreateWindow(
        (LPCSTR) "BUTTON",
        (LPCSTR) "Switch to Directed",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        700,
        30,
        160,
        50,
        hWnd,
        (HMENU) IDC_BUTTON,
        (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
      Button_undirected = CreateWindow(
        (LPCSTR) "BUTTON",
        (LPCSTR) "Switch to Undirected",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        700,
        600,
        160,
        50,
        hWnd,
        (HMENU) IDC_BUTTON2,
        (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
      return 0;
    }
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {

        case IDC_BUTTON:
          state = 0;
          InvalidateRect(hWnd, NULL, FALSE);
          break;

        case IDC_BUTTON2:
          state = 1;
          InvalidateRect(hWnd, NULL, FALSE);
          break;
      }
    }
    case WM_PAINT :
      hdc = BeginPaint(hWnd, &ps);
      SetGraphicsMode(hdc, GM_ADVANCED);
      HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
      HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
      HPEN GPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
      HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
      SelectObject(hdc, NoPen);
      Rectangle(hdc, 0, 0, 670, 700);


      char *nn[vertices] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10\0", "11\0", "12\0"};

      struct coordinates coordinates;

      double circleRadius = 200;
      double vertexRadius = circleRadius / 12;

      double loopRadius = vertexRadius;
      double dtx = vertexRadius / 2.5;

      double circleCenterX = 370;
      double circleCenterY = 360;


      double angleAlpha = 2.0 * M_PI / (double) vertices;
      for (int i = 0; i < vertices; i++) {

        double sinAlpha = sin(angleAlpha * (double) i);
        double cosAlpha = cos(angleAlpha * (double) i);
        coordinates.nx[i] = circleCenterX + circleRadius * sinAlpha;
        coordinates.ny[i] = circleCenterY - circleRadius * cosAlpha;
        coordinates.loopX[i] = circleCenterX + (circleRadius + loopRadius) * sinAlpha;
        coordinates.loopY[i] = circleCenterY - (circleRadius + loopRadius) * cosAlpha;

      }


      double **T = randm(vertices);
      double coefficient = 1.0 - 0.01 - 0.005 - 0.05;
      double **A = mulmr(coefficient, T, vertices);

      int initialXOofRandMatrix = 750;
      int initialYofRandMatrix = 150;
      TextOut(hdc, initialXOofRandMatrix, initialYofRandMatrix, (LPCSTR) L"Initial Matrix", 28);
      //printMatrix(A, vertices, initialXOofRandMatrix, initialYofRandMatrix, hdc);

      double **R = randm(vertices);
      //double **C = symmetricMatrix(mulmr(coefficient, R, vertices), vertices);
      int initialXOofSymmMatrix = initialXOofRandMatrix;
      int initialYofSymmMatrix = initialYofRandMatrix + 210;
      TextOut(hdc, initialXOofSymmMatrix, initialYofSymmMatrix, (LPCSTR) L"Symmetric Matrix", 31);
      //printMatrix(C, vertices, initialXOofSymmMatrix, initialYofSymmMatrix, hdc);


      SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
      SelectObject(hdc, KPen);



      /*depictUndirectedGraph(circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, angleAlpha,
                              coordinates, C, KPen, GPen, hdc);*/


      SelectObject(hdc, BPen);
      SelectObject(hdc, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hdc, RGB(204, 204, 255));
      SetBkMode(hdc, TRANSPARENT);

      for (int i = 0; i < vertices; ++i) {
        Ellipse(hdc, coordinates.nx[i] - vertexRadius, coordinates.ny[i] - vertexRadius,
                coordinates.nx[i] + vertexRadius, coordinates.ny[i] + vertexRadius);
        TextOut(hdc, coordinates.nx[i] - dtx, coordinates.ny[i] - vertexRadius / 2, nn[i], 2);
      }


      EndPaint(hWnd, &ps);

      freeMatrix(A, vertices);
      /*freeMatrix(C, vertices);*/
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return (DefWindowProc(hWnd, messg, wParam, lParam));
  }
  return 0;
}
