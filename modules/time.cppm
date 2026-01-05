/// @file time.cppm
/// @brief Date wrapper

export module foundationpp:time;

import std;
import :types;
import :object;
import :detail;
import :runtime;

export namespace NS {
inline namespace v1 {
/// Wrapper for NSDate - represents a point in time
class Date final : public Copying<Date> {
public:
    /// Create a date with the given time interval from now
    /// @param interval Seconds from the current date/time
    /// @return Autoreleased date
    [[nodiscard]] static auto dateWithTimeIntervalSinceNow(TimeInterval interval) -> Date* {
        return Object::sendMessage<Date*>(
            Detail::Classes::NSDate(),
            Detail::Selectors::dateWithTimeIntervalSinceNow(),
            interval
        );
    }

    /// Create a date representing the current moment
    /// @return Autoreleased date for now
    [[nodiscard]] static auto now() -> Date* {
        return dateWithTimeIntervalSinceNow(0.0);
    }

    /// Allocate an uninitialized date
    /// @return Allocated date (must be initialized)
    [[nodiscard]] static auto alloc() -> Date* {
        return Object::alloc<Date>(Detail::Classes::NSDate());
    }

    /// Initialize with the current date/time
    /// @return Initialized date for now
    [[nodiscard]] auto init() -> Date* {
        return Object::init<Date>();
    }
};
} // namespace v1
} // namespace NS
