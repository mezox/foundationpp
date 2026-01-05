/// @file notification.cppm
/// @brief Notification and NotificationCenter wrappers

export module foundationpp:notification;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :string;
import :collections;

export namespace NS {
inline namespace v1 {

/// Notification name type
using NotificationName = String*;

/// Wrapper for NSNotification - an immutable notification object
class Notification final : public Copying<Notification> {
public:
    /// Get the notification name
    /// @return The name identifying this notification
    [[nodiscard]] auto name() const -> String* {
        return Object::sendMessage<String*>(this, Detail::Selectors::name());
    }

    /// Get the object associated with the notification
    /// @return The sender object, or nullptr
    [[nodiscard]] auto object() const -> Object* {
        return Object::sendMessage<Object*>(this, Detail::Selectors::object());
    }

    /// Get the user info dictionary
    /// @return Additional information, or nullptr
    [[nodiscard]] auto userInfo() const -> Dictionary<String, Object>* {
        return Object::sendMessage<Dictionary<String, Object>*>(
            this,
            Detail::Selectors::userInfo()
        );
    }
};

/// Opaque token returned when adding an observer
/// Used to remove the observer later
class ObserverToken final : public Referencing<ObserverToken> {
    // Opaque type wrapping the observer object returned by NSNotificationCenter
};

/// Wrapper for NSNotificationCenter - notification dispatch mechanism
class NotificationCenter final : public Referencing<NotificationCenter> {
public:
    /// Get the default notification center
    /// @return The shared notification center
    [[nodiscard]] static auto defaultCenter() -> NotificationCenter* {
        return Object::sendMessage<NotificationCenter*>(
            Detail::Classes::NSNotificationCenter(),
            Detail::Selectors::defaultCenter()
        );
    }

    /// Add an observer using a block
    /// @param pName Notification name to observe (nullptr for all)
    /// @param pObject Sender to observe (nullptr for all)
    /// @param pQueue Operation queue for callback (nullptr for posting thread)
    /// @param pBlock Block to call when notification is posted
    /// @return Observer token for later removal
    [[nodiscard]] auto addObserverForName(
        const String* pName,
        const Object* pObject,
        const void* pQueue, // NSOperationQueue*
        void (^pBlock)(Notification*)
    ) -> ObserverToken* {
        return Object::sendMessage<ObserverToken*>(
            this,
            Detail::Selectors::addObserverNameObjectQueueBlock(),
            pName,
            pObject,
            pQueue,
            pBlock
        );
    }

    /// Add an observer using a std::function
    /// @param pName Notification name to observe (nullptr for all)
    /// @param pObject Sender to observe (nullptr for all)
    /// @param callback Function to call when notification is posted
    /// @return Observer token for later removal
    /// @note This creates a block wrapper around the std::function
    [[nodiscard]] auto addObserver(
        const String* pName,
        const Object* pObject,
        std::function<void(Notification*)> callback
    ) -> ObserverToken* {
        // Wrap std::function in a block
        auto block = ^(Notification* notification) {
            callback(notification);
        };
        return addObserverForName(pName, pObject, nullptr, block);
    }

    /// Remove an observer
    /// @param pObserver The observer token to remove
    auto removeObserver(ObserverToken* pObserver) -> void {
        Object::sendMessage<void>(this, Detail::Selectors::removeObserver(), pObserver);
    }
};

/// RAII guard that automatically removes an observer on destruction
class ObserverGuard {
public:
    /// Create a guard for an observer
    /// @param center The notification center
    /// @param token The observer token
    ObserverGuard(NotificationCenter* center, ObserverToken* token)
        : center_{center}, token_{token} {}

    /// Remove the observer
    ~ObserverGuard() {
        if (center_ && token_) {
            center_->removeObserver(token_);
        }
    }

    // Non-copyable
    ObserverGuard(const ObserverGuard&) = delete;
    ObserverGuard& operator=(const ObserverGuard&) = delete;

    // Movable
    ObserverGuard(ObserverGuard&& other) noexcept
        : center_{std::exchange(other.center_, nullptr)}
        , token_{std::exchange(other.token_, nullptr)} {}

    ObserverGuard& operator=(ObserverGuard&& other) noexcept {
        if (this != &other) {
            if (center_ && token_) {
                center_->removeObserver(token_);
            }
            center_ = std::exchange(other.center_, nullptr);
            token_ = std::exchange(other.token_, nullptr);
        }
        return *this;
    }

    /// Get the observer token
    [[nodiscard]] auto token() const noexcept -> ObserverToken* {
        return token_;
    }

    /// Release the guard without removing the observer
    auto release() noexcept -> ObserverToken* {
        center_ = nullptr;
        return std::exchange(token_, nullptr);
    }

private:
    NotificationCenter* center_;
    ObserverToken* token_;
};

} // namespace v1
} // namespace NS
