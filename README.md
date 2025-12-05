# ChaCha & PRNG Statistical Comparison Project

This project benchmarks various random number generators:
- **PCG**
- **LCG**
- **ChaCha20, 12, 8, 4**
- **ChaCha4-Feedback (experimental)**

Each RNG is tested using the **TestU01** statistical suite:
- SmallCrush
- Crush
- BigCrush

The output is saved as `.csv` files and analyzed with Python visualizations.

---

## 🛠 Build Instructions

### 1. Configure and build

```bash
cmake -S . -B build
cmake --build build
