/// @file collections.cppm
/// @brief Collection wrappers - Array, Dictionary, Set, Enumerator

export module foundationpp:collections;

import std;
import :types;
import :object;
import :detail;
import :runtime;
import :string;

export namespace NS {
inline namespace v1 {

// Forward declarations
template<ObjectType T = Object>
class Array;

template<ObjectType KeyType = Object, ObjectType ObjectType = Object>
class Dictionary;

/// Iterator for Foundation collections
/// @tparam T The element type
template<ObjectType T = Object>
class Enumerator final : public Referencing<Enumerator<T>> {
public:
    /// Get the next object in the enumeration
    /// @return The next object, or nullptr when exhausted
    [[nodiscard]] auto nextObject() -> T* {
        return Object::sendMessage<T*>(this, Detail::Selectors::nextObject());
    }

    /// Get all remaining objects as an array
    /// @return Array containing remaining objects
    [[nodiscard]] auto allObjects() -> Array<T>* {
        return Object::sendMessage<Array<T>*>(this, Detail::Selectors::allObjects());
    }
};

/// Wrapper for NSArray - ordered collection of objects
/// @tparam T The element type (defaults to Object)
template<ObjectType T>
class Array final : public Copying<Array<T>> {
public:
    /// Create an empty array
    /// @return Autoreleased empty array
    [[nodiscard]] static auto array() -> Array* {
        return Object::sendMessage<Array*>(Detail::Classes::NSArray(), Detail::Selectors::array());
    }

    /// Create an array containing a single object
    /// @param pObject The object to add
    /// @return Autoreleased array with one element
    [[nodiscard]] static auto arrayWithObject(const T* pObject) -> Array* {
        return Object::sendMessage<Array*>(
            Detail::Classes::NSArray(),
            Detail::Selectors::arrayWithObject(),
            pObject
        );
    }

    /// Create an array from a C array of objects
    /// @param pObjects Pointer to array of object pointers
    /// @param count Number of objects
    /// @return Autoreleased array
    [[nodiscard]] static auto arrayWithObjects(const T* const* pObjects, UInteger count) -> Array* {
        return Object::sendMessage<Array*>(
            Detail::Classes::NSArray(),
            Detail::Selectors::arrayWithObjectsCount(),
            pObjects,
            count
        );
    }

    /// Allocate an uninitialized array
    /// @return Allocated array (must be initialized)
    [[nodiscard]] static auto alloc() -> Array* {
        return Object::alloc<Array>(Detail::Classes::NSArray());
    }

    /// Initialize as empty array
    /// @return Initialized empty array
    [[nodiscard]] auto init() -> Array* {
        return Object::init<Array>();
    }

    /// Initialize with objects from a C array
    /// @param pObjects Pointer to array of object pointers
    /// @param count Number of objects
    /// @return Initialized array
    [[nodiscard]] auto init(const T* const* pObjects, UInteger count) -> Array* {
        return Object::sendMessage<Array*>(
            this,
            Detail::Selectors::initWithObjectsCount(),
            pObjects,
            count
        );
    }

    /// Get the number of elements
    /// @return Element count
    [[nodiscard]] auto count() const -> UInteger {
        return Object::sendMessage<UInteger>(this, Detail::Selectors::count());
    }

    /// Get the object at a specific index
    /// @param index Zero-based index
    /// @return The object at that index
    [[nodiscard]] auto objectAtIndex(UInteger index) const -> T* {
        return Object::sendMessage<T*>(this, Detail::Selectors::objectAtIndex(), index);
    }

    /// Array subscript operator
    /// @param index Zero-based index
    /// @return The object at that index
    [[nodiscard]] auto operator[](UInteger index) const -> T* {
        return objectAtIndex(index);
    }

    /// Get an enumerator for iterating over objects
    /// @return Object enumerator
    [[nodiscard]] auto objectEnumerator() const -> Enumerator<T>* {
        return Object::sendMessage<Enumerator<T>*>(this, Detail::Selectors::objectEnumerator());
    }

    /// Check if the array is empty
    /// @return true if count is 0
    [[nodiscard]] auto empty() const -> bool {
        return count() == 0;
    }

    /// Get the first object
    /// @return First object, or nullptr if empty
    [[nodiscard]] auto firstObject() const -> T* {
        return count() > 0 ? objectAtIndex(0) : nullptr;
    }

    /// Get the last object
    /// @return Last object, or nullptr if empty
    [[nodiscard]] auto lastObject() const -> T* {
        auto c = count();
        return c > 0 ? objectAtIndex(c - 1) : nullptr;
    }
};

/// Wrapper for NSDictionary - key-value mapping
/// @tparam KeyType The key type (defaults to Object)
/// @tparam ValueType The value type (defaults to Object)
template<ObjectType KeyType, ObjectType ValueType>
class Dictionary final : public Copying<Dictionary<KeyType, ValueType>> {
public:
    /// Create an empty dictionary
    /// @return Autoreleased empty dictionary
    [[nodiscard]] static auto dictionary() -> Dictionary* {
        return Object::sendMessage<Dictionary*>(
            Detail::Classes::NSDictionary(),
            Detail::Selectors::dictionary()
        );
    }

    /// Create a dictionary with a single key-value pair
    /// @param pObject The value
    /// @param pKey The key
    /// @return Autoreleased dictionary
    [[nodiscard]] static auto dictionaryWithObject(const ValueType* pObject, const KeyType* pKey)
        -> Dictionary* {
        return Object::sendMessage<Dictionary*>(
            Detail::Classes::NSDictionary(),
            Detail::Selectors::dictionaryWithObjectForKey(),
            pObject,
            pKey
        );
    }

    /// Create a dictionary from parallel arrays of keys and values
    /// @param pObjects Pointer to array of value pointers
    /// @param pKeys Pointer to array of key pointers
    /// @param count Number of key-value pairs
    /// @return Autoreleased dictionary
    [[nodiscard]] static auto dictionaryWithObjects(
        const ValueType* const* pObjects,
        const KeyType* const* pKeys,
        UInteger count
    ) -> Dictionary* {
        return Object::sendMessage<Dictionary*>(
            Detail::Classes::NSDictionary(),
            Detail::Selectors::dictionaryWithObjectsForKeysCount(),
            pObjects,
            pKeys,
            count
        );
    }

    /// Allocate an uninitialized dictionary
    /// @return Allocated dictionary (must be initialized)
    [[nodiscard]] static auto alloc() -> Dictionary* {
        return Object::alloc<Dictionary>(Detail::Classes::NSDictionary());
    }

    /// Initialize as empty dictionary
    /// @return Initialized empty dictionary
    [[nodiscard]] auto init() -> Dictionary* {
        return Object::init<Dictionary>();
    }

    /// Initialize with key-value pairs from parallel arrays
    /// @param pObjects Pointer to array of value pointers
    /// @param pKeys Pointer to array of key pointers
    /// @param count Number of key-value pairs
    /// @return Initialized dictionary
    [[nodiscard]] auto init(
        const ValueType* const* pObjects,
        const KeyType* const* pKeys,
        UInteger count
    ) -> Dictionary* {
        return Object::sendMessage<Dictionary*>(
            this,
            Detail::Selectors::initWithObjectsForKeysCount(),
            pObjects,
            pKeys,
            count
        );
    }

    /// Get the number of key-value pairs
    /// @return Count of entries
    [[nodiscard]] auto count() const -> UInteger {
        return Object::sendMessage<UInteger>(this, Detail::Selectors::count());
    }

    /// Get the value for a key
    /// @param pKey The key to look up
    /// @return The associated value, or nullptr if not found
    [[nodiscard]] auto objectForKey(const KeyType* pKey) const -> ValueType* {
        return Object::sendMessage<ValueType*>(this, Detail::Selectors::objectForKey(), pKey);
    }

    /// Dictionary subscript operator
    /// @param pKey The key to look up
    /// @return The associated value, or nullptr if not found
    [[nodiscard]] auto operator[](const KeyType* pKey) const -> ValueType* {
        return objectForKey(pKey);
    }

    /// Get an enumerator for iterating over keys
    /// @return Key enumerator
    [[nodiscard]] auto keyEnumerator() const -> Enumerator<KeyType>* {
        return Object::sendMessage<Enumerator<KeyType>*>(this, Detail::Selectors::keyEnumerator());
    }

    /// Get an enumerator for iterating over values
    /// @return Object enumerator
    [[nodiscard]] auto objectEnumerator() const -> Enumerator<ValueType>* {
        return Object::sendMessage<Enumerator<ValueType>*>(this, Detail::Selectors::objectEnumerator());
    }

    /// Check if the dictionary is empty
    /// @return true if count is 0
    [[nodiscard]] auto empty() const -> bool {
        return count() == 0;
    }
};

/// Wrapper for NSSet - unordered collection of unique objects
/// @tparam T The element type (defaults to Object)
template<ObjectType T = Object>
class Set final : public Copying<Set<T>> {
public:
    /// Allocate an uninitialized set
    /// @return Allocated set (must be initialized)
    [[nodiscard]] static auto alloc() -> Set* {
        return Object::alloc<Set>(Detail::Classes::NSSet());
    }

    /// Initialize as empty set
    /// @return Initialized empty set
    [[nodiscard]] auto init() -> Set* {
        return Object::init<Set>();
    }

    /// Initialize with objects from a C array
    /// @param pObjects Pointer to array of object pointers
    /// @param count Number of objects
    /// @return Initialized set
    [[nodiscard]] auto init(const T* const* pObjects, UInteger count) -> Set* {
        return Object::sendMessage<Set*>(
            this,
            Detail::Selectors::initWithObjectsCount(),
            pObjects,
            count
        );
    }

    /// Get the number of elements
    /// @return Element count
    [[nodiscard]] auto count() const -> UInteger {
        return Object::sendMessage<UInteger>(this, Detail::Selectors::count());
    }

    /// Get all objects as an array
    /// @return Array containing all objects
    [[nodiscard]] auto allObjects() const -> Array<T>* {
        return Object::sendMessage<Array<T>*>(this, Detail::Selectors::allObjects());
    }

    /// Get an enumerator for iterating over objects
    /// @return Object enumerator
    [[nodiscard]] auto objectEnumerator() const -> Enumerator<T>* {
        return Object::sendMessage<Enumerator<T>*>(this, Detail::Selectors::objectEnumerator());
    }

    /// Check if the set is empty
    /// @return true if count is 0
    [[nodiscard]] auto empty() const -> bool {
        return count() == 0;
    }
};

} // namespace v1
} // namespace NS
