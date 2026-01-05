/// @file memory.cppm
/// @brief Smart pointer for Foundation objects - SharedPtr with retain/transfer semantics

export module foundationpp:memory;

import std;
import :object;

export namespace NS {
inline namespace v1 {

/// Tag type indicating the pointer should be retained (refcount incremented)
struct RetainTag {
    explicit RetainTag() = default;
};

/// Tag type indicating ownership is being transferred (no refcount change)
struct TransferTag {
    explicit TransferTag() = default;
};

/// Tag instance for retaining a pointer
inline constexpr RetainTag Retain{};

/// Tag instance for transferring ownership
inline constexpr TransferTag Transfer{};

/// Smart pointer for Foundation objects with automatic reference counting
///
/// SharedPtr manages the lifetime of Foundation objects by automatically
/// calling retain/release. It supports two construction modes:
/// - Retain: Increments the reference count (use when you don't own the pointer)
/// - Transfer: Takes ownership without incrementing (use for alloc/init results)
///
/// @tparam T The Foundation object type (must derive from Object)
template<ObjectType T>
class SharedPtr {
public:
    using element_type = T;
    using pointer = T*;

    /// Create a null SharedPtr
    constexpr SharedPtr() noexcept = default;

    /// Create a null SharedPtr
    constexpr SharedPtr(std::nullptr_t) noexcept : ptr_{nullptr} {}

    /// Create SharedPtr by retaining a pointer
    /// Increments the reference count of the object
    /// @param ptr The pointer to retain
    SharedPtr(T* ptr, RetainTag) noexcept
        : ptr_{ptr ? static_cast<T*>(ptr->retain()) : nullptr} {}

    /// Create SharedPtr by transferring ownership
    /// Does NOT increment the reference count - assumes refcount >= 1
    /// @param ptr The pointer to take ownership of
    SharedPtr(T* ptr, TransferTag) noexcept : ptr_{ptr} {}

    /// Copy constructor - retains the pointer
    SharedPtr(const SharedPtr& other) noexcept
        : ptr_{other.ptr_ ? static_cast<T*>(other.ptr_->retain()) : nullptr} {}

    /// Move constructor - transfers ownership
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_{std::exchange(other.ptr_, nullptr)} {}

    /// Converting copy constructor for derived types
    /// @tparam U A type derived from T
    template<std::derived_from<T> U>
    SharedPtr(const SharedPtr<U>& other) noexcept
        : ptr_{other.get() ? static_cast<T*>(other.get()->retain()) : nullptr} {}

    /// Converting move constructor for derived types
    /// @tparam U A type derived from T
    template<std::derived_from<T> U>
    SharedPtr(SharedPtr<U>&& other) noexcept
        : ptr_{static_cast<T*>(other.release())} {}

    /// Destructor - releases the pointer
    ~SharedPtr() {
        if (ptr_) {
            ptr_->release();
        }
    }

    /// Copy assignment - retains the new pointer, releases the old
    auto operator=(const SharedPtr& other) -> SharedPtr& {
        if (this != &other) {
            auto* oldPtr = ptr_;
            ptr_ = other.ptr_ ? static_cast<T*>(other.ptr_->retain()) : nullptr;
            if (oldPtr) {
                oldPtr->release();
            }
        }
        return *this;
    }

    /// Move assignment - transfers ownership
    auto operator=(SharedPtr&& other) noexcept -> SharedPtr& {
        if (this != &other) {
            if (ptr_ != other.ptr_) {
                if (ptr_) {
                    ptr_->release();
                }
                ptr_ = std::exchange(other.ptr_, nullptr);
            } else {
                // Same pointer - release other's and null it
                if (other.ptr_) {
                    other.ptr_->release();
                }
                other.ptr_ = nullptr;
            }
        }
        return *this;
    }

    /// Assign nullptr - releases the current pointer
    auto operator=(std::nullptr_t) noexcept -> SharedPtr& {
        reset();
        return *this;
    }

    /// Converting copy assignment for derived types
    template<std::derived_from<T> U>
    auto operator=(const SharedPtr<U>& other) -> SharedPtr& {
        auto* oldPtr = ptr_;
        ptr_ = other.get() ? static_cast<T*>(other.get()->retain()) : nullptr;
        if (oldPtr) {
            oldPtr->release();
        }
        return *this;
    }

    /// Converting move assignment for derived types
    template<std::derived_from<T> U>
    auto operator=(SharedPtr<U>&& other) noexcept -> SharedPtr& {
        if (ptr_) {
            ptr_->release();
        }
        ptr_ = static_cast<T*>(other.release());
        return *this;
    }

    /// Get the raw pointer
    /// @return The managed pointer
    [[nodiscard]] auto get() const noexcept -> T* {
        return ptr_;
    }

    /// Arrow operator for member access
    /// @return The managed pointer
    [[nodiscard]] auto operator->() const noexcept -> T* {
        return ptr_;
    }

    /// Dereference operator
    /// @return Reference to the managed object
    [[nodiscard]] auto operator*() const noexcept -> T& {
        return *ptr_;
    }

    /// Check if the pointer is non-null
    /// @return true if managing a valid pointer
    [[nodiscard]] explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    /// Release the current pointer and set to null
    auto reset() noexcept -> void {
        if (ptr_) {
            ptr_->release();
            ptr_ = nullptr;
        }
    }

    /// Release ownership and return the pointer
    /// After calling, the SharedPtr no longer manages the object
    /// @return The previously managed pointer
    [[nodiscard]] auto release() noexcept -> T* {
        return std::exchange(ptr_, nullptr);
    }

    /// Swap with another SharedPtr
    auto swap(SharedPtr& other) noexcept -> void {
        std::swap(ptr_, other.ptr_);
    }

    /// Detach the pointer without releasing
    /// @deprecated Use release() instead
    auto detach() noexcept -> void {
        ptr_ = nullptr;
    }

    /// Compare with nullptr
    [[nodiscard]] auto operator==(std::nullptr_t) const noexcept -> bool {
        return ptr_ == nullptr;
    }

    /// Three-way comparison with another SharedPtr
    template<ObjectType U>
    [[nodiscard]] auto operator<=>(const SharedPtr<U>& other) const noexcept {
        return ptr_ <=> other.get();
    }

    /// Equality comparison with another SharedPtr
    template<ObjectType U>
    [[nodiscard]] auto operator==(const SharedPtr<U>& other) const noexcept -> bool {
        return ptr_ == other.get();
    }

private:
    T* ptr_{nullptr};

    // Allow access from other SharedPtr instantiations for converting constructors
    template<ObjectType U>
    friend class SharedPtr;
};

/// Create a SharedPtr by retaining a pointer
/// @tparam T The object type
/// @param ptr The pointer to retain
/// @return SharedPtr managing the retained pointer
template<ObjectType T>
[[nodiscard]] auto RetainPtr(T* ptr) -> SharedPtr<T> {
    return SharedPtr<T>{ptr, Retain};
}

/// Create a SharedPtr by transferring ownership
/// @tparam T The object type
/// @param ptr The pointer to take ownership of
/// @return SharedPtr managing the transferred pointer
template<ObjectType T>
[[nodiscard]] auto TransferPtr(T* ptr) -> SharedPtr<T> {
    return SharedPtr<T>{ptr, Transfer};
}

template<ObjectType T>
SharedPtr(T*, RetainTag) -> SharedPtr<T>;

template<ObjectType T>
SharedPtr(T*, TransferTag) -> SharedPtr<T>;

/// Non-member swap
template<ObjectType T>
auto swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept -> void {
    lhs.swap(rhs);
}

} // namespace v1
} // namespace NS
