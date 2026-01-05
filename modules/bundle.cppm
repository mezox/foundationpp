/// @file bundle.cppm
/// @brief Bundle wrapper with std::expected error handling

export module foundationpp:bundle;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :string;
import :collections;
import :data;
import :error;

export namespace NS {
inline namespace v1 {
/// Wrapper for NSBundle - represents a bundle of code and resources
class Bundle final : public Referencing<Bundle> {
public:
    /// Get the main bundle (the application's bundle)
    /// @return The main bundle
    [[nodiscard]] static auto mainBundle() -> Bundle* {
        return Object::sendMessage<Bundle*>(
            Detail::Classes::NSBundle(),
            Detail::Selectors::mainBundle()
        );
    }

    /// Get a bundle from a path
    /// @param pPath Path to the bundle
    /// @return Bundle at the path, or nullptr if not found
    [[nodiscard]] static auto bundleWithPath(const String* pPath) -> Bundle* {
        return Object::sendMessage<Bundle*>(
            Detail::Classes::NSBundle(),
            Detail::Selectors::bundleWithPath(),
            pPath
        );
    }

    /// Get a bundle from a URL
    /// @param pURL URL to the bundle
    /// @return Bundle at the URL, or nullptr if not found
    [[nodiscard]] static auto bundleWithURL(const URL* pURL) -> Bundle* {
        return Object::sendMessage<Bundle*>(
            Detail::Classes::NSBundle(),
            Detail::Selectors::bundleWithURL(),
            pURL
        );
    }

    /// Get all loaded bundles
    /// @return Array of all loaded bundles
    [[nodiscard]] static auto allBundles() -> Array<Object>* {
        return Object::sendMessage<Array<Object>*>(
            Detail::Classes::NSBundle(),
            Detail::Selectors::allBundles()
        );
    }

    /// Get all loaded frameworks
    /// @return Array of all loaded framework bundles
    [[nodiscard]] static auto allFrameworks() -> Array<Object>* {
        return Object::sendMessage<Array<Object>*>(
            Detail::Classes::NSBundle(),
            Detail::Selectors::allFrameworks()
        );
    }

    /// Allocate an uninitialized bundle
    /// @return Allocated bundle (must be initialized)
    [[nodiscard]] static auto alloc() -> Bundle* {
        return Object::alloc<Bundle>(Detail::Classes::NSBundle());
    }

    /// Initialize with a path
    /// @param pPath Path to the bundle
    /// @return Initialized bundle, or nullptr if invalid
    [[nodiscard]] auto init(const String* pPath) -> Bundle* {
        return Object::sendMessage<Bundle*>(this, Detail::Selectors::initWithPath(), pPath);
    }

    /// Load the bundle's executable code
    /// @return Success or Error
    [[nodiscard]] auto load() -> VoidResult {
        Error* error = nullptr;
        bool success = Object::sendMessage<bool>(
            this,
            Detail::Selectors::loadAndReturnError(),
            &error
        );
        if (success) {
            return {};
        }
        return std::unexpected{error};
    }

    /// Unload the bundle's executable code
    /// @return true if unloaded successfully
    [[nodiscard]] auto unload() -> bool {
        return Object::sendMessage<bool>(this, Detail::Selectors::unload());
    }

    /// Check if the bundle is loaded
    /// @return true if loaded
    [[nodiscard]] auto isLoaded() const -> bool {
        return Object::sendMessage<bool>(this, Detail::Selectors::isLoaded());
    }

    /// Preflight the bundle (check if it can be loaded)
    /// @return Success or Error
    [[nodiscard]] auto preflight() const -> VoidResult {
        Error* error = nullptr;
        bool success = Object::sendMessage<bool>(
            this,
            Detail::Selectors::preflightAndReturnError(),
            &error
        );
        if (success) {
            return {};
        }
        return std::unexpected{error};
    }

    /// Get the bundle identifier
    /// @return Bundle identifier string
    [[nodiscard]] auto bundleIdentifier() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::bundleIdentifier());
    }

    /// Get the info dictionary
    /// @return Dictionary of bundle info
    [[nodiscard]] auto infoDictionary() const -> Dictionary<String, Object>* {
        return Object::sendMessage<Dictionary<String, Object>*>(
            this,
            Detail::Selectors::infoDictionary()
        );
    }

    /// Get the localized info dictionary
    /// @return Dictionary of localized bundle info
    [[nodiscard]] auto localizedInfoDictionary() const -> Dictionary<String, Object>* {
        return Object::sendMessage<Dictionary<String, Object>*>(
            this,
            Detail::Selectors::localizedInfoDictionary()
        );
    }

    /// Get an object from the info dictionary
    /// @param pKey The key to look up
    /// @return The value, or nullptr if not found
    [[nodiscard]] auto objectForInfoDictionaryKey(const String* pKey) const -> Object* {
        return Object::sendMessage<Object*>(
            this,
            Detail::Selectors::objectForInfoDictionaryKey(),
            pKey
        );
    }

    /// Get the bundle path
    /// @return File system path to the bundle
    [[nodiscard]] auto bundlePath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::bundlePath());
    }

    /// Get the bundle URL
    /// @return URL to the bundle
    [[nodiscard]] auto bundleURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::bundleURL());
    }

    /// Get the resource path
    /// @return Path to the Resources directory
    [[nodiscard]] auto resourcePath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::resourcePath());
    }

    /// Get the resource URL
    /// @return URL to the Resources directory
    [[nodiscard]] auto resourceURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::resourceURL());
    }

    /// Get the executable path
    /// @return Path to the main executable
    [[nodiscard]] auto executablePath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::executablePath());
    }

    /// Get the executable URL
    /// @return URL to the main executable
    [[nodiscard]] auto executableURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::executableURL());
    }

    /// Get the private frameworks path
    /// @return Path to the PrivateFrameworks directory
    [[nodiscard]] auto privateFrameworksPath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::privateFrameworksPath());
    }

    /// Get the private frameworks URL
    /// @return URL to the PrivateFrameworks directory
    [[nodiscard]] auto privateFrameworksURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::privateFrameworksURL());
    }

    /// Get the shared frameworks path
    /// @return Path to the SharedFrameworks directory
    [[nodiscard]] auto sharedFrameworksPath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::sharedFrameworksPath());
    }

    /// Get the shared frameworks URL
    /// @return URL to the SharedFrameworks directory
    [[nodiscard]] auto sharedFrameworksURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::sharedFrameworksURL());
    }

    /// Get the shared support path
    /// @return Path to the SharedSupport directory
    [[nodiscard]] auto sharedSupportPath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::sharedSupportPath());
    }

    /// Get the shared support URL
    /// @return URL to the SharedSupport directory
    [[nodiscard]] auto sharedSupportURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::sharedSupportURL());
    }

    /// Get the built-in plug-ins path
    /// @return Path to the PlugIns directory
    [[nodiscard]] auto builtInPlugInsPath() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::builtInPlugInsPath());
    }

    /// Get the built-in plug-ins URL
    /// @return URL to the PlugIns directory
    [[nodiscard]] auto builtInPlugInsURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::builtInPlugInsURL());
    }

    /// Get the App Store receipt URL
    /// @return URL to the receipt file
    [[nodiscard]] auto appStoreReceiptURL() const -> URL* {
        return Object::sendMessage<URL*>(this, Detail::Selectors::appStoreReceiptURL());
    }

    /// Get the path for an auxiliary executable
    /// @param pName Name of the auxiliary executable
    /// @return Path to the executable, or nullptr if not found
    [[nodiscard]] auto pathForAuxiliaryExecutable(const String* pName) const -> String* {
        return Object::sendMessage<String*>(
            this,
            Detail::Selectors::pathForAuxiliaryExecutable(),
            pName
        );
    }

    /// Get the URL for an auxiliary executable
    /// @param pName Name of the auxiliary executable
    /// @return URL to the executable, or nullptr if not found
    [[nodiscard]] auto URLForAuxiliaryExecutable(const String* pName) const -> URL* {
        return Object::sendMessage<URL*>(
            this,
            Detail::Selectors::URLForAuxiliaryExecutable(),
            pName
        );
    }

    /// Get a localized string
    /// @param pKey The key in the strings file
    /// @param pValue Default value if not found
    /// @param pTable The strings table name (nil for Localizable.strings)
    /// @return Localized string
    [[nodiscard]] auto localizedStringForKey(
        const String* pKey,
        const String* pValue,
        const String* pTable
    ) const -> String* {
        return Object::sendMessage<String*>(
            this,
            Detail::Selectors::localizedStringForKeyValueTable(),
            pKey,
            pValue,
            pTable
        );
    }
};
} // namespace v1
} // namespace NS
