/// @file foundationpp.cppm
/// @brief Primary module interface for foundationpp
///
/// This is the main entry point for the foundationpp library.
/// Import this module to access all Foundation wrappers.

export module foundationpp;

// Re-export all public partitions
export import :types;
export import :object;
export import :memory;
export import :string;
export import :collections;
export import :data;
export import :error;
export import :time;
export import :bundle;
export import :process;
export import :concurrency;
export import :notification;

// Import internal partitions (NOT re-exported - hidden from users)
import :detail;
import :runtime;

export namespace NS {
inline namespace v1 {
/// Library version information
struct Version {
    int major;
    int minor;
    int patch;

    constexpr Version(int maj, int min, int pat) noexcept
        : major{maj}, minor{min}, patch{pat} {}

    [[nodiscard]] constexpr auto operator==(const Version&) const noexcept -> bool = default;
    [[nodiscard]] constexpr auto operator<=>(const Version&) const noexcept = default;
};

/// Current library version
inline constexpr Version LibraryVersion{2, 0, 0};
} // namespace v1
} // namespace NS
