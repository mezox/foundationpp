/// @file types.cppm
/// @brief Basic Foundation types - Integer, UInteger, Range, enumerations

export module foundationpp:types;

import std;

export namespace NS {
inline namespace v1 {

/// Signed integer type matching Foundation's NSInteger
using Integer = std::intptr_t;

/// Unsigned integer type matching Foundation's NSUInteger
using UInteger = std::uintptr_t;

/// Time interval in seconds matching Foundation's NSTimeInterval
using TimeInterval = double;

/// Unicode character type matching Foundation's unichar
using UniChar = std::uint16_t;

/// A range of locations in a collection, matching Foundation's NSRange
struct Range {
    UInteger location;
    UInteger length;

    /// Default constructor - creates an empty range at location 0
    constexpr Range() noexcept : location{0}, length{0} {}

    /// Construct a range with location and length
    /// @param loc Starting location
    /// @param len Length of the range
    constexpr Range(UInteger loc, UInteger len) noexcept
        : location{loc}, length{len} {}

    /// Create a range with location and length
    /// @param loc Starting location
    /// @param len Length of the range
    /// @return The constructed range
    [[nodiscard]] static constexpr auto make(UInteger loc, UInteger len) noexcept -> Range {
        return Range{loc, len};
    }

    /// Get the maximum location (location + length)
    /// @return The end location (one past the last valid index)
    [[nodiscard]] constexpr auto max() const noexcept -> UInteger {
        return location + length;
    }

    /// Check if a location is within this range
    /// @param loc The location to check
    /// @return true if loc is within [location, location + length)
    [[nodiscard]] constexpr auto contains(UInteger loc) const noexcept -> bool {
        return loc >= location && (loc - location) < length;
    }

    /// Check if this range is empty
    /// @return true if length is 0
    [[nodiscard]] constexpr auto empty() const noexcept -> bool {
        return length == 0;
    }

    /// Equality comparison
    [[nodiscard]] constexpr auto operator==(const Range&) const noexcept -> bool = default;

    /// Three-way comparison
    [[nodiscard]] constexpr auto operator<=>(const Range&) const noexcept = default;
};

/// Sentinel value indicating not found, matching Foundation's NSNotFound
inline constexpr Integer NotFound = std::numeric_limits<Integer>::max();

/// Result of comparing two objects, matching Foundation's NSComparisonResult
enum class ComparisonResult : Integer {
    OrderedAscending = -1,  ///< Left operand is less than right
    OrderedSame = 0,        ///< Operands are equal
    OrderedDescending = 1   ///< Left operand is greater than right
};

/// Character encoding for strings, matching Foundation's NSStringEncoding
enum class StringEncoding : UInteger {
    ASCII = 1,
    NEXTSTEP = 2,
    JapaneseEUC = 3,
    UTF8 = 4,
    ISOLatin1 = 5,
    Symbol = 6,
    NonLossyASCII = 7,
    ShiftJIS = 8,
    ISOLatin2 = 9,
    Unicode = 10,
    WindowsCP1251 = 11,
    WindowsCP1252 = 12,
    WindowsCP1253 = 13,
    WindowsCP1254 = 14,
    WindowsCP1250 = 15,
    ISO2022JP = 21,
    MacOSRoman = 30,

    UTF16 = Unicode,
    UTF16BigEndian = 0x90000100,
    UTF16LittleEndian = 0x94000100,
    UTF32 = 0x8c000100,
    UTF32BigEndian = 0x98000100,
    UTF32LittleEndian = 0x9c000100
};

/// Options for string searching, matching Foundation's NSStringCompareOptions
enum class StringCompareOptions : UInteger {
    CaseInsensitive = 1,
    Literal = 2,
    Backwards = 4,
    Anchored = 8,
    Numeric = 64,
    DiacriticInsensitive = 128,
    WidthInsensitive = 256,
    ForcedOrdering = 512,
    RegularExpression = 1024
};

/// Bitwise OR for StringCompareOptions
[[nodiscard]] constexpr auto operator|(StringCompareOptions lhs, StringCompareOptions rhs) noexcept
    -> StringCompareOptions {
    return static_cast<StringCompareOptions>(
        static_cast<UInteger>(lhs) | static_cast<UInteger>(rhs)
    );
}

/// Bitwise AND for StringCompareOptions
[[nodiscard]] constexpr auto operator&(StringCompareOptions lhs, StringCompareOptions rhs) noexcept
    -> StringCompareOptions {
    return static_cast<StringCompareOptions>(
        static_cast<UInteger>(lhs) & static_cast<UInteger>(rhs)
    );
}

/// Activity options for ProcessInfo, matching Foundation's NSActivityOptions
enum class ActivityOptions : std::uint64_t {
    IdleDisplaySleepDisabled = (1ULL << 40),
    IdleSystemSleepDisabled = (1ULL << 20),
    SuddenTerminationDisabled = (1ULL << 14),
    AutomaticTerminationDisabled = (1ULL << 15),
    UserInitiated = (0x00FFFFFFULL | (1ULL << 20)),
    UserInitiatedAllowingIdleSystemSleep = (0x00FFFFFFULL),
    Background = 0x000000FFULL,
    LatencyCritical = 0xFF00000000ULL
};

/// Bitwise OR for ActivityOptions
[[nodiscard]] constexpr auto operator|(ActivityOptions lhs, ActivityOptions rhs) noexcept
    -> ActivityOptions {
    return static_cast<ActivityOptions>(
        static_cast<std::uint64_t>(lhs) | static_cast<std::uint64_t>(rhs)
    );
}

/// Thermal state of the device, matching Foundation's NSProcessInfoThermalState
enum class ProcessInfoThermalState : Integer {
    Nominal = 0,
    Fair = 1,
    Serious = 2,
    Critical = 3
};

/// Operating system version structure, matching Foundation's NSOperatingSystemVersion
struct OperatingSystemVersion {
    Integer majorVersion;
    Integer minorVersion;
    Integer patchVersion;

    constexpr OperatingSystemVersion() noexcept
        : majorVersion{0}, minorVersion{0}, patchVersion{0} {}

    constexpr OperatingSystemVersion(Integer major, Integer minor, Integer patch) noexcept
        : majorVersion{major}, minorVersion{minor}, patchVersion{patch} {}

    [[nodiscard]] constexpr auto operator==(const OperatingSystemVersion&) const noexcept -> bool = default;
    [[nodiscard]] constexpr auto operator<=>(const OperatingSystemVersion&) const noexcept = default;
};

/// State for fast enumeration, matching Foundation's NSFastEnumerationState
struct FastEnumerationState {
    unsigned long state;
    void** itemsPtr;
    unsigned long* mutationsPtr;
    unsigned long extra[5];
};

} // namespace v1
} // namespace NS
