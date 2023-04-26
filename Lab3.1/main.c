#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define vertices 12
#define IDC_BUTTON 3456
#define IDC_BUTTON2 3457

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[] = "j";

struct coordinates {
    double nx[vertices];
    double ny[vertices];
    double loopX[vertices];
    double loopY[vertices];
};


double **randm(int n) {
  srand(2220);
  double **matrix = (double **) malloc(sizeof(double *) * n);
  for (int i = 0; i < n; i++) {
    matrix[i] = (double *) malloc(sizeof(double) * n);
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = (double) (rand() * 2.0) / (double) RAND_MAX;
    }
  }
  return matrix;

}

double **mulmr(double coef, double **matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] *= coef;
      matrix[i][j] = matrix[i][j] < 1 ? 0 : 1;
    }
  }
  return matrix;
}


double **symmetricMatrix(double **matrix, int n) {
  double **symmetrical = (double **) malloc(n * sizeof(double *));
  for (int i = 0; i < n; ++i) {
    symmetrical[i] = (double *) malloc(n * sizeof(double));
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      symmetrical[i][j] = matrix[i][j];
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (symmetrical[i][j] != symmetrical[j][i]) {
        symmetrical[i][j] = 1;
        symmetrical[j][i] = 1;
      }
    }
  }
  return symmetrical;
}

void freeMatrix(double **matrix, int n) {
  for (int i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

void printMatrix(double **matrix, int n, int initialX, int initialY, HDC hdc) {
  for (int i = 0, y = initialY + 30; i < n; i++, y += 15) {
    for (int j = 0, x = initialX; j < n; j++, x += 13) {
      wchar_t buffer[2];
      swprintf(buffer, 2, L"%lf", matrix[i][j]);
      TextOut(hdc, x, y, (LPCSTR) buffer, 1);
    }
    MoveToEx(hdc, initialX, y, NULL);
  }
}

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

  const double archWidthRatio = 0.55;
  const double radiusOfVertex = 20.0;
  double archLength = sqrt((finalX - startX) ^ 2 + (finalY - startY) ^ 2);
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
                      (LPCSTR) L"Lab 3. Made by Daniil Timofeev",
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
  HWND hwndButton_direct;
  HWND hwndButton_undirect;
  int flag = 1;
  switch (messg) {
    case WM_CREATE: {
      hwndButton_direct = CreateWindow(
              (LPCSTR) L"BUTTON",
              (LPCSTR) L"Directed graph",
              WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
              700,
              50,
              160,
              50,
              hWnd,
              (HMENU) IDC_BUTTON,
              (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
              NULL);
      hwndButton_undirect = CreateWindow(
              (LPCSTR) L"BUTTON",
              (LPCSTR) L"Undirected graph",
              WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
              700,
              125,
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
          flag = 0;
          InvalidateRect(hWnd, NULL, FALSE);
          break;

        case IDC_BUTTON2:
          flag = 1;
          InvalidateRect(hWnd, NULL, FALSE);
          break;
      }
    }
    case WM_PAINT :
      hdc = BeginPaint(hWnd, &ps);
      SetGraphicsMode(hdc, GM_ADVANCED);
      HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
      HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
      HPEN GPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 128));
      HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
      SelectObject(hdc, NoPen);
      Rectangle(hdc, 0, 0, 670, 700);


      wchar_t *nn[vertices] = {
              L"1", L"2", L"3", L"4", L"5", L"6",
              L"7", L"8", L"9", L"10", L"11", L"12"
      };

      struct coordinates coordinates;

      double circleRadius = 300;
      double vertexRadius = circleRadius / 10;

      double loopRadius = vertexRadius;

      double circleCenterX = 450;
      double circleCenterY = 450;

      double dtx = vertexRadius / 2.5;

      double angleAlpha = 2.0 * M_PI / (double) vertices;
      for (int i = 0; i < vertices; i++) {
        double sinAlpha = sin(angleAlpha * (double) i);
        double cosAlpha = cos(angleAlpha * (double) i);
        coordinates.nx[i] = circleCenterX + circleRadius * sinAlpha;
        coordinates.ny[i] = circleCenterY - circleRadius * cosAlpha;
        coordinates.loopX[i] = circleCenterX + (circleRadius + loopRadius) * sinAlpha;
        coordinates.loopY[i] = circleCenterY - (circleRadius + loopRadius) * cosAlpha;
      }


      double coefficient = 1.0 - 0.02 - 0.005 - 0.25;
      double **T = randm(vertices);
      double **A = mulmr(coefficient, T, vertices);

      int initialXOofRandMatrix = 750;
      int initialYofRandMatrix = 150;
      TextOut(hdc, initialXOofRandMatrix, initialYofRandMatrix, (LPCSTR) L"Initial Matrix", 28);
      printMatrix(A, vertices, initialXOofRandMatrix, initialYofRandMatrix, hdc);

      double **R = randm(vertices);
      double **C = symmetricMatrix(mulmr(coefficient, R, vertices), vertices);
      int initialXOofSymmMatrix = initialXOofRandMatrix;
      int initialYofSymmMatrix = initialYofRandMatrix + 210;
      TextOut(hdc, initialXOofSymmMatrix, initialYofSymmMatrix, (LPCSTR) L"Symmetric Matrix", 31);
      printMatrix(C, vertices, initialXOofSymmMatrix, initialYofSymmMatrix, hdc);
      EndPaint(hWnd, &ps);

      freeMatrix(A, vertices);
      freeMatrix(C, vertices);
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return (DefWindowProc(hWnd, messg, wParam, lParam));
  }
  return 0;
}





