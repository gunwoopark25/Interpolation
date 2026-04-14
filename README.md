# Interpolation

주어진 통과점(Point on Curve, POC)으로부터 Bézier curve(베지어 곡선)의 제어점(Control Point, CP)을 역산하는 C++ 구현체.

## 개요

Degree 3 (Cubic) Bézier curve를 기준으로, 사용자가 지정한 통과점 4개를 정확히 지나는 곡선의 제어점을 **Bernstein matrix(베른스타인 행렬)** 기반 선형시스템으로 역산하고, De Casteljau 알고리즘으로 곡선을 재생성한다.

선형 역문제(inverse problem)를 Gauss-Jordan Elimination(가우스-조던 소거법)으로 풀고, PostScript(포스트스크립트) 포맷으로 결과를 시각화한다.

## 처리 파이프라인

```
points.txt 로드
    → Min-Max 정규화
    → Chord-length parameterization(현길이 파라미터화)
    → Bernstein matrix(베른스타인 행렬) 구성
    → Gauss-Jordan Elimination(가우스-조던 소거법) → 역행렬 계산
    → 행렬 곱 → 제어점 역산
    → De Casteljau 알고리즘 → 곡선 포인트 생성
    → 역정규화 → CP.txt + Curve.ps 출력
```

## 수학적 배경

**Bernstein Polynomial(베른스타인 다항식)** 기반의 선형시스템을 구성한다:

$$B_{j}(u_i) = \binom{n}{j}(1 - u_i)^{n-j} \cdot u_i^{j}$$

$$\mathbf{B} \cdot \mathbf{CP} = \mathbf{POC}$$

여기서 $\mathbf{B}$는 $4 \times 4$ Bernstein matrix, $\mathbf{CP}$는 미지의 제어점, $\mathbf{POC}$는 주어진 통과점이다.
Gauss-Jordan 소거법으로 $\mathbf{B}^{-1}$을 구하고 $\mathbf{CP} = \mathbf{B}^{-1} \mathbf{POC}$ 를 계산한다.

**Chord-length parameterization(현길이 파라미터화)** 으로 파라미터 $u$를 결정한다:

$$u_i = \frac{\sum_{k=1}^{i} \|\mathbf{P}_k - \mathbf{P}_{k-1}\|}{\sum_{k=1}^{n} \|\mathbf{P}_k - \mathbf{P}_{k-1}\|}$$

## 디렉토리 구조

```
Interpolation/
├── Interpolation.slnx              # Visual Studio solution
├── Interpolation/                  # 원본 구현체
│   ├── main.cpp                    # 진입점, 파이프라인 호출
│   ├── Interpolation.h             # 클래스 선언
│   ├── Interpolation.cpp           # 핵심 알고리즘 구현 (396줄)
│   ├── Point2D.h                   # 2D 좌표 구조체
│   ├── CMakeLists.txt              # CMake 빌드 설정
│   ├── points.txt                  # 입력 데이터 (통과점)
│   ├── CP.txt                      # 출력: 역산된 제어점
│   └── Curve.ps                    # 출력: PostScript 곡선 시각화
└── Interpolation_new/              # 리팩토링 버전
    └── Interpolation_new/
        ├── main.cpp
        ├── Interpolation.h/.cpp
        ├── Point2D.h
        ├── points.txt
        ├── CMakeLists.txt
        └── Curve.ps
```

## 주요 함수

| 함수 | 설명 |
|------|------|
| `fileLoad()` | `points.txt`에서 통과점 좌표 로드 및 동적 메모리 할당 |
| `normalization()` | Min-Max 스케일링으로 좌표를 [0,1]로 정규화 |
| `chord_length()` | 연속 점 간 유클리드 거리 누적합 계산 |
| `chord_length_normalization()` | 파라미터 $u$를 [0,1]로 정규화 |
| `makeBernsteinMatrix()` | 이항계수 기반 Bernstein 행렬 $\mathbf{B}$ 구성 |
| `GaussJordanElimination()` | 확대행렬 $[\mathbf{B} \mid \mathbf{I}]$ → $[\mathbf{I} \mid \mathbf{B}^{-1}]$ 변환 |
| `matrixMultiplication()` | $\mathbf{CP} = \mathbf{B}^{-1} \cdot \mathbf{POC}$ 계산 |
| `BezierCurve()` | De Casteljau 알고리즘으로 곡선 위의 점 생성 |
| `solveNormalization()` | 제어점 및 통과점을 원래 좌표계로 역변환 |
| `fileWrite()` | `CP.txt`(제어점) 및 `Curve.ps`(시각화) 파일 출력 |

## 입출력 형식

**입력** — `points.txt`: `x y` 형식으로 통과점을 한 줄씩 입력. Degree 3 기준 **4개 점** 필요.

```
100 100
200 300
300 350
500 100
```

**출력 1** — `CP.txt`: 역산된 제어점 좌표 + De Casteljau로 생성된 곡선 포인트

```
P0.x P0.y
P1.x P1.y
...
```

**출력 2** — `Curve.ps`: PostScript 벡터 포맷으로 아래 요소를 시각화

- 제어 다각형(control polygon) — 실선
- 제어점(control points) — 원 (끝점은 채움, 중간점은 테두리)
- 통과점(points on curve) — 원
- Bézier 곡선 — 실선

## 빌드 방법

**CMake (권장)**

```bash
cd Interpolation
cmake -B build -G Ninja
cmake --build build
./build/Interpolation
```

**Visual Studio**

`Interpolation.slnx`를 Visual Studio에서 열고 빌드.

**빌드 환경**

- Language: C++17
- Build system: CMake 3.20+
- Generator: Ninja 또는 Makefile
- IDE: CLion / Visual Studio 2022

## 구현 상태

- [x] 파일 로드 (`fileLoad`)
- [x] 좌표 정규화 / 역정규화 (`normalization`, `solveNormalization`)
- [x] Chord-length parameterization (`chord_length`, `chord_length_normalization`)
- [x] Bernstein 행렬 구성 (`makeBernsteinMatrix`)
- [x] Gauss-Jordan 소거법으로 역행렬 계산 (`GaussJordanElimination`)
- [x] 행렬 곱으로 제어점 역산 (`matrixMultiplication`)
- [x] De Casteljau 알고리즘으로 곡선 생성 (`BezierCurve`)
- [x] PostScript 시각화 출력 (`fileWrite`)

## 알려진 이슈

`fileWrite()` 내 제어점/통과점 원 그리기 로직에 대입 연산자 오류가 있음:

```cpp
// 현재 (버그): 조건식에서 비교 대신 대입 발생
if (i = 0 || i == Degree)

// 수정 필요
if (i == 0 || i == Degree)
```

## 관련 이론

- Farin, G. (2002). *Curves and Surfaces for CAGD: A Practical Guide* (5th ed.). Morgan Kaufmann.
- Piegl, L., & Tiller, W. (1997). *The NURBS Book* (2nd ed.). Springer. [DOI: 10.1007/978-3-642-59223-2](https://doi.org/10.1007/978-3-642-59223-2)
