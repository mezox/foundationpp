/// @file detail.cppm
/// @brief Internal implementation details for foundationpp - selector and class caching
/// @note This partition is NOT exported from the primary module interface

module;

#include <objc/runtime.h>
#include <objc/message.h>
#include <dlfcn.h>

export module foundationpp:detail;

namespace NS::Detail {
inline namespace v1 {

/// Pre-defined selectors for common Foundation methods
/// Uses lazy initialization via static local variables for thread-safety
struct Selectors final {
    Selectors() = delete;

    // Memory management
    [[nodiscard]] static auto alloc() -> SEL {
        static auto sel = sel_registerName("alloc");
        return sel;
    }

    [[nodiscard]] static auto init() -> SEL {
        static auto sel = sel_registerName("init");
        return sel;
    }

    [[nodiscard]] static auto retain() -> SEL {
        static auto sel = sel_registerName("retain");
        return sel;
    }

    [[nodiscard]] static auto release() -> SEL {
        static auto sel = sel_registerName("release");
        return sel;
    }

    [[nodiscard]] static auto autorelease() -> SEL {
        static auto sel = sel_registerName("autorelease");
        return sel;
    }

    [[nodiscard]] static auto retainCount() -> SEL {
        static auto sel = sel_registerName("retainCount");
        return sel;
    }

    [[nodiscard]] static auto copy() -> SEL {
        static auto sel = sel_registerName("copy");
        return sel;
    }

    [[nodiscard]] static auto drain() -> SEL {
        static auto sel = sel_registerName("drain");
        return sel;
    }

    // Object methods
    [[nodiscard]] static auto hash() -> SEL {
        static auto sel = sel_registerName("hash");
        return sel;
    }

    [[nodiscard]] static auto isEqual() -> SEL {
        static auto sel = sel_registerName("isEqual:");
        return sel;
    }

    [[nodiscard]] static auto description() -> SEL {
        static auto sel = sel_registerName("description");
        return sel;
    }

    [[nodiscard]] static auto debugDescription() -> SEL {
        static auto sel = sel_registerName("debugDescription");
        return sel;
    }

    [[nodiscard]] static auto respondsToSelector() -> SEL {
        static auto sel = sel_registerName("respondsToSelector:");
        return sel;
    }

    [[nodiscard]] static auto methodSignatureForSelector() -> SEL {
        static auto sel = sel_registerName("methodSignatureForSelector:");
        return sel;
    }

    // String methods
    [[nodiscard]] static auto string() -> SEL {
        static auto sel = sel_registerName("string");
        return sel;
    }

    [[nodiscard]] static auto stringWithString() -> SEL {
        static auto sel = sel_registerName("stringWithString:");
        return sel;
    }

    [[nodiscard]] static auto stringWithCStringEncoding() -> SEL {
        static auto sel = sel_registerName("stringWithCString:encoding:");
        return sel;
    }

    [[nodiscard]] static auto initWithCStringEncoding() -> SEL {
        static auto sel = sel_registerName("initWithCString:encoding:");
        return sel;
    }

    [[nodiscard]] static auto initWithBytesNoCopyLengthEncodingFreeWhenDone() -> SEL {
        static auto sel = sel_registerName("initWithBytesNoCopy:length:encoding:freeWhenDone:");
        return sel;
    }

    [[nodiscard]] static auto initWithString() -> SEL {
        static auto sel = sel_registerName("initWithString:");
        return sel;
    }

    [[nodiscard]] static auto length() -> SEL {
        static auto sel = sel_registerName("length");
        return sel;
    }

    [[nodiscard]] static auto UTF8String() -> SEL {
        static auto sel = sel_registerName("UTF8String");
        return sel;
    }

    [[nodiscard]] static auto cStringUsingEncoding() -> SEL {
        static auto sel = sel_registerName("cStringUsingEncoding:");
        return sel;
    }

    [[nodiscard]] static auto characterAtIndex() -> SEL {
        static auto sel = sel_registerName("characterAtIndex:");
        return sel;
    }

    [[nodiscard]] static auto lengthOfBytesUsingEncoding() -> SEL {
        static auto sel = sel_registerName("lengthOfBytesUsingEncoding:");
        return sel;
    }

    [[nodiscard]] static auto maximumLengthOfBytesUsingEncoding() -> SEL {
        static auto sel = sel_registerName("maximumLengthOfBytesUsingEncoding:");
        return sel;
    }

    [[nodiscard]] static auto isEqualToString() -> SEL {
        static auto sel = sel_registerName("isEqualToString:");
        return sel;
    }

    [[nodiscard]] static auto compare() -> SEL {
        static auto sel = sel_registerName("compare:");
        return sel;
    }

    [[nodiscard]] static auto caseInsensitiveCompare() -> SEL {
        static auto sel = sel_registerName("caseInsensitiveCompare:");
        return sel;
    }

    [[nodiscard]] static auto rangeOfStringOptions() -> SEL {
        static auto sel = sel_registerName("rangeOfString:options:");
        return sel;
    }

    [[nodiscard]] static auto stringByAppendingString() -> SEL {
        static auto sel = sel_registerName("stringByAppendingString:");
        return sel;
    }

    // Array methods
    [[nodiscard]] static auto array() -> SEL {
        static auto sel = sel_registerName("array");
        return sel;
    }

    [[nodiscard]] static auto arrayWithObject() -> SEL {
        static auto sel = sel_registerName("arrayWithObject:");
        return sel;
    }

    [[nodiscard]] static auto arrayWithObjectsCount() -> SEL {
        static auto sel = sel_registerName("arrayWithObjects:count:");
        return sel;
    }

    [[nodiscard]] static auto initWithObjectsCount() -> SEL {
        static auto sel = sel_registerName("initWithObjects:count:");
        return sel;
    }

    [[nodiscard]] static auto count() -> SEL {
        static auto sel = sel_registerName("count");
        return sel;
    }

    [[nodiscard]] static auto objectAtIndex() -> SEL {
        static auto sel = sel_registerName("objectAtIndex:");
        return sel;
    }

    [[nodiscard]] static auto objectEnumerator() -> SEL {
        static auto sel = sel_registerName("objectEnumerator");
        return sel;
    }

    [[nodiscard]] static auto addObject() -> SEL {
        static auto sel = sel_registerName("addObject:");
        return sel;
    }

    // Dictionary methods
    [[nodiscard]] static auto dictionary() -> SEL {
        static auto sel = sel_registerName("dictionary");
        return sel;
    }

    [[nodiscard]] static auto dictionaryWithObjectForKey() -> SEL {
        static auto sel = sel_registerName("dictionaryWithObject:forKey:");
        return sel;
    }

    [[nodiscard]] static auto dictionaryWithObjectsForKeysCount() -> SEL {
        static auto sel = sel_registerName("dictionaryWithObjects:forKeys:count:");
        return sel;
    }

    [[nodiscard]] static auto initWithObjectsForKeysCount() -> SEL {
        static auto sel = sel_registerName("initWithObjects:forKeys:count:");
        return sel;
    }

    [[nodiscard]] static auto objectForKey() -> SEL {
        static auto sel = sel_registerName("objectForKey:");
        return sel;
    }

    [[nodiscard]] static auto keyEnumerator() -> SEL {
        static auto sel = sel_registerName("keyEnumerator");
        return sel;
    }

    [[nodiscard]] static auto allObjects() -> SEL {
        static auto sel = sel_registerName("allObjects");
        return sel;
    }

    // Set methods (allObjects already defined above)

    // Enumerator methods
    [[nodiscard]] static auto nextObject() -> SEL {
        static auto sel = sel_registerName("nextObject");
        return sel;
    }

    [[nodiscard]] static auto countByEnumeratingWithStateObjectsCount() -> SEL {
        static auto sel = sel_registerName("countByEnumeratingWithState:objects:count:");
        return sel;
    }

    // Data methods
    [[nodiscard]] static auto bytes() -> SEL {
        static auto sel = sel_registerName("bytes");
        return sel;
    }

    [[nodiscard]] static auto mutableBytes() -> SEL {
        static auto sel = sel_registerName("mutableBytes");
        return sel;
    }

    // URL methods
    [[nodiscard]] static auto fileURLWithPath() -> SEL {
        static auto sel = sel_registerName("fileURLWithPath:");
        return sel;
    }

    [[nodiscard]] static auto initWithString_URL() -> SEL {
        static auto sel = sel_registerName("initWithString:");
        return sel;
    }

    [[nodiscard]] static auto initFileURLWithPath() -> SEL {
        static auto sel = sel_registerName("initFileURLWithPath:");
        return sel;
    }

    [[nodiscard]] static auto initWithURL() -> SEL {
        static auto sel = sel_registerName("initWithURL:");
        return sel;
    }

    [[nodiscard]] static auto fileSystemRepresentation() -> SEL {
        static auto sel = sel_registerName("fileSystemRepresentation");
        return sel;
    }

    // Error methods
    [[nodiscard]] static auto errorWithDomainCodeUserInfo() -> SEL {
        static auto sel = sel_registerName("errorWithDomain:code:userInfo:");
        return sel;
    }

    [[nodiscard]] static auto initWithDomainCodeUserInfo() -> SEL {
        static auto sel = sel_registerName("initWithDomain:code:userInfo:");
        return sel;
    }

    [[nodiscard]] static auto code() -> SEL {
        static auto sel = sel_registerName("code");
        return sel;
    }

    [[nodiscard]] static auto domain() -> SEL {
        static auto sel = sel_registerName("domain");
        return sel;
    }

    [[nodiscard]] static auto userInfo() -> SEL {
        static auto sel = sel_registerName("userInfo");
        return sel;
    }

    [[nodiscard]] static auto localizedDescription() -> SEL {
        static auto sel = sel_registerName("localizedDescription");
        return sel;
    }

    [[nodiscard]] static auto localizedFailureReason() -> SEL {
        static auto sel = sel_registerName("localizedFailureReason");
        return sel;
    }

    [[nodiscard]] static auto localizedRecoverySuggestion() -> SEL {
        static auto sel = sel_registerName("localizedRecoverySuggestion");
        return sel;
    }

    [[nodiscard]] static auto localizedRecoveryOptions() -> SEL {
        static auto sel = sel_registerName("localizedRecoveryOptions");
        return sel;
    }

    // Date methods
    [[nodiscard]] static auto dateWithTimeIntervalSinceNow() -> SEL {
        static auto sel = sel_registerName("dateWithTimeIntervalSinceNow:");
        return sel;
    }

    // Number methods
    [[nodiscard]] static auto numberWithBool() -> SEL {
        static auto sel = sel_registerName("numberWithBool:");
        return sel;
    }

    [[nodiscard]] static auto numberWithChar() -> SEL {
        static auto sel = sel_registerName("numberWithChar:");
        return sel;
    }

    [[nodiscard]] static auto numberWithShort() -> SEL {
        static auto sel = sel_registerName("numberWithShort:");
        return sel;
    }

    [[nodiscard]] static auto numberWithInt() -> SEL {
        static auto sel = sel_registerName("numberWithInt:");
        return sel;
    }

    [[nodiscard]] static auto numberWithLong() -> SEL {
        static auto sel = sel_registerName("numberWithLong:");
        return sel;
    }

    [[nodiscard]] static auto numberWithLongLong() -> SEL {
        static auto sel = sel_registerName("numberWithLongLong:");
        return sel;
    }

    [[nodiscard]] static auto numberWithFloat() -> SEL {
        static auto sel = sel_registerName("numberWithFloat:");
        return sel;
    }

    [[nodiscard]] static auto numberWithDouble() -> SEL {
        static auto sel = sel_registerName("numberWithDouble:");
        return sel;
    }

    [[nodiscard]] static auto numberWithUnsignedChar() -> SEL {
        static auto sel = sel_registerName("numberWithUnsignedChar:");
        return sel;
    }

    [[nodiscard]] static auto numberWithUnsignedShort() -> SEL {
        static auto sel = sel_registerName("numberWithUnsignedShort:");
        return sel;
    }

    [[nodiscard]] static auto numberWithUnsignedInt() -> SEL {
        static auto sel = sel_registerName("numberWithUnsignedInt:");
        return sel;
    }

    [[nodiscard]] static auto numberWithUnsignedLong() -> SEL {
        static auto sel = sel_registerName("numberWithUnsignedLong:");
        return sel;
    }

    [[nodiscard]] static auto numberWithUnsignedLongLong() -> SEL {
        static auto sel = sel_registerName("numberWithUnsignedLongLong:");
        return sel;
    }

    [[nodiscard]] static auto boolValue() -> SEL {
        static auto sel = sel_registerName("boolValue");
        return sel;
    }

    [[nodiscard]] static auto charValue() -> SEL {
        static auto sel = sel_registerName("charValue");
        return sel;
    }

    [[nodiscard]] static auto shortValue() -> SEL {
        static auto sel = sel_registerName("shortValue");
        return sel;
    }

    [[nodiscard]] static auto intValue() -> SEL {
        static auto sel = sel_registerName("intValue");
        return sel;
    }

    [[nodiscard]] static auto longValue() -> SEL {
        static auto sel = sel_registerName("longValue");
        return sel;
    }

    [[nodiscard]] static auto longLongValue() -> SEL {
        static auto sel = sel_registerName("longLongValue");
        return sel;
    }

    [[nodiscard]] static auto floatValue() -> SEL {
        static auto sel = sel_registerName("floatValue");
        return sel;
    }

    [[nodiscard]] static auto doubleValue() -> SEL {
        static auto sel = sel_registerName("doubleValue");
        return sel;
    }

    [[nodiscard]] static auto unsignedCharValue() -> SEL {
        static auto sel = sel_registerName("unsignedCharValue");
        return sel;
    }

    [[nodiscard]] static auto unsignedShortValue() -> SEL {
        static auto sel = sel_registerName("unsignedShortValue");
        return sel;
    }

    [[nodiscard]] static auto unsignedIntValue() -> SEL {
        static auto sel = sel_registerName("unsignedIntValue");
        return sel;
    }

    [[nodiscard]] static auto unsignedLongValue() -> SEL {
        static auto sel = sel_registerName("unsignedLongValue");
        return sel;
    }

    [[nodiscard]] static auto unsignedLongLongValue() -> SEL {
        static auto sel = sel_registerName("unsignedLongLongValue");
        return sel;
    }

    [[nodiscard]] static auto integerValue() -> SEL {
        static auto sel = sel_registerName("integerValue");
        return sel;
    }

    [[nodiscard]] static auto unsignedIntegerValue() -> SEL {
        static auto sel = sel_registerName("unsignedIntegerValue");
        return sel;
    }

    [[nodiscard]] static auto stringValue() -> SEL {
        static auto sel = sel_registerName("stringValue");
        return sel;
    }

    [[nodiscard]] static auto isEqualToNumber() -> SEL {
        static auto sel = sel_registerName("isEqualToNumber:");
        return sel;
    }

    // Value methods
    [[nodiscard]] static auto valueWithBytesObjCType() -> SEL {
        static auto sel = sel_registerName("valueWithBytes:objCType:");
        return sel;
    }

    [[nodiscard]] static auto valueWithPointer() -> SEL {
        static auto sel = sel_registerName("valueWithPointer:");
        return sel;
    }

    [[nodiscard]] static auto initWithBytesObjCType() -> SEL {
        static auto sel = sel_registerName("initWithBytes:objCType:");
        return sel;
    }

    [[nodiscard]] static auto getValueSize() -> SEL {
        static auto sel = sel_registerName("getValue:size:");
        return sel;
    }

    [[nodiscard]] static auto objCType() -> SEL {
        static auto sel = sel_registerName("objCType");
        return sel;
    }

    [[nodiscard]] static auto pointerValue() -> SEL {
        static auto sel = sel_registerName("pointerValue");
        return sel;
    }

    [[nodiscard]] static auto isEqualToValue() -> SEL {
        static auto sel = sel_registerName("isEqualToValue:");
        return sel;
    }

    // Bundle methods
    [[nodiscard]] static auto mainBundle() -> SEL {
        static auto sel = sel_registerName("mainBundle");
        return sel;
    }

    [[nodiscard]] static auto bundleWithPath() -> SEL {
        static auto sel = sel_registerName("bundleWithPath:");
        return sel;
    }

    [[nodiscard]] static auto bundleWithURL() -> SEL {
        static auto sel = sel_registerName("bundleWithURL:");
        return sel;
    }

    [[nodiscard]] static auto allBundles() -> SEL {
        static auto sel = sel_registerName("allBundles");
        return sel;
    }

    [[nodiscard]] static auto allFrameworks() -> SEL {
        static auto sel = sel_registerName("allFrameworks");
        return sel;
    }

    [[nodiscard]] static auto initWithPath() -> SEL {
        static auto sel = sel_registerName("initWithPath:");
        return sel;
    }

    [[nodiscard]] static auto load() -> SEL {
        static auto sel = sel_registerName("load");
        return sel;
    }

    [[nodiscard]] static auto loadAndReturnError() -> SEL {
        static auto sel = sel_registerName("loadAndReturnError:");
        return sel;
    }

    [[nodiscard]] static auto unload() -> SEL {
        static auto sel = sel_registerName("unload");
        return sel;
    }

    [[nodiscard]] static auto isLoaded() -> SEL {
        static auto sel = sel_registerName("isLoaded");
        return sel;
    }

    [[nodiscard]] static auto preflightAndReturnError() -> SEL {
        static auto sel = sel_registerName("preflightAndReturnError:");
        return sel;
    }

    [[nodiscard]] static auto bundleIdentifier() -> SEL {
        static auto sel = sel_registerName("bundleIdentifier");
        return sel;
    }

    [[nodiscard]] static auto bundlePath() -> SEL {
        static auto sel = sel_registerName("bundlePath");
        return sel;
    }

    [[nodiscard]] static auto bundleURL() -> SEL {
        static auto sel = sel_registerName("bundleURL");
        return sel;
    }

    [[nodiscard]] static auto resourcePath() -> SEL {
        static auto sel = sel_registerName("resourcePath");
        return sel;
    }

    [[nodiscard]] static auto resourceURL() -> SEL {
        static auto sel = sel_registerName("resourceURL");
        return sel;
    }

    [[nodiscard]] static auto executablePath() -> SEL {
        static auto sel = sel_registerName("executablePath");
        return sel;
    }

    [[nodiscard]] static auto executableURL() -> SEL {
        static auto sel = sel_registerName("executableURL");
        return sel;
    }

    [[nodiscard]] static auto privateFrameworksPath() -> SEL {
        static auto sel = sel_registerName("privateFrameworksPath");
        return sel;
    }

    [[nodiscard]] static auto privateFrameworksURL() -> SEL {
        static auto sel = sel_registerName("privateFrameworksURL");
        return sel;
    }

    [[nodiscard]] static auto sharedFrameworksPath() -> SEL {
        static auto sel = sel_registerName("sharedFrameworksPath");
        return sel;
    }

    [[nodiscard]] static auto sharedFrameworksURL() -> SEL {
        static auto sel = sel_registerName("sharedFrameworksURL");
        return sel;
    }

    [[nodiscard]] static auto sharedSupportPath() -> SEL {
        static auto sel = sel_registerName("sharedSupportPath");
        return sel;
    }

    [[nodiscard]] static auto sharedSupportURL() -> SEL {
        static auto sel = sel_registerName("sharedSupportURL");
        return sel;
    }

    [[nodiscard]] static auto builtInPlugInsPath() -> SEL {
        static auto sel = sel_registerName("builtInPlugInsPath");
        return sel;
    }

    [[nodiscard]] static auto builtInPlugInsURL() -> SEL {
        static auto sel = sel_registerName("builtInPlugInsURL");
        return sel;
    }

    [[nodiscard]] static auto appStoreReceiptURL() -> SEL {
        static auto sel = sel_registerName("appStoreReceiptURL");
        return sel;
    }

    [[nodiscard]] static auto pathForAuxiliaryExecutable() -> SEL {
        static auto sel = sel_registerName("pathForAuxiliaryExecutable:");
        return sel;
    }

    [[nodiscard]] static auto URLForAuxiliaryExecutable() -> SEL {
        static auto sel = sel_registerName("URLForAuxiliaryExecutable:");
        return sel;
    }

    [[nodiscard]] static auto infoDictionary() -> SEL {
        static auto sel = sel_registerName("infoDictionary");
        return sel;
    }

    [[nodiscard]] static auto localizedInfoDictionary() -> SEL {
        static auto sel = sel_registerName("localizedInfoDictionary");
        return sel;
    }

    [[nodiscard]] static auto objectForInfoDictionaryKey() -> SEL {
        static auto sel = sel_registerName("objectForInfoDictionaryKey:");
        return sel;
    }

    [[nodiscard]] static auto localizedStringForKeyValueTable() -> SEL {
        static auto sel = sel_registerName("localizedStringForKey:value:table:");
        return sel;
    }

    // ProcessInfo methods
    [[nodiscard]] static auto processInfo() -> SEL {
        static auto sel = sel_registerName("processInfo");
        return sel;
    }

    [[nodiscard]] static auto processName() -> SEL {
        static auto sel = sel_registerName("processName");
        return sel;
    }

    [[nodiscard]] static auto setProcessName() -> SEL {
        static auto sel = sel_registerName("setProcessName:");
        return sel;
    }

    [[nodiscard]] static auto processIdentifier() -> SEL {
        static auto sel = sel_registerName("processIdentifier");
        return sel;
    }

    [[nodiscard]] static auto environment() -> SEL {
        static auto sel = sel_registerName("environment");
        return sel;
    }

    [[nodiscard]] static auto arguments() -> SEL {
        static auto sel = sel_registerName("arguments");
        return sel;
    }

    [[nodiscard]] static auto hostName() -> SEL {
        static auto sel = sel_registerName("hostName");
        return sel;
    }

    [[nodiscard]] static auto operatingSystemVersionString() -> SEL {
        static auto sel = sel_registerName("operatingSystemVersionString");
        return sel;
    }

    [[nodiscard]] static auto operatingSystemVersion() -> SEL {
        static auto sel = sel_registerName("operatingSystemVersion");
        return sel;
    }

    [[nodiscard]] static auto processorCount() -> SEL {
        static auto sel = sel_registerName("processorCount");
        return sel;
    }

    [[nodiscard]] static auto activeProcessorCount() -> SEL {
        static auto sel = sel_registerName("activeProcessorCount");
        return sel;
    }

    [[nodiscard]] static auto physicalMemory() -> SEL {
        static auto sel = sel_registerName("physicalMemory");
        return sel;
    }

    [[nodiscard]] static auto isOperatingSystemAtLeastVersion() -> SEL {
        static auto sel = sel_registerName("isOperatingSystemAtLeastVersion:");
        return sel;
    }

    [[nodiscard]] static auto systemUptime() -> SEL {
        static auto sel = sel_registerName("systemUptime");
        return sel;
    }

    [[nodiscard]] static auto userName() -> SEL {
        static auto sel = sel_registerName("userName");
        return sel;
    }

    [[nodiscard]] static auto fullUserName() -> SEL {
        static auto sel = sel_registerName("fullUserName");
        return sel;
    }

    [[nodiscard]] static auto globallyUniqueString() -> SEL {
        static auto sel = sel_registerName("globallyUniqueString");
        return sel;
    }

    [[nodiscard]] static auto thermalState() -> SEL {
        static auto sel = sel_registerName("thermalState");
        return sel;
    }

    [[nodiscard]] static auto isLowPowerModeEnabled() -> SEL {
        static auto sel = sel_registerName("isLowPowerModeEnabled");
        return sel;
    }

    [[nodiscard]] static auto isMacCatalystApp() -> SEL {
        static auto sel = sel_registerName("isMacCatalystApp");
        return sel;
    }

    [[nodiscard]] static auto isiOSAppOnMac() -> SEL {
        static auto sel = sel_registerName("isiOSAppOnMac");
        return sel;
    }

    // Concurrency methods
    [[nodiscard]] static auto lock() -> SEL {
        static auto sel = sel_registerName("lock");
        return sel;
    }

    [[nodiscard]] static auto unlock() -> SEL {
        static auto sel = sel_registerName("unlock");
        return sel;
    }

    [[nodiscard]] static auto wait() -> SEL {
        static auto sel = sel_registerName("wait");
        return sel;
    }

    [[nodiscard]] static auto waitUntilDate() -> SEL {
        static auto sel = sel_registerName("waitUntilDate:");
        return sel;
    }

    [[nodiscard]] static auto signal() -> SEL {
        static auto sel = sel_registerName("signal");
        return sel;
    }

    [[nodiscard]] static auto broadcast() -> SEL {
        static auto sel = sel_registerName("broadcast");
        return sel;
    }

    [[nodiscard]] static auto showPools() -> SEL {
        static auto sel = sel_registerName("showPools");
        return sel;
    }

    // Notification methods
    [[nodiscard]] static auto name() -> SEL {
        static auto sel = sel_registerName("name");
        return sel;
    }

    [[nodiscard]] static auto object() -> SEL {
        static auto sel = sel_registerName("object");
        return sel;
    }

    [[nodiscard]] static auto defaultCenter() -> SEL {
        static auto sel = sel_registerName("defaultCenter");
        return sel;
    }

    [[nodiscard]] static auto addObserverNameObjectQueueBlock() -> SEL {
        static auto sel = sel_registerName("addObserverForName:object:queue:usingBlock:");
        return sel;
    }

    [[nodiscard]] static auto removeObserver() -> SEL {
        static auto sel = sel_registerName("removeObserver:");
        return sel;
    }
};

/// Pre-defined classes for Foundation types
struct Classes final {
    Classes() = delete;

    [[nodiscard]] static auto NSObject() -> Class {
        static auto cls = objc_lookUpClass("NSObject");
        return cls;
    }

    [[nodiscard]] static auto NSString() -> Class {
        static auto cls = objc_lookUpClass("NSString");
        return cls;
    }

    [[nodiscard]] static auto NSArray() -> Class {
        static auto cls = objc_lookUpClass("NSArray");
        return cls;
    }

    [[nodiscard]] static auto NSDictionary() -> Class {
        static auto cls = objc_lookUpClass("NSDictionary");
        return cls;
    }

    [[nodiscard]] static auto NSSet() -> Class {
        static auto cls = objc_lookUpClass("NSSet");
        return cls;
    }

    [[nodiscard]] static auto NSData() -> Class {
        static auto cls = objc_lookUpClass("NSData");
        return cls;
    }

    [[nodiscard]] static auto NSURL() -> Class {
        static auto cls = objc_lookUpClass("NSURL");
        return cls;
    }

    [[nodiscard]] static auto NSError() -> Class {
        static auto cls = objc_lookUpClass("NSError");
        return cls;
    }

    [[nodiscard]] static auto NSDate() -> Class {
        static auto cls = objc_lookUpClass("NSDate");
        return cls;
    }

    [[nodiscard]] static auto NSNumber() -> Class {
        static auto cls = objc_lookUpClass("NSNumber");
        return cls;
    }

    [[nodiscard]] static auto NSValue() -> Class {
        static auto cls = objc_lookUpClass("NSValue");
        return cls;
    }

    [[nodiscard]] static auto NSBundle() -> Class {
        static auto cls = objc_lookUpClass("NSBundle");
        return cls;
    }

    [[nodiscard]] static auto NSProcessInfo() -> Class {
        static auto cls = objc_lookUpClass("NSProcessInfo");
        return cls;
    }

    [[nodiscard]] static auto NSAutoreleasePool() -> Class {
        static auto cls = objc_lookUpClass("NSAutoreleasePool");
        return cls;
    }

    [[nodiscard]] static auto NSCondition() -> Class {
        static auto cls = objc_lookUpClass("NSCondition");
        return cls;
    }

    [[nodiscard]] static auto NSNotification() -> Class {
        static auto cls = objc_lookUpClass("NSNotification");
        return cls;
    }

    [[nodiscard]] static auto NSNotificationCenter() -> Class {
        static auto cls = objc_lookUpClass("NSNotificationCenter");
        return cls;
    }
};

} // namespace v1
} // namespace NS::Detail
