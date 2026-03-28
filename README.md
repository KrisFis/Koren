# Koren

**A header-only foundational C++ library** — my personal alternative to the STL, inspired by Unreal Engine's architecture but written in pure C++ from scratch.

Started as a learning exercise in implementing STL-like containers and types. It has since grown into a generic foundation library I use across my own projects under the **Alternity Arts** umbrella.

> **Note:** Still a work in progress. Some types currently typedef from `std::` as placeholders — these are intended to be replaced with custom implementations over time.

---

## Namespace

All library types live under the `Kor::` namespace. The name comes from *Koren* — Czech for *root*.

```cpp
#include "Kor/Kor.h"

Kor::TArray<int> myArray;
Kor::FString myStr = KOR_TEXT("Hello World!");
```

---

## Platform Support

| **Platform** | **Tested** | **Macro**              |
|:-------------|:----------:|:-----------------------|
| Windows      |    YES     | `KOR_PLATFORM_WINDOWS` |
| Linux        |    YES     | `KOR_PLATFORM_LINUX`   |
| Apple        |    YES     | `KOR_PLATFORM_APPLE`   |

| **Architecture** | **Tested** | **Macro**             |
|:-----------------|:----------:|:----------------------|
| 64-bit           |    YES     | `KOR_ARCHITECTURE_64` |
| 32-bit           |     NO     | `KOR_ARCHITECTURE_32` |

| **Compiler**  | **Tested** | **Macro**                              |
|:--------------|:----------:|:---------------------------------------|
| MSVC          |    YES     | `KOR_COMPILER_MSVC`                    |
| Clang         |    YES     | `KOR_COMPILER_GNUC && KOR_COMPILER_CLANG` |
| GCC           |    YES     | `KOR_COMPILER_GNUC && KOR_COMPILER_GCC`   |

---

## Features

| **Description**                | **Example**                                    |
|:-------------------------------|:-----------------------------------------------|
| STL-like containers            | `TArray`, `TSharedPtr`                         |
| Lightweight math library       | `SMath::Abs`, `SMath::Lerp`                    |
| Human-readable build macros    | `KOR_BUILD_RELEASE`, `PLATFORM_WINDOWS`        |
| Extra utility types            | `TSharedClass`, `TAsync`                       |
| Object-oriented string helpers | `SString::Split`                               |
| Character encoding             | `KOR_TEXT("myText")`, `KTEXT("myText")`          |
| Debug assertions               | `KOR_DEMAND`, `KOR_EXPECT`, `KOR_REQUIRE`        |

---

## Text Literals

Wide character string literals are wrapped via `KOR_TEXT`. A shorter alias `KTEXT` is provided for convenience — both are identical.

```cpp
Kor::tchar* a = KOR_TEXT("Hello");   // canonical
Kor::tchar* b = KTEXT("Hello");      // shorthand alias — same thing
```

`KTEXT` is just:
```cpp
#define KTEXT(x) KOR_TEXT(x)
```
 
---

## Assertions

| **Macro**      | **Fatal** | **Returns bool** | **Use when...**                               |
|:---------------|:---------:|:----------------:|:----------------------------------------------|
| `KOR_ASSERT`   |    YES    |        NO        | This must be true — statement, crash if not   |
| `KOR_EXPECT`   |    NO     |       YES        | This should be true — warn and recover if not |

```cpp
KOR_ASSERT(ptr != nullptr);         // fatal, hard requirement

if (KOR_EXPECT(index < size))       // non-fatal — recoverable expectation
{
    arr[index] = value;
}
```

---

## STL Equivalents

| **Description**   | **Koren**              | **STL equivalent**       |
|:------------------|:----------------------:|:------------------------:|
| Smart pointers    | `TSharedPtr, TWeakPtr` | `shared_ptr, weak_ptr`   |
| Dynamic array     | `TArray`               | `vector`                 |
| Dynamic string    | `TString<CharType>`    | `string`                 |
| FIFO container    | `TQueue`               | `queue`                  |
| Optional value    | `TOptional`            | `optional`               |

---

## Includes

### `Kor/KorMinimal.h`
Minimal include — macros, templates, forward declarations, and core platform code. Use this when you only need the foundations.

### `Kor/Kor.h`
Full include — all features, declarations, and implementations.

---

## Platform Code

Each platform has its own folder, accessible via platform macros defined in `KorMinimal.h`. This keeps platform-specific code isolated without scattering `#ifdef` everywhere.

**Example —** on Linux, `KOR_PLATFORM_HEADER_FROM(Kor, Memory)` resolves to `Kor/Linux/LinuxMemory.h` automatically.

```cpp
#include "Kor/KorMinimal.h" // or "Kor/Build.h"
 
// Relative to current include root
#include KOR_PLATFORM_HEADER(MyHeader)
 
// Relative to explicit root (use when including from a different module)
#include KOR_PLATFORM_HEADER_FROM(Kor, MyHeader)
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
Personal reasons mostly — I wanted to understand how these things work at the implementation level, and I prefer UE-style naming and architecture. This library reflects my own taste and needs.

### Can I use it in my own project?
Yes, feel free. Keep in mind it's opinionated and shaped around my workflows.
