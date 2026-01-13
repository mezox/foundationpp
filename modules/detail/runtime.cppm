/// @file runtime.cppm
/// @brief Objective-C runtime message dispatch - architecture-aware objc_msgSend wrappers
/// @note This partition is NOT exported from the primary module interface

module;

#include <objc/runtime.h>
#include <objc/message.h>
#include <cassert>

export module foundationpp:runtime;

import std;
import :detail;

namespace NS::Runtime {
inline namespace v1 {

/// Compile-time flag to enable safe message dispatch with selector validation
#ifdef FOUNDATIONPP_SAFE_MESSAGES
inline constexpr bool SafeMessagesEnabled = true;
#else
inline constexpr bool SafeMessagesEnabled = false;
#endif

namespace detail {
/// Low-level selector check that doesn't go through sendMessage (avoids recursion)
/// Uses class_respondsToSelector directly on the object's class
[[nodiscard]] inline auto objectRespondsToSelector(const void* obj, SEL sel) -> bool {
    if (!obj) return true;  // nil messaging is valid in ObjC, always "succeeds"
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast) - object_getClass requires non-const
    auto cls = object_getClass(reinterpret_cast<id>(const_cast<void*>(obj)));
    return class_respondsToSelector(cls, sel);
}

} // namespace detail

/// Traits for determining which objc_msgSend variant to use based on return type
/// @tparam Ret The return type of the message
template<typename Ret>
struct DispatchTraits {
    /// Whether objc_msgSend_stret is required for this return type
    /// - ARM64: Never required (ABI handles large returns)
    /// - x86_64: Required for structs > 16 bytes
    /// - ARM32: Required for structs > 4 bytes
    static constexpr bool requires_stret = []() consteval {
#if defined(__arm64__)
        return false;
#elif defined(__x86_64__) || defined(__i386__)
        return std::is_class_v<Ret> && sizeof(Ret) > (sizeof(std::uintptr_t) * 2);
#elif defined(__arm__)
        return std::is_class_v<Ret> && sizeof(Ret) > sizeof(std::uintptr_t);
#else
        static_assert(false, "Unsupported architecture");
#endif
    }();

    /// Whether objc_msgSend_fpret is required for this return type
    /// Only used on x86/x86_64 for floating-point returns
    static constexpr bool requires_fpret = []() consteval {
#if defined(__x86_64__) || defined(__i386__)
        return std::is_floating_point_v<Ret>;
#else
        return false;
#endif
    }();
};

/// Specialization for void return type
template<>
struct DispatchTraits<void> {
    static constexpr bool requires_stret = false;
    static constexpr bool requires_fpret = false;
};

/// Send an Objective-C message to an object
/// @tparam Ret The expected return type
/// @tparam Args The argument types
/// @param obj The target object (or class for class methods)
/// @param sel The selector to invoke
/// @param args The arguments to pass
/// @return The result of the message send
/// @note When FOUNDATIONPP_SAFE_MESSAGES is enabled, asserts if selector is not implemented
template<typename Ret, typename... Args>
[[nodiscard]] auto sendMessage(const void* obj, SEL sel, Args&&... args) -> Ret {
    if constexpr (SafeMessagesEnabled) {
        assert(detail::objectRespondsToSelector(obj, sel) && "Object does not respond to selector");
    }
    // Use std::decay_t to strip reference qualifiers from Args
    // This is necessary because Args&&... deduces lvalue references for lvalue arguments,
    // but C function pointers need value types, not reference types
    if constexpr (std::is_void_v<Ret>) {
        using Proc = void(*)(const void*, SEL, std::decay_t<Args>...);
        auto proc = reinterpret_cast<Proc>(&objc_msgSend);
        (*proc)(obj, sel, std::forward<Args>(args)...);
    }
#if defined(__x86_64__) || defined(__i386__)
    else if constexpr (DispatchTraits<Ret>::requires_fpret) {
        using Proc = Ret(*)(const void*, SEL, std::decay_t<Args>...);
        auto proc = reinterpret_cast<Proc>(&objc_msgSend_fpret);
        return (*proc)(obj, sel, std::forward<Args>(args)...);
    }
#endif
#if !defined(__arm64__)
    else if constexpr (DispatchTraits<Ret>::requires_stret) {
        using Proc = void(*)(Ret*, const void*, SEL, std::decay_t<Args>...);
        auto proc = reinterpret_cast<Proc>(&objc_msgSend_stret);
        Ret result;
        (*proc)(&result, obj, sel, std::forward<Args>(args)...);
        return result;
    }
#endif
    else {
        using Proc = Ret(*)(const void*, SEL, std::decay_t<Args>...);
        auto proc = reinterpret_cast<Proc>(&objc_msgSend);
        return (*proc)(obj, sel, std::forward<Args>(args)...);
    }
}

/// Check if an object responds to a selector
/// @param obj The object to check
/// @param sel The selector to check for
/// @return true if the object responds to the selector
[[nodiscard]] inline auto respondsToSelector(const void* obj, SEL sel) -> bool {
    return sendMessage<bool>(obj, Detail::Selectors::respondsToSelector(), sel);
}

/// Send an Objective-C message safely, checking if the selector is implemented first
/// @tparam Ret The expected return type
/// @tparam Args The argument types
/// @param obj The target object
/// @param sel The selector to invoke
/// @param args The arguments to pass
/// @return std::optional<Ret> containing the result, or std::nullopt if selector not implemented
template<typename Ret, typename... Args>
    requires (!std::is_void_v<Ret>)
[[nodiscard]] auto sendMessageSafe(const void* obj, SEL sel, Args&&... args) -> std::optional<Ret> {
    if (respondsToSelector(obj, sel)) {
        return sendMessage<Ret>(obj, sel, std::forward<Args>(args)...);
    }
    return std::nullopt;
}

/// Send an Objective-C message safely for void return type
/// @tparam Args The argument types
/// @param obj The target object
/// @param sel The selector to invoke
/// @param args The arguments to pass
/// @return true if the message was sent, false if selector not implemented
template<typename... Args>
[[nodiscard]] auto sendMessageSafeVoid(const void* obj, SEL sel, Args&&... args) -> bool {
    if (respondsToSelector(obj, sel)) {
        sendMessage<void>(obj, sel, std::forward<Args>(args)...);
        return true;
    }
    return false;
}

/// Allocate an instance of a class
/// @param cls The class to allocate
/// @return Pointer to the allocated (but not initialized) object
[[nodiscard]] inline auto allocInstance(Class cls) -> void* {
    return sendMessage<void*>(cls, Detail::Selectors::alloc());
}

/// Allocate an instance of a class by name
/// @param className The name of the class to allocate (must be null-terminated)
/// @return Pointer to the allocated (but not initialized) object, or nullptr if class not found
[[nodiscard]] inline auto allocInstance(const char* className) -> void* {
    auto cls = objc_lookUpClass(className);
    if (!cls) {
        return nullptr;
    }
    return allocInstance(cls);
}

/// Initialize an allocated object
/// @param obj The allocated object to initialize
/// @return Pointer to the initialized object
[[nodiscard]] inline auto initInstance(void* obj) -> void* {
    return sendMessage<void*>(obj, Detail::Selectors::init());
}

/// Retain an object (increment reference count)
/// @param obj The object to retain
/// @return The retained object
[[nodiscard]] inline auto retainObject(void* obj) -> void* {
    return sendMessage<void*>(obj, Detail::Selectors::retain());
}

/// Release an object (decrement reference count)
/// @param obj The object to release
inline auto releaseObject(void* obj) -> void {
    sendMessage<void>(obj, Detail::Selectors::release());
}

/// Autorelease an object
/// @param obj The object to autorelease
/// @return The autoreleased object
[[nodiscard]] inline auto autoreleaseObject(void* obj) -> void* {
    return sendMessage<void*>(obj, Detail::Selectors::autorelease());
}

/// Get the retain count of an object
/// @param obj The object to query
/// @return The current retain count
[[nodiscard]] inline auto getRetainCount(const void* obj) -> std::size_t {
    return sendMessage<std::size_t>(obj, Detail::Selectors::retainCount());
}

} // namespace v1
} // namespace NS::Runtime
