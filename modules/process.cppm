/// @file process.cppm
/// @brief ProcessInfo wrapper

export module foundationpp:process;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :string;
import :collections;

export namespace NS {
inline namespace v1 {
/// Wrapper for NSProcessInfo - provides information about the current process
class ProcessInfo final : public Referencing<ProcessInfo> {
public:
    /// Get the shared process info object
    /// @return The process info singleton
    [[nodiscard]] static auto processInfo() -> ProcessInfo* {
        return Object::sendMessage<ProcessInfo*>(
            Detail::Classes::NSProcessInfo(),
            Detail::Selectors::processInfo()
        );
    }

    /// Get the process name
    /// @return Name of the process
    [[nodiscard]] auto processName() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::processName());
    }

    /// Set the process name
    /// @param pName New process name
    auto setProcessName(const String* pName) -> void {
        Object::sendMessage<void>(this, Detail::Selectors::setProcessName(), pName);
    }

    /// Get the process identifier (PID)
    /// @return Process ID
    [[nodiscard]] auto processIdentifier() const -> int {
        return Object::sendMessage<int>(this, Detail::Selectors::processIdentifier());
    }

    /// Get a globally unique string
    /// @return Unique identifier string
    [[nodiscard]] auto globallyUniqueString() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::globallyUniqueString());
    }

    /// Get the environment variables
    /// @return Dictionary of environment variables
    [[nodiscard]] auto environment() const -> Dictionary<String, String>* {
        return Object::sendMessage<Dictionary<String, String>*>(
            this,
            Detail::Selectors::environment()
        );
    }

    /// Get the command line arguments
    /// @return Array of argument strings
    [[nodiscard]] auto arguments() const -> Array<String>* {
        return Object::sendMessage<Array<String>*>(this, Detail::Selectors::arguments());
    }

    /// Get the host name
    /// @return Network host name
    [[nodiscard]] auto hostName() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::hostName());
    }

    /// Get the user name
    /// @return Current user's short name
    [[nodiscard]] auto userName() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::userName());
    }

    /// Get the full user name
    /// @return Current user's full name
    [[nodiscard]] auto fullUserName() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::fullUserName());
    }

    /// Get the operating system version string
    /// @return Human-readable OS version
    [[nodiscard]] auto operatingSystemVersionString() const -> String* {
        return Object::sendMessage<String*>(
            this,
            Detail::Selectors::operatingSystemVersionString()
        );
    }

    /// Get the operating system version
    /// @return Structured OS version
    [[nodiscard]] auto operatingSystemVersion() const -> OperatingSystemVersion {
        return Object::sendMessage<OperatingSystemVersion>(
            this,
            Detail::Selectors::operatingSystemVersion()
        );
    }

    /// Check if the OS is at least a given version
    /// @param version Minimum version to check
    /// @return true if current OS >= version
    [[nodiscard]] auto isOperatingSystemAtLeastVersion(OperatingSystemVersion version) const -> bool {
        return Object::sendMessage<bool>(
            this,
            Detail::Selectors::isOperatingSystemAtLeastVersion(),
            version
        );
    }

    /// Get the number of processing cores
    /// @return Total processor count
    [[nodiscard]] auto processorCount() const -> UInteger {
        return Object::sendMessage<UInteger>(this, Detail::Selectors::processorCount());
    }

    /// Get the number of active processing cores
    /// @return Active processor count
    [[nodiscard]] auto activeProcessorCount() const -> UInteger {
        return Object::sendMessage<UInteger>(this, Detail::Selectors::activeProcessorCount());
    }

    /// Get the physical memory size
    /// @return Physical RAM in bytes
    [[nodiscard]] auto physicalMemory() const -> std::uint64_t {
        return Object::sendMessage<std::uint64_t>(this, Detail::Selectors::physicalMemory());
    }

    /// Get the system uptime
    /// @return Seconds since boot
    [[nodiscard]] auto systemUptime() const -> TimeInterval {
        return Object::sendMessage<TimeInterval>(this, Detail::Selectors::systemUptime());
    }

    /// Get the current thermal state
    /// @return Thermal state of the device
    [[nodiscard]] auto thermalState() const -> ProcessInfoThermalState {
        return Object::sendMessage<ProcessInfoThermalState>(
            this,
            Detail::Selectors::thermalState()
        );
    }

    /// Check if low power mode is enabled
    /// @return true if in low power mode
    [[nodiscard]] auto isLowPowerModeEnabled() const -> bool {
        return Object::sendMessage<bool>(this, Detail::Selectors::isLowPowerModeEnabled());
    }

    /// Check if running as a Mac Catalyst app
    /// @return true if running via Catalyst
    [[nodiscard]] auto isMacCatalystApp() const -> bool {
        return Object::sendMessageSafe<bool>(this, Detail::Selectors::isMacCatalystApp())
            .value_or(false);
    }

    /// Check if running as an iOS app on Mac
    /// @return true if iOS app on Apple Silicon Mac
    [[nodiscard]] auto isiOSAppOnMac() const -> bool {
        return Object::sendMessageSafe<bool>(this, Detail::Selectors::isiOSAppOnMac())
            .value_or(false);
    }
};
} // namespace v1
} // namespace NS
