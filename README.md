# Interpolation

주어진 통과점(Point on Curve)으로부터 Bézier curve(베지어 곡선)의 제어점(Control Point)을 역산하는 C++ 보간 프로그램. (작업 중)

## 개요

Degree 3 (Cubic) Bézier curve를 기준으로, 사용자가 지정한 통과점을 정확히 지나는 곡선의 제어점을 Bernstein matrix(베른스타인 행렬) 기반 선형시스템으로 풀어낸다.

### 처리 파이프라인

```
points.txt 로드 → 좌표 정규화 → Chord-length 파라미터화 → Bernstein 행렬 생성 → [제어점 역산 - 미구현]
```

## 파일 구조

```
Interpolation/
├── main.cpp              # 진입점, 파이프라인 호출
├── Interpolation.h/.cpp  # 핵심 보간 클래스
├── Point2D.h             # 2D 좌표 구조체
└── points.txt            # 입력 데이터 (통과점 좌표)
```

## 주요 기능

| 함수 | 설명 |
|------|------|
| `fileLoad()` | `points.txt`에서 통과점 좌표 로드 |
| `normalization()` | 첫점-끝점 거리 기준으로 좌표 정규화 |
| `solveNormalization()` | 제어점을 원래 좌표계로 역변환 |
| `chord_length()` | Chord-length parameterization(현길이 파라미터화)으로 파라미터 `u` 계산 |
| `makeBernsteinMatrix()` | 이항계수 기반 Bernstein 행렬 구성 |

## 입력 형식

`points.txt`에 `x y` 형식으로 통과점을 한 줄씩 입력. Degree 3 기준 4개 점 필요.

```
100 100
200 300
300 350
500 100
```

## 빌드 환경

- Language: C++17
- Build: CMake 3.20+, 또는 Visual Studio (`.slnx`)
- IDE: CLion / Visual Studio

## 현재 구현 상태

- [x] 파일 로드
- [x] 좌표 정규화 / 역정규화
- [x] Chord-length 파라미터화
- [x] Bernstein 행렬 생성
- [ ] 선형시스템 풀이 (제어점 역산)
- [ ] 곡선 포인트 생성 및 출력
