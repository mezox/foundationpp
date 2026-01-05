/// @file string.cppm
/// @brief String wrapper for NSString

export module foundationpp:string;

import std;
import :types;
import :object;
import :detail;
import :runtime;

export namespace NS {
inline namespace v1 {
/// Wrapper for NSString - immutable Unicode string
class String final : public Copying<String> {
public:
    /// Create an empty string
    /// @return Autoreleased empty string
    [[nodiscard]] static auto string() -> String* {
        return sendMessage<String*>(Detail::Classes::NSString(), Detail::Selectors::string());
    }

    /// Create a string from a C string with UTF-8 encoding
    /// @param pString Null-terminated C string
    /// @return Autoreleased string, or nullptr on failure
    [[nodiscard]] static auto string(const char* pString) -> String* {
        return string(pString, StringEncoding::UTF8);
    }

    /// Create a string from a C string with specified encoding
    /// @param pString Null-terminated C string
    /// @param encoding The character encoding
    /// @return Autoreleased string, or nullptr on failure
    [[nodiscard]] static auto string(const char* pString, StringEncoding encoding) -> String* {
        return sendMessage<String*>(
            Detail::Classes::NSString(),
            Detail::Selectors::stringWithCStringEncoding(),
            pString,
            static_cast<UInteger>(encoding)
        );
    }

    /// Create a string by copying another string
    /// @param pString The string to copy
    /// @return Autoreleased string copy
    [[nodiscard]] static auto string(const String* pString) -> String* {
        return sendMessage<String*>(
            Detail::Classes::NSString(),
            Detail::Selectors::stringWithString(),
            pString
        );
    }

    /// Allocate an uninitialized string
    /// @return Allocated string (must be initialized)
    [[nodiscard]] static auto alloc() -> String* {
        return Object::alloc<String>(Detail::Classes::NSString());
    }

    /// Initialize with default (empty) content
    /// @return Initialized string
    [[nodiscard]] auto init() -> String* {
        return Object::init<String>();
    }

    /// Initialize with a C string using UTF-8 encoding
    /// @param pString Null-terminated C string
    /// @return Initialized string
    [[nodiscard]] auto init(const char* pString) -> String* {
        return init(pString, StringEncoding::UTF8);
    }

    /// Initialize with a C string using specified encoding
    /// @param pString Null-terminated C string
    /// @param encoding The character encoding
    /// @return Initialized string
    [[nodiscard]] auto init(const char* pString, StringEncoding encoding) -> String* {
        return sendMessage<String*>(
            this,
            Detail::Selectors::initWithCStringEncoding(),
            pString,
            static_cast<UInteger>(encoding)
        );
    }

    /// Initialize by copying another string
    /// @param pString The string to copy
    /// @return Initialized string
    [[nodiscard]] auto init(const String* pString) -> String* {
        return sendMessage<String*>(
            this,
            Detail::Selectors::initWithString(),
            pString
        );
    }

    /// Get the number of Unicode characters
    /// @return Character count
    [[nodiscard]] auto length() const -> UInteger {
        return sendMessage<UInteger>(this, Detail::Selectors::length());
    }

    /// Get the character at a specific index
    /// @param index Zero-based character index
    /// @return The Unicode character at that index
    [[nodiscard]] auto characterAtIndex(UInteger index) const -> UniChar {
        return sendMessage<UniChar>(this, Detail::Selectors::characterAtIndex(), index);
    }

    /// Get a null-terminated UTF-8 C string
    /// @return Pointer to UTF-8 data (valid for lifetime of the String)
    [[nodiscard]] auto UTF8String() const -> const char* {
        return sendMessage<const char*>(this, Detail::Selectors::UTF8String());
    }

    /// Get a null-terminated C string in the specified encoding
    /// @param encoding The desired character encoding
    /// @return Pointer to encoded data, or nullptr if conversion fails
    [[nodiscard]] auto cStringUsingEncoding(StringEncoding encoding) const -> const char* {
        return sendMessage<const char*>(
            this,
            Detail::Selectors::cStringUsingEncoding(),
            static_cast<UInteger>(encoding)
        );
    }

    /// Get the number of bytes needed for the string in an encoding
    /// @param encoding The character encoding
    /// @return Byte count (0 if not representable)
    [[nodiscard]] auto lengthOfBytesUsingEncoding(StringEncoding encoding) const -> UInteger {
        return sendMessage<UInteger>(
            this,
            Detail::Selectors::lengthOfBytesUsingEncoding(),
            static_cast<UInteger>(encoding)
        );
    }

    /// Get the maximum number of bytes needed for the string in an encoding
    /// @param encoding The character encoding
    /// @return Maximum byte count
    [[nodiscard]] auto maximumLengthOfBytesUsingEncoding(StringEncoding encoding) const -> UInteger {
        return sendMessage<UInteger>(
            this,
            Detail::Selectors::maximumLengthOfBytesUsingEncoding(),
            static_cast<UInteger>(encoding)
        );
    }

    /// Check equality with another string
    /// @param pString The string to compare with
    /// @return true if the strings are equal
    [[nodiscard]] auto isEqualToString(const String* pString) const -> bool {
        return sendMessage<bool>(this, Detail::Selectors::isEqualToString(), pString);
    }

    /// Compare with another string
    /// @param pString The string to compare with
    /// @return Comparison result
    [[nodiscard]] auto compare(const String* pString) const -> ComparisonResult {
        return sendMessage<ComparisonResult>(this, Detail::Selectors::compare(), pString);
    }

    /// Case-insensitive comparison
    /// @param pString The string to compare with
    /// @return Comparison result
    [[nodiscard]] auto caseInsensitiveCompare(const String* pString) const -> ComparisonResult {
        return sendMessage<ComparisonResult>(
            this,
            Detail::Selectors::caseInsensitiveCompare(),
            pString
        );
    }

    /// Find a substring
    /// @param pString The substring to find
    /// @param options Search options
    /// @return Range of the match, or {NotFound, 0} if not found
    [[nodiscard]] auto rangeOfString(const String* pString, StringCompareOptions options = {}) const
        -> Range {
        return sendMessage<Range>(
            this,
            Detail::Selectors::rangeOfStringOptions(),
            pString,
            static_cast<UInteger>(options)
        );
    }

    /// Create a new string by appending another string
    /// @param pString The string to append
    /// @return Autoreleased concatenated string
    [[nodiscard]] auto stringByAppendingString(const String* pString) const -> String* {
        return sendMessage<String*>(this, Detail::Selectors::stringByAppendingString(), pString);
    }

    /// Check if the string is empty
    /// @return true if length is 0
    [[nodiscard]] auto empty() const -> bool {
        return length() == 0;
    }

    /// Convert to std::string (UTF-8)
    /// @return std::string containing the UTF-8 representation
    [[nodiscard]] auto toStdString() const -> std::string {
        auto* utf8 = UTF8String();
        return utf8 ? std::string{utf8} : std::string{};
    }

    /// Convert to std::string_view (UTF-8)
    /// @return std::string_view pointing to the internal UTF-8 data
    /// @warning The view is only valid for the lifetime of the String object
    [[nodiscard]] auto toStringView() const -> std::string_view {
        auto* utf8 = UTF8String();
        return utf8 ? std::string_view{utf8} : std::string_view{};
    }
};
} // namespace v1
} // namespace NS
