/// @file concurrency.cppm
/// @brief Concurrency primitives - AutoreleasePool, Condition

export module foundationpp:concurrency;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :time;

export namespace NS {
inline namespace v1 {
/// Wrapper for NSAutoreleasePool - manages autorelease memory
///
/// Objects that are autoreleased are added to the current pool and
/// released when the pool is drained.
class AutoreleasePool final : public Referencing<AutoreleasePool> {
public:
    /// Allocate an uninitialized pool
    /// @return Allocated pool (must be initialized)
    [[nodiscard]] static auto alloc() -> AutoreleasePool* {
        return Object::alloc<AutoreleasePool>(Detail::Classes::NSAutoreleasePool());
    }

    /// Initialize the pool
    /// @return Initialized pool
    [[nodiscard]] auto init() -> AutoreleasePool* {
        return Object::init<AutoreleasePool>();
    }

    /// Drain the pool, releasing all autoreleased objects
    auto drain() -> void {
        Object::sendMessage<void>(this, Detail::Selectors::drain());
    }

    /// Add an object to this pool
    /// @param pObject The object to autorelease
    auto addObject(Object* pObject) -> void {
        Object::sendMessage<void>(this, Detail::Selectors::addObject(), pObject);
    }

    /// Show the current autorelease pools (for debugging)
    static auto showPools() -> void {
        Object::sendMessage<void>(
            Detail::Classes::NSAutoreleasePool(),
            Detail::Selectors::showPools()
        );
    }
};

/// RAII wrapper for AutoreleasePool
///
/// Creates a pool on construction and drains it on destruction.
/// Use this for scoped memory management:
/// @code
/// {
///     NS::AutoreleasePoolScope pool;
///     // Objects created here are autoreleased to this pool
/// } // Pool is drained here
/// @endcode
class AutoreleasePoolScope {
public:
    /// Create and initialize a new autorelease pool
    AutoreleasePoolScope() : pool_{AutoreleasePool::alloc()->init()} {}

    /// Drain the pool
    ~AutoreleasePoolScope() {
        if (pool_) {
            pool_->drain();
        }
    }

    // Non-copyable
    AutoreleasePoolScope(const AutoreleasePoolScope&) = delete;
    AutoreleasePoolScope& operator=(const AutoreleasePoolScope&) = delete;

    // Movable
    AutoreleasePoolScope(AutoreleasePoolScope&& other) noexcept
        : pool_{std::exchange(other.pool_, nullptr)} {}

    AutoreleasePoolScope& operator=(AutoreleasePoolScope&& other) noexcept {
        if (this != &other) {
            if (pool_) pool_->drain();
            pool_ = std::exchange(other.pool_, nullptr);
        }
        return *this;
    }

    /// Get the underlying pool
    [[nodiscard]] auto get() const noexcept -> AutoreleasePool* {
        return pool_;
    }

private:
    AutoreleasePool* pool_;
};

/// Wrapper for NSCondition - condition variable with mutex
class Condition final : public Locking<Condition> {
public:
    /// Allocate an uninitialized condition
    /// @return Allocated condition (must be initialized)
    [[nodiscard]] static auto alloc() -> Condition* {
        return Object::alloc<Condition>(Detail::Classes::NSCondition());
    }

    /// Initialize the condition
    /// @return Initialized condition
    [[nodiscard]] auto init() -> Condition* {
        return Object::init<Condition>();
    }

    /// Wait for the condition to be signaled
    /// Must be called while holding the lock
    auto wait() -> void {
        Object::sendMessage<void>(this, Detail::Selectors::wait());
    }

    /// Wait for the condition with a timeout
    /// Must be called while holding the lock
    /// @param pLimit Maximum date to wait until
    /// @return true if signaled, false if timed out
    [[nodiscard]] auto waitUntilDate(const Date* pLimit) -> bool {
        return Object::sendMessage<bool>(this, Detail::Selectors::waitUntilDate(), pLimit);
    }

    /// Signal one waiting thread
    auto signal() -> void {
        Object::sendMessage<void>(this, Detail::Selectors::signal());
    }

    /// Signal all waiting threads
    auto broadcast() -> void {
        Object::sendMessage<void>(this, Detail::Selectors::broadcast());
    }

    /// Get the condition name (for debugging)
    /// @return The name, or nullptr if not set
    [[nodiscard]] auto name() const -> class String* {
        return Object::sendMessage<class String*>(this, Detail::Selectors::name());
    }
};
} // namespace v1
} // namespace NS
