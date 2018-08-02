// © 2017 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html

// char16ptr.h
// created: 2017feb28 Markus W. Scherer

#ifndef __CHAR16PTR_H__
#define __CHAR16PTR_H__

#include <cstddef>
#include "unicode/utypes.h"

/**
 * \file
 * \brief C++ API: UChar pointer wrappers with
 *        implicit conversion from bit-compatible raw pointer types.
 *        Also conversion functions from UChar * to UChar * and OldUChar *.
 */

U_NAMESPACE_BEGIN

/**
 * \def U_ALIASING_BARRIER
 * Barrier for pointer anti-aliasing optimizations even across function boundaries.
 * @internal
 */
#ifdef U_ALIASING_BARRIER
    // Use the predefined value.
#elif (defined(__clang__) || defined(__GNUC__)) && U_PLATFORM != U_PF_BROWSER_NATIVE_CLIENT
#   define U_ALIASING_BARRIER(ptr) asm volatile("" : : "rm"(ptr) : "memory")
#elif defined(U_IN_DOXYGEN)
#   define U_ALIASING_BARRIER(ptr)
#endif

/**
 * UChar * wrapper with implicit conversion from distinct but bit-compatible pointer types.
 * @stable ICU 59
 */
class U_COMMON_API Char16Ptr U_FINAL {
public:
    /**
     * Copies the pointer.
     * @param p pointer
     * @stable ICU 59
     */
    inline Char16Ptr(UChar *p);
#if !U_CHAR16_IS_TYPEDEF
    /**
     * Converts the pointer to UChar *.
     * @param p pointer to be converted
     * @stable ICU 59
     */
    inline Char16Ptr(uint16_t *p);
#endif
#if U_SIZEOF_WCHAR_T==2 || defined(U_IN_DOXYGEN)
    /**
     * Converts the pointer to UChar *.
     * (Only defined if U_SIZEOF_WCHAR_T==2.)
     * @param p pointer to be converted
     * @stable ICU 59
     */
    inline Char16Ptr(wchar_t *p);
#endif
    /**
     * nullptr constructor.
     * @param p nullptr
     * @stable ICU 59
     */
    inline Char16Ptr(std::nullptr_t p);
    /**
     * Destructor.
     * @stable ICU 59
     */
    inline ~Char16Ptr();

    /**
     * Pointer access.
     * @return the wrapped pointer
     * @stable ICU 59
     */
    inline UChar *get() const;
    /**
     * UChar pointer access via type conversion (e.g., static_cast).
     * @return the wrapped pointer
     * @stable ICU 59
     */
    inline operator UChar *() const { return get(); }

private:
    Char16Ptr() = delete;

#ifdef U_ALIASING_BARRIER
    template<typename T> static UChar *cast(T *t) {
        U_ALIASING_BARRIER(t);
        return reinterpret_cast<UChar *>(t);
    }

    UChar *p_;
#else
    union {
        UChar *cp;
        uint16_t *up;
        wchar_t *wp;
    } u_;
#endif
};

/// \cond
#ifdef U_ALIASING_BARRIER

Char16Ptr::Char16Ptr(UChar *p) : p_(p) {}
#if !U_CHAR16_IS_TYPEDEF
Char16Ptr::Char16Ptr(uint16_t *p) : p_(cast(p)) {}
#endif
#if U_SIZEOF_WCHAR_T==2
Char16Ptr::Char16Ptr(wchar_t *p) : p_(cast(p)) {}
#endif
Char16Ptr::Char16Ptr(std::nullptr_t p) : p_(p) {}
Char16Ptr::~Char16Ptr() {
    U_ALIASING_BARRIER(p_);
}

UChar *Char16Ptr::get() const { return p_; }

#else

Char16Ptr::Char16Ptr(UChar *p) { u_.cp = p; }
#if !U_CHAR16_IS_TYPEDEF
Char16Ptr::Char16Ptr(uint16_t *p) { u_.up = p; }
#endif
#if U_SIZEOF_WCHAR_T==2
Char16Ptr::Char16Ptr(wchar_t *p) { u_.wp = p; }
#endif
Char16Ptr::Char16Ptr(std::nullptr_t p) { u_.cp = p; }
Char16Ptr::~Char16Ptr() {}

UChar *Char16Ptr::get() const { return u_.cp; }

#endif
/// \endcond

/**
 * const UChar * wrapper with implicit conversion from distinct but bit-compatible pointer types.
 * @stable ICU 59
 */
class U_COMMON_API ConstChar16Ptr U_FINAL {
public:
    /**
     * Copies the pointer.
     * @param p pointer
     * @stable ICU 59
     */
    inline ConstChar16Ptr(const UChar *p);
#if !U_CHAR16_IS_TYPEDEF
    /**
     * Converts the pointer to UChar *.
     * @param p pointer to be converted
     * @stable ICU 59
     */
    inline ConstChar16Ptr(const uint16_t *p);
#endif
#if U_SIZEOF_WCHAR_T==2 || defined(U_IN_DOXYGEN)
    /**
     * Converts the pointer to UChar *.
     * (Only defined if U_SIZEOF_WCHAR_T==2.)
     * @param p pointer to be converted
     * @stable ICU 59
     */
    inline ConstChar16Ptr(const wchar_t *p);
#endif
    /**
     * nullptr constructor.
     * @param p nullptr
     * @stable ICU 59
     */
    inline ConstChar16Ptr(const std::nullptr_t p);

    /**
     * Destructor.
     * @stable ICU 59
     */
    inline ~ConstChar16Ptr();

    /**
     * Pointer access.
     * @return the wrapped pointer
     * @stable ICU 59
     */
    inline const UChar *get() const;
    /**
     * UChar pointer access via type conversion (e.g., static_cast).
     * @return the wrapped pointer
     * @stable ICU 59
     */
    inline operator const UChar *() const { return get(); }

private:
    ConstChar16Ptr() = delete;

#ifdef U_ALIASING_BARRIER
    template<typename T> static const UChar *cast(const T *t) {
        U_ALIASING_BARRIER(t);
        return reinterpret_cast<const UChar *>(t);
    }

    const UChar *p_;
#else
    union {
        const UChar *cp;
        const uint16_t *up;
        const wchar_t *wp;
    } u_;
#endif
};

/// \cond
#ifdef U_ALIASING_BARRIER

ConstChar16Ptr::ConstChar16Ptr(const UChar *p) : p_(p) {}
#if !U_CHAR16_IS_TYPEDEF
ConstChar16Ptr::ConstChar16Ptr(const uint16_t *p) : p_(cast(p)) {}
#endif
#if U_SIZEOF_WCHAR_T==2
ConstChar16Ptr::ConstChar16Ptr(const wchar_t *p) : p_(cast(p)) {}
#endif
ConstChar16Ptr::ConstChar16Ptr(const std::nullptr_t p) : p_(p) {}
ConstChar16Ptr::~ConstChar16Ptr() {
    U_ALIASING_BARRIER(p_);
}

const UChar *ConstChar16Ptr::get() const { return p_; }

#else

ConstChar16Ptr::ConstChar16Ptr(const UChar *p) { u_.cp = p; }
#if !U_CHAR16_IS_TYPEDEF
ConstChar16Ptr::ConstChar16Ptr(const uint16_t *p) { u_.up = p; }
#endif
#if U_SIZEOF_WCHAR_T==2
ConstChar16Ptr::ConstChar16Ptr(const wchar_t *p) { u_.wp = p; }
#endif
ConstChar16Ptr::ConstChar16Ptr(const std::nullptr_t p) { u_.cp = p; }
ConstChar16Ptr::~ConstChar16Ptr() {}

const UChar *ConstChar16Ptr::get() const { return u_.cp; }

#endif
/// \endcond

/**
 * Converts from const UChar * to const UChar *.
 * Includes an aliasing barrier if available.
 * @param p pointer
 * @return p as const UChar *
 * @stable ICU 59
 */
inline const UChar *toUCharPtr(const UChar *p) {
#ifdef U_ALIASING_BARRIER
    U_ALIASING_BARRIER(p);
#endif
    return reinterpret_cast<const UChar *>(p);
}

/**
 * Converts from UChar * to UChar *.
 * Includes an aliasing barrier if available.
 * @param p pointer
 * @return p as UChar *
 * @stable ICU 59
 */
inline UChar *toUCharPtr(UChar *p) {
#ifdef U_ALIASING_BARRIER
    U_ALIASING_BARRIER(p);
#endif
    return reinterpret_cast<UChar *>(p);
}

/**
 * Converts from const UChar * to const OldUChar *.
 * Includes an aliasing barrier if available.
 * @param p pointer
 * @return p as const OldUChar *
 * @stable ICU 59
 */
inline const OldUChar *toOldUCharPtr(const UChar *p) {
#ifdef U_ALIASING_BARRIER
    U_ALIASING_BARRIER(p);
#endif
    return reinterpret_cast<const OldUChar *>(p);
}

/**
 * Converts from UChar * to OldUChar *.
 * Includes an aliasing barrier if available.
 * @param p pointer
 * @return p as OldUChar *
 * @stable ICU 59
 */
inline OldUChar *toOldUCharPtr(UChar *p) {
#ifdef U_ALIASING_BARRIER
    U_ALIASING_BARRIER(p);
#endif
    return reinterpret_cast<OldUChar *>(p);
}

U_NAMESPACE_END

#endif  // __CHAR16PTR_H__
