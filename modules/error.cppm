/// @file error.cppm
/// @brief Error wrapper with std::expected integration

export module foundationpp:error;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :string;
import :collections;

export namespace NS {
inline namespace v1 {
// Forward declaration
class Error;

/// Result type for operations that can fail with an Error
/// @tparam T The success value type
template<typename T>
using Result = std::expected<T, Error*>;

/// Result type for void operations that can fail with an Error
using VoidResult = std::expected<void, Error*>;

/// Wrapper for NSError - encapsulates error information
class Error final : public Copying<Error> {
public:
    /// Create an error with domain, code, and user info
    /// @param pDomain The error domain
    /// @param code The error code
    /// @param pUserInfo Dictionary of additional information (can be nullptr)
    /// @return Autoreleased error
    [[nodiscard]] static auto error(
        const String* pDomain,
        Integer code,
        const Dictionary<String, Object>* pUserInfo = nullptr
    ) -> Error* {
        return Object::sendMessage<Error*>(
            Detail::Classes::NSError(),
            Detail::Selectors::errorWithDomainCodeUserInfo(),
            pDomain,
            code,
            pUserInfo
        );
    }

    /// Allocate an uninitialized error
    /// @return Allocated error (must be initialized)
    [[nodiscard]] static auto alloc() -> Error* {
        return Object::alloc<Error>(Detail::Classes::NSError());
    }

    /// Initialize with domain, code, and user info
    /// @param pDomain The error domain
    /// @param code The error code
    /// @param pUserInfo Dictionary of additional information (can be nullptr)
    /// @return Initialized error
    [[nodiscard]] auto init(
        const String* pDomain,
        Integer code,
        const Dictionary<String, Object>* pUserInfo = nullptr
    ) -> Error* {
        return Object::sendMessage<Error*>(
            this,
            Detail::Selectors::initWithDomainCodeUserInfo(),
            pDomain,
            code,
            pUserInfo
        );
    }

    /// Get the error code
    /// @return The error code within the domain
    [[nodiscard]] auto code() const -> Integer {
        return Object::sendMessage<Integer>(this, Detail::Selectors::code());
    }

    /// Get the error domain
    /// @return The domain string
    [[nodiscard]] auto domain() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::domain());
    }

    /// Get the user info dictionary
    /// @return Dictionary with additional information
    [[nodiscard]] auto userInfo() const -> Dictionary<String, Object>* {
        return Object::sendMessage<Dictionary<String, Object>*>(
            this,
            Detail::Selectors::userInfo()
        );
    }

    /// Get the localized description
    /// @return Human-readable description of the error
    [[nodiscard]] auto localizedDescription() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::localizedDescription());
    }

    /// Get the localized failure reason
    /// @return Reason for the failure, or nullptr if not available
    [[nodiscard]] auto localizedFailureReason() const -> String* {
        return Object::sendMessageSafe<String*>(
            this,
            Detail::Selectors::localizedFailureReason()
        ).value_or(nullptr);
    }

    /// Get the localized recovery suggestion
    /// @return Suggested recovery action, or nullptr if not available
    [[nodiscard]] auto localizedRecoverySuggestion() const -> String* {
        return Object::sendMessageSafe<String*>(
            this,
            Detail::Selectors::localizedRecoverySuggestion()
        ).value_or(nullptr);
    }

    /// Get the localized recovery options
    /// @return Array of recovery option titles, or nullptr if not available
    [[nodiscard]] auto localizedRecoveryOptions() const -> Array<String>* {
        return Object::sendMessageSafe<Array<String>*>(
            this,
            Detail::Selectors::localizedRecoveryOptions()
        ).value_or(nullptr);
    }

    /// Convert to std::string for logging
    /// @return String representation of the error
    [[nodiscard]] auto toStdString() const -> std::string {
        auto* desc = localizedDescription();
        return desc ? desc->toStdString() : std::string{};
    }
};

/// Standard Cocoa error domain
/// @note Use String::string("NSCocoaErrorDomain") to match
inline constexpr const char* CocoaErrorDomain = "NSCocoaErrorDomain";

/// POSIX/BSD error domain
inline constexpr const char* POSIXErrorDomain = "NSPOSIXErrorDomain";

/// OSStatus error domain
inline constexpr const char* OSStatusErrorDomain = "NSOSStatusErrorDomain";

/// Mach error domain
inline constexpr const char* MachErrorDomain = "NSMachErrorDomain";

/// Create an unexpected error result
/// @tparam T The expected value type
/// @param error The error
/// @return std::unexpected containing the error
template<typename T>
[[nodiscard]] auto makeError(Error* error) -> Result<T> {
    return std::unexpected{error};
}

/// Create a void unexpected error result
/// @param error The error
/// @return std::unexpected containing the error
[[nodiscard]] inline auto makeVoidError(Error* error) -> VoidResult {
    return std::unexpected{error};
}
} // namespace v1
} // namespace NS
