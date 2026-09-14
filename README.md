# Koren

**A header-only foundational C++ library** - my personal alternative to the STL, inspired by Unreal Engine's architecture but written in pure C++ from scratch.

Started as a learning exercise in implementing STL-like containers and types. It has since grown into a generic foundation library I use across my own projects.

> **Note:** Still a work in progress. Some types currently typedef from `std::` as placeholders - these are intended to be replaced with custom implementations over time.

---

## Namespace

All library types live under the `Kor::` namespace. The name comes from *Koren* - Czech for *root*.

```cpp
#include "Kor/Kor.h"

Kor::TArray<int32> myArray;
Kor::SString myStr = KOR_TEXT("Hello World!");
```

---

## Platform Support

| **Platform** | **Tested** | **Macro**              |
|:-------------|:----------:|:-----------------------|
| Windows      |    YES     | `KOR_PLATFORM_WINDOWS` |
| Linux        |    YES     | `KOR_PLATFORM_LINUX`   |
| Apple        |    YES     | `KOR_PLATFORM_APPLE`   |
| Unix         |    YES     | `KOR_PLATFORM_UNIX`    |

| **Architecture** | **Tested** | **Macro**             |
|:-----------------|:----------:|:----------------------|
| 64-bit           |    YES     | `KOR_ARCHITECTURE_64` |
| 32-bit           |     NO     | `KOR_ARCHITECTURE_32` |

| **Compiler** | **Tested** | **Macro**                                 |
|:-------------|:----------:|:------------------------------------------|
| MSVC         |    YES     | `KOR_COMPILER_MSVC`                       |
| Clang        |    YES     | `KOR_COMPILER_GNUC && KOR_COMPILER_CLANG` |
| GCC          |    YES     | `KOR_COMPILER_GNUC && KOR_COMPILER_GCC`   |

---

## Libraries

| Module         | Description                                                                                                                                                                            |
|----------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Archive**    | Polymorphic serialization streams (`SArchive` etc.) unifying binary/string I/O behind a common read/write interface, with `<<`/`>>` operators for streaming primitives and containers. |
| **Container**  | Core data structures (`TArray`, `TQueue`, etc.) with allocator-aware storage and STL-like mutation/iteration APIs.                                                                     |
| **IO**         | Low-level input/output primitives. Currently minimal; may eventually merge with Archive under a unified serialization/streaming module.                                                |
| **Math**       | Scalar and vector math operations (`SMathOps`, etc.).                                                                                                                                  |
| **Memory**     | Memory management primitives, smart pointers (`TSharedPtr`, `TSharedClass`), typed memory operations (`SMemoryOps`), and platform allocation.                                          |
| **String**     | Unicode-aware string types and operations (`TString`, `TStringView`, `SStringOps`, `SCharOps`).                                                                                        |
| **TypeTraits** | Compile-time type introspection and metaprogramming traits.                                                                                                                            |
| **Utility**    | General-purpose helpers, forwarding (`Forward`/`Move`), invocation (`Kor::Invoke`), assertions, and dispatch utilities like `IsValid`.                                                 |

---

---

## Features

| **Description**                | **Example**                                                                          |
|:-------------------------------|:-------------------------------------------------------------------------------------|
| STL-like containers            | `TArray`, `TSharedPtr`                                                               |
| Lightweight math library       | `SMathOps::Abs`, `SMathOps::Lerp`                                                    |
| Human-readable build macros    | `KOR_BUILD_RELEASE`, `KOR_PLATFORM_WINDOWS`                                          |
| Extra utility types            | `TSharedClass`, `IsValid`                                                            |
| Object-oriented string helpers | `SString::Split`                                                                     |
| Character encoding             | `KOR_TEXT("myText")`, `KTEXT("myText")`                                              |
| Debug assertions               | `KOR_ASSERT`, `KOR_EXPECT`                                                           |
| Type Traits Generators         | `KOR_DEFINE_HAS_GLOBAL_METHOD_TRAIT(THasGetNum, GetNum); THasGetNum<SMyType>::Value` |

---

## Text Literals

Wide character string literals are wrapped via `KOR_TEXT`. A shorter alias `KTEXT` is provided for convenience - both are identical.

```cpp
Kor::tchar* a = KOR_TEXT("Hello");   // canonical
Kor::tchar* b = KTEXT("Hello");      // shorthand alias - same thing
```

`KTEXT` is just:
```cpp
#define KTEXT(x) KOR_TEXT(x)
```
 
---

## Assertions

| **Macro**      | **Fatal** | **Returns bool** | **Use when...**                               |
|:---------------|:---------:|:----------------:|:----------------------------------------------|
| `KOR_ASSERT`   |    YES    |        NO        | This must be true - statement, crash if not   |
| `KOR_EXPECT`   |    NO     |       YES        | This should be true - warn and recover if not |

```cpp
KOR_ASSERT(ptr != nullptr);         // fatal, hard requirement

if (KOR_EXPECT(index < size))       // non-fatal - recoverable expectation
{
    arr[index] = value;
}
```

---

## STL Equivalents

| **Description** |                **Koren**                 |      **STL equivalent**       |
|:----------------|:----------------------------------------:|:-----------------------------:|
| Smart pointers  |          `TSharedPtr, TWeakPtr`          |    `shared_ptr, weak_ptr`     |
| Dynamic array   |                 `TArray`                 |           `vector`            |
| Dynamic string  |     `TString<wchar>, TString<char8>`     |     `wstring, u8string, `     |
| FIFO container  |                 `TQueue`                 |            `queue`            |
| Optional value  |               `TOptional`                |          `optional`           |
| String view     | `TStringView<wchar>, TStringView<char8>` | `wstring_view, u8string_view` |

---

## Includes

### `Kor/KorMinimal.h`
Minimal include - macros, templates, forward declarations, and core platform code.

### `Kor/Kor.h`
Full PCH-like include - all features, declarations, and implementations.

---

## Platform Code

Each platform has its own folder, accessible via platform macros defined in `KorMinimal.h`. This keeps platform-specific code isolated without scattering `#ifdef` everywhere.

**Example -** on Linux, `KOR_PLATFORM_HEADER_FROM(Kor/Core, Memory)` resolves to `Kor/Core/Linux/LinuxMemory.h`.

```cpp
#include "Kor/KorMinimal.h"
 
// Relative to current include root
#include KOR_PLATFORM_HEADER(MyHeader)
 
// Relative to explicit root (use when including from a different module)
#include KOR_PLATFORM_HEADER_FROM(Kor/Core, MyHeader)
```

### Header Resolution

| Macro                                  | Expands to                         |
|----------------------------------------|------------------------------------|
| `KOR_PLATFORM_HEADER(Name)`            | `<Platform>/<Platform>Name.h`      |
| `KOR_PLATFORM_HEADER_FROM(Root, Name)` | `Root/<Platform>/<Platform>Name.h` |

### Type Name Construction

| Macro                                    | Expands to             | Use when                 |
|------------------------------------------|------------------------|--------------------------|
| `KOR_PLATFORM_TYPE(Name)`                | `<Platform>Name`       | No prefix needed         |
| `KOR_PLATFORM_TYPE_CUSTOM(Prefix, Name)` | `Prefix<Platform>Name` | Custom prefix convention |
| `KOR_PLATFORM_STRUCT(Name)`              | `S<Platform>Name`      | Structs                  |
| `KOR_PLATFORM_CLASS(Name)`               | `C<Platform>Name`      | Classes                  |
| `KOR_PLATFORM_TEMPLATE(Name)`            | `T<Platform>Name`      | Templates                |
| `KOR_PLATFORM_NAMESPACE(Name)`           | `N<Platform>Name`      | Namespaces               |
| `KOR_PLATFORM_FUNC(Name)`                | `F<Platform>Name`      | Free functions           |

---

## FAQ

### Why not just use the STL?
Personal reasons mostly - I wanted to understand how these things work at the implementation level, and I prefer UE-style naming and architecture. This library reflects my own taste and needs.

### Can I use it in my own project?
Yes, feel free. Keep in mind it's opinionated and shaped around my workflows.
