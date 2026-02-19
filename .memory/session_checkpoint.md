# Session Checkpoint: Module 06 (Copy Constructor & Deep Copy)

## 🎯 Current Goal
*   **Master Copy Semantics**: Understand and implement Deep Copy to prevent Double Free errors.
*   **Next Topic**: Operator Overloading (`operator=`) and Move Semantics (Modern C++).

## ✅ Completed Tasks (2026-02-19)
### ⚔️ Algorithm
*   **STL Stack**: Practiced `std::stack` basics and precautions (`empty()` check).
*   **Problem 2493 (Tower)**: Solved using **Monotone Stack** (Left-to-Right approach).
*   **Concept**: Learned about "Shadowing Effect" in Monotone Stacks.

### 🎓 Basic C++ (Module 06)
*   **Cumulative Practice**: Implemented `Item` and `Inventory` classes from scratch.
    *   Namespace, Initializer List, Destructor (`delete` loop).
*   **Copy Constructor**: Implemented **Deep Copy** to fix Double Free crash.
*   **Concepts**:
    *   `Item*&` (Pointer Reference) vs `Item*` (Pointer Copy).
    *   `this` pointer (C++ vs Java).
    *   `const` correctness in Copy Constructor.

## 👣 Next Steps
1.  **Operator Overloading**: Implement `operator=` for `Inventory` to handle assignment (not just initialization).
2.  **Rule of Three**: Complete the trio (Destructor, Copy Ctor, Copy Assignment).
3.  **Next Algo**: **6198 (Rooftop Garden)** to reinforce Monotone Stack.

## ⚠️ Key Constraints (Context)
*   **Socratic Method**: Guide STL selection with questions, don't just give answers.
*   **Modern C++**: Emphasize `std::unique_ptr` as the ultimate solution to manual memory management (foreshadowing Module 09).
