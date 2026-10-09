# C++ workshop — implement and explain an owning value type

[Contents](../../TEXTBOOK.md)

This workshop brings together constructors, destruction, copy/move semantics, exceptions, references, const correctness, and numeric bounds. Attempt the interface before reading the reference. The normal application choice would be vector; implementing a small buffer here makes ownership mechanics visible.

## Specification

IntBuffer owns a fixed-size sequence of zero-initialized integers. Copies own independent storage. Moves transfer ownership and leave the source empty. Checked access throws for an invalid index. Copy assignment must leave the destination unchanged if allocating the replacement fails. The class must not leak or delete one allocation twice.

The invariant is: a zero size has no owned allocation, and a positive size has an allocation for exactly that many integers. The representation cannot be exposed for callers to replace independently of size.

## Complete C++17 reference program

```cpp
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

class IntBuffer {
    std::unique_ptr<int[]> data_;
    std::size_t size_ = 0;
public:
    explicit IntBuffer(std::size_t size = 0)
        : data_(size ? std::make_unique<int[]>(size) : nullptr), size_(size) {}

    IntBuffer(const IntBuffer& other) : IntBuffer(other.size_) {
        for (std::size_t i=0; i<size_; ++i) data_[i]=other.data_[i];
    }

    IntBuffer(IntBuffer&& other) noexcept
        : data_(std::move(other.data_)), size_(std::exchange(other.size_,0)) {}

    IntBuffer& operator=(const IntBuffer& other) {
        if (this != &other) {
            IntBuffer replacement(other);
            swap(replacement);
        }
        return *this;
    }

    IntBuffer& operator=(IntBuffer&& other) noexcept {
        if (this != &other) {
            data_=std::move(other.data_);
            size_=std::exchange(other.size_,0);
        }
        return *this;
    }

    ~IntBuffer() = default;

    void swap(IntBuffer& other) noexcept {
        data_.swap(other.data_);
        std::swap(size_,other.size_);
    }

    std::size_t size() const noexcept { return size_; }
    int& at(std::size_t index) {
        if (index>=size_) throw std::out_of_range("Buffer index");
        return data_[index];
    }
    const int& at(std::size_t index) const {
        if (index>=size_) throw std::out_of_range("Buffer index");
        return data_[index];
    }
};

int main() {
    IntBuffer first(3);
    first.at(0)=10;
    IntBuffer copied=first;
    copied.at(0)=99;
    std::cout << first.at(0) << ' ' << copied.at(0) << '\n';
    IntBuffer moved=std::move(copied);
    std::cout << copied.size() << ' ' << moved.at(0) << '\n';
    first=moved;
    moved.at(0)=7;
    std::cout << first.at(0) << ' ' << moved.at(0) << '\n';
    try {
        first.at(3)=1;
    } catch (const std::out_of_range&) {
        std::cout << "Invalid index rejected\n";
    }
}
```

Expected output, to verify after compiling:

```text
10 99
0 99
99 7
Invalid index rejected
```

## Walkthrough

Construction uses make_unique for array ownership and value-initializes integers to zero. If allocation fails, construction does not produce a partially usable buffer. The delegating copy constructor first creates the correct-size destination, then copies values. Integer assignments in that loop do not throw.

Move construction transfers the owner and resets the source size through exchange. Merely defaulting the move constructor would transfer the pointer but leave the scalar size unchanged, breaking this class's chosen moved-from invariant. This is a case where composing a safe member still requires reasoning about relationships among members.

Copy assignment first makes a complete replacement. If allocation fails, the destination has not changed. Swapping commits the new state without throwing. The temporary then destroys the old destination allocation. This is the strong exception guarantee through preparation and commit.

Move assignment releases the previous owned allocation through unique_ptr assignment and takes the new one. It performs no allocation. The self-check prevents accidentally emptying an object during self-move under this implementation's contract.

Two at overloads preserve const correctness: a mutable buffer returns an editable reference, while a const buffer returns a const reference. Both references borrow storage; they become invalid when that allocation is destroyed, replaced, or transferred and later destroyed by another owner.

## Required experiments and solutions

1. Copy an empty buffer. **Expected:** Empty independent value; no element access is valid.
2. Assign a buffer to itself. **Expected:** Values unchanged.
3. Move from a buffer, then assign a fresh buffer into the source. **Expected:** The empty moved-from source remains a valid assignable object.
4. Explain the cost of copying n values. **Answer:** O(n) time and O(n) new storage. Moving is O(1) here.
5. Replace unique_ptr with a raw pointer and default the copy. **Reasoning only:** Both objects would claim one allocation; default destruction would either leak or, if manual deletion were added, risk double deletion. Do not execute the broken ownership version.
6. Redesign using vector. **Answer:** A vector member can supply the desired ordinary copy/move/destruction behavior with far less custom code, though its exact moved-from value contract should not be invented. Prefer that design when the learning exercise is over.

**Final explanation:** Tell a reviewer who owns each allocation before and after every statement in main. If you can do that accurately, copy/move syntax has become an ownership model rather than something to memorize.
