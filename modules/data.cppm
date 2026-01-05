/// @file data.cppm
/// @brief Data and URL wrappers

export module foundationpp:data;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :string;

export namespace NS {
inline namespace v1 {
/// Wrapper for NSData - immutable container for binary data
class Data final : public Copying<Data> {
public:
    /// Allocate an uninitialized Data object
    /// @return Allocated Data (must be initialized)
    [[nodiscard]] static auto alloc() -> Data* {
        return Object::alloc<Data>(Detail::Classes::NSData());
    }

    /// Initialize as empty data
    /// @return Initialized empty data
    [[nodiscard]] auto init() -> Data* {
        return Object::init<Data>();
    }

    /// Get the number of bytes
    /// @return Byte count
    [[nodiscard]] auto length() const -> UInteger {
        return Object::sendMessage<UInteger>(this, Detail::Selectors::length());
    }

    /// Get a pointer to the raw bytes
    /// @return Pointer to immutable byte data
    [[nodiscard]] auto bytes() const -> const void* {
        return Object::sendMessage<const void*>(this, Detail::Selectors::bytes());
    }

    /// Check if the data is empty
    /// @return true if length is 0
    [[nodiscard]] auto empty() const -> bool {
        return length() == 0;
    }

    /// Get the data as a span of bytes
    /// @return Span view of the data
    [[nodiscard]] auto asSpan() const -> std::span<const std::byte> {
        return std::span{static_cast<const std::byte*>(bytes()), length()};
    }
};

/// Wrapper for NSURL - represents a URL resource locator
class URL final : public Copying<URL> {
public:
    /// Create a file URL from a path string
    /// @param pPath The file system path
    /// @return Autoreleased file URL
    [[nodiscard]] static auto fileURLWithPath(const String* pPath) -> URL* {
        return Object::sendMessage<URL*>(
            Detail::Classes::NSURL(),
            Detail::Selectors::fileURLWithPath(),
            pPath
        );
    }

    /// Allocate an uninitialized URL
    /// @return Allocated URL (must be initialized)
    [[nodiscard]] static auto alloc() -> URL* {
        return Object::alloc<URL>(Detail::Classes::NSURL());
    }

    /// Initialize with a URL string
    /// @param pString The URL string
    /// @return Initialized URL, or nullptr if invalid
    [[nodiscard]] auto init(const String* pString) -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::initWithString_URL(), pString);
    }

    /// Initialize as a file URL with a path
    /// @param pPath The file system path
    /// @return Initialized file URL
    [[nodiscard]] auto initFileURLWithPath(const String* pPath) -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::initFileURLWithPath(), pPath);
    }

    /// Initialize by copying another URL
    /// @param pURL The URL to copy
    /// @return Initialized URL
    [[nodiscard]] auto init(const URL* pURL) -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::initWithURL(), pURL);
    }

    /// Get the file system representation
    /// @return Null-terminated C string path, or nullptr if not a file URL
    [[nodiscard]] auto fileSystemRepresentation() const -> const char* {
        return Object::sendMessage<const char*>(
            this,
            Detail::Selectors::fileSystemRepresentation()
        );
    }
};
} // namespace v1
} // namespace NS
