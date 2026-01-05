/// @file object.cppm
/// @brief Object base class and CRTP mixins - Referencing, Copying, SecureCoding, Locking

module;

#include <objc/runtime.h>

export module foundationpp:object;

import std;
import :types;
import :detail;
import :runtime;

export namespace NS {
inline namespace v1 {

// Forward declarations
class Object;
class String;
class MethodSignature;

/// Concept for types that derive from Object
template<typename T>
concept ObjectType = std::derived_from<T, Object>;

/// Concept for types that can be reference counted
template<typename T>
concept ReferenceCounted = ObjectType<T> && requires(T* t) {
    { t->retain() } -> std::same_as<T*>;
    { t->release() } -> std::same_as<void>;
    { t->autorelease() } -> std::same_as<T*>;
};

/// Concept for types that support copying
template<typename T>
concept Copyable = ReferenceCounted<T> && requires(const T* t) {
    { t->copy() } -> std::same_as<T*>;
};

/// Base class for all Foundation object wrappers
///
/// Object provides the fundamental interface shared by all Foundation objects:
/// - Reference counting (retain, release, autorelease)
/// - Equality and hashing
/// - Description for debugging
///
/// This class wraps the underlying objc_object and provides type-safe access
/// to Foundation methods via the Objective-C runtime.
class Object : public objc_object {
public:
    /// Increment the reference count
    /// @return this object
    [[nodiscard]] auto retain() -> Object* {
        return static_cast<Object*>(Runtime::retainObject(this));
    }

    /// Decrement the reference count
    /// Object may be deallocated if count reaches zero
    auto release() -> void {
        Runtime::releaseObject(this);
    }

    /// Add this object to the current autorelease pool
    /// @return this object
    [[nodiscard]] auto autorelease() -> Object* {
        return static_cast<Object*>(Runtime::autoreleaseObject(this));
    }

    /// Get the current reference count
    /// @return The retain count (for debugging only)
    [[nodiscard]] auto retainCount() const -> UInteger {
        return Runtime::getRetainCount(this);
    }

    /// Get the hash value for this object
    /// @return Hash suitable for use in hash tables
    [[nodiscard]] auto hash() const -> UInteger {
        return Runtime::sendMessage<UInteger>(this, Detail::Selectors::hash());
    }

    /// Check equality with another object
    /// @param pObject The object to compare with
    /// @return true if the objects are equal
    [[nodiscard]] auto isEqual(const Object* pObject) const -> bool {
        return Runtime::sendMessage<bool>(this, Detail::Selectors::isEqual(), pObject);
    }

    /// Get a human-readable description of this object
    /// @return String description
    [[nodiscard]] auto description() const -> String* {
        return Runtime::sendMessage<String*>(this, Detail::Selectors::description());
    }

    /// Get a debug description of this object
    /// @return String description suitable for debugging
    [[nodiscard]] auto debugDescription() const -> String* {
        return Runtime::sendMessageSafe<String*>(this, Detail::Selectors::debugDescription())
            .value_or(nullptr);
    }

protected:
    /// Allocate an instance of a class
    /// @tparam Class The type to allocate (must derive from Object)
    /// @param pClassName Name of the Objective-C class
    /// @return Allocated (but not initialized) instance
    template<ObjectType Class>
    [[nodiscard]] static auto alloc(const char* pClassName) -> Class* {
        return static_cast<Class*>(Runtime::allocInstance(pClassName));
    }

    /// Allocate an instance of a class
    /// @tparam Class The type to allocate (must derive from Object)
    /// @param pClass The Objective-C class object
    /// @return Allocated (but not initialized) instance
    template<ObjectType Class>
    [[nodiscard]] static auto alloc(const void* pClass) -> Class* {
        return static_cast<Class*>(Runtime::sendMessage<void*>(pClass, Detail::Selectors::alloc()));
    }

    /// Initialize an allocated object
    /// @tparam Class The type to initialize
    /// @return Initialized instance
    template<ObjectType Class>
    [[nodiscard]] auto init() -> Class* {
        return static_cast<Class*>(Runtime::initInstance(this));
    }

    /// Send a message to an object
    /// @tparam Ret Return type
    /// @tparam Args Argument types
    /// @param pObj Target object
    /// @param sel Selector to invoke
    /// @param args Arguments to pass
    /// @return Result of the message send
    template<typename Ret, typename... Args>
    [[nodiscard]] static auto sendMessage(const void* pObj, SEL sel, Args&&... args) -> Ret {
        return Runtime::sendMessage<Ret>(pObj, sel, std::forward<Args>(args)...);
    }

    /// Send a message safely, checking if selector is implemented
    /// @tparam Ret Return type
    /// @tparam Args Argument types
    /// @param pObj Target object
    /// @param sel Selector to invoke
    /// @param args Arguments to pass
    /// @return Optional result, nullopt if selector not implemented
    template<typename Ret, typename... Args>
        requires (!std::is_void_v<Ret>)
    [[nodiscard]] static auto sendMessageSafe(const void* pObj, SEL sel, Args&&... args)
        -> std::optional<Ret> {
        return Runtime::sendMessageSafe<Ret>(pObj, sel, std::forward<Args>(args)...);
    }

    /// Check if object responds to a selector
    /// @param pObj The object to check
    /// @param sel The selector
    /// @return true if the object responds to the selector
    [[nodiscard]] static auto respondsToSelector(const void* pObj, SEL sel) -> bool {
        return Runtime::respondsToSelector(pObj, sel);
    }

private:
    // Prevent direct construction - Objects are created via alloc/init
    Object() = delete;
    Object(const Object&) = delete;
    ~Object() = delete;
    Object& operator=(const Object&) = delete;
};

/// CRTP mixin providing typed reference counting
/// @tparam Derived The derived class type
/// @tparam Base The base class (defaults to Object)
template<typename Derived, typename Base = Object>
class Referencing : public Base {
public:
    /// Increment the reference count
    /// @return this object as Derived*
    [[nodiscard]] auto retain() -> Derived* {
        return static_cast<Derived*>(Base::retain());
    }

    /// Decrement the reference count
    /// Inherited from Base
    using Base::release;

    /// Add to the current autorelease pool
    /// @return this object as Derived*
    [[nodiscard]] auto autorelease() -> Derived* {
        return static_cast<Derived*>(Base::autorelease());
    }

    /// Get the current reference count
    /// Inherited from Base
    using Base::retainCount;
};

/// CRTP mixin providing copy semantics
/// @tparam Derived The derived class type
/// @tparam Base The base class (defaults to Object)
template<typename Derived, typename Base = Object>
class Copying : public Referencing<Derived, Base> {
public:
    /// Create a copy of this object
    /// @return A new object that is a copy of this one
    [[nodiscard]] auto copy() const -> Derived* {
        return Object::sendMessage<Derived*>(this, Detail::Selectors::copy());
    }
};

/// CRTP mixin for secure coding support
/// @tparam Derived The derived class type
/// @tparam Base The base class (defaults to Object)
template<typename Derived, typename Base = Object>
class SecureCoding : public Referencing<Derived, Base> {
    // Marker mixin for types that support secure coding
    // Additional methods would be added for NSCoding/NSSecureCoding protocols
};

/// CRTP mixin for types that support locking
/// @tparam Derived The derived class type
/// @tparam Base The base class (defaults to Object)
template<typename Derived, typename Base = Object>
class Locking : public Base {
public:
    /// Acquire the lock
    auto lock() -> void {
        Object::sendMessage<void>(this, Detail::Selectors::lock());
    }

    /// Release the lock
    auto unlock() -> void {
        Object::sendMessage<void>(this, Detail::Selectors::unlock());
    }

    /// RAII lock guard for Locking types
    class Guard {
    public:
        explicit Guard(Derived* obj) : obj_{obj} {
            if (obj_) obj_->lock();
        }

        ~Guard() {
            if (obj_) obj_->unlock();
        }

        Guard(const Guard&) = delete;
        Guard& operator=(const Guard&) = delete;

        Guard(Guard&& other) noexcept : obj_{std::exchange(other.obj_, nullptr)} {}
        Guard& operator=(Guard&& other) noexcept {
            if (this != &other) {
                if (obj_) obj_->unlock();
                obj_ = std::exchange(other.obj_, nullptr);
            }
            return *this;
        }

    private:
        Derived* obj_;
    };

    /// Create a scoped lock guard
    /// @return RAII guard that holds the lock
    [[nodiscard]] auto scopedLock() -> Guard {
        return Guard{static_cast<Derived*>(this)};
    }
};

} // namespace v1
} // namespace NS
