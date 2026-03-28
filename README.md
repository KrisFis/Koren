# Koren

**A header-only foundational C++ library** — my personal alternative to the STL, inspired by Unreal Engine's architecture but written in pure C++ from scratch.

Started as a learning exercise in implementing STL-like containers and types. It has since grown into a generic foundation library I use across my own projects under the **Alternity Arts** umbrella.

> **Note:** Still a work in progress. Some types currently typedef from `std::` as placeholders — these are intended to be replaced with custom implementations over time.

---

## Namespace

All library types live under the `kor::` namespace. The name comes from *Koren* — Czech for *root*.

```cpp
#include "Kor/Kor.h"
~~~~
kor::TArray<int> myArray;
kor::FString myStr = KOR_TEXT("Hello World!");
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
| Clang         |    YES     | `KOR_COMPILER_GNUC && KOR_COMPILER_GNUC_CLANG` |
| GCC           |    YES     | `KOR_COMPILER_GNUC && KOR_COMPILER_GNUC_GCC`   |
| Intel         |     NO     | `KOR_COMPILER_GNUC && KOR_COMPILER_GNUC_INTEL` |

---

## Features

| **Description**                 | **Example**                                    |
|:--------------------------------|:-----------------------------------------------|
| STL-like containers             | `TArray`, `TSharedPtr`                         |
| Lightweight math library        | `SMath::Abs`, `SMath::Lerp`                    |
| Human-readable build macros     | `KOR_BUILD_RELEASE`, `PLATFORM_WINDOWS`        |
| Extra utility types             | `TSharedClass`, `TAsync`                       |
| Object-oriented string helpers  | `SString::Split`                               |
| Character encoding via FChar    | `KOR_TEXT("myText")`, `KTEXT("myText")`          |
| Debug assertions                | `KOR_DEMAND`, `KOR_EXPECT`, `KOR_REQUIRE`        |

---

## Text Literals

Wide character string literals are wrapped via `KOR_TEXT`. A shorter alias `KTEXT` is provided for convenience — both are identical.

```cpp
kor::tchar* a = KOR_TEXT("Hello");   // canonical
kor::tchar* b = KTEXT("Hello");      // shorthand alias — same thing
```

`KTEXT` is just:
```cpp
#define KTEXT(x) KOR_TEXT(x)
```
 
---

## Assertions

| **Macro**      | **Fatal** | **Evaluates in release** | **Use when...**                                 |
|:---------------|:---------:|:------------------------:|:------------------------------------------------|
| `KOR_DEMAND`   |    YES    |            NO            | This must be true — crash immediately if not    |
| `KOR_EXPECT`   |    NO     |            NO            | This should be true — warn and recover if not   |
| `KOR_REQUIRE`  |    YES    |           YES            | Expression must always run and must be true     |

```cpp
KOR_DEMAND(ptr != nullptr);       // fatal — hard requirement
KOR_EXPECT(index < size);         // non-fatal — recoverable expectation
KOR_REQUIRE(init() == true);      // always evaluates — fatal if false

if (KOR_EXPECT(index < size))
{
    arr[index] = value;              // only runs if condition passed
}
 
if (!KOR_REQUIRE(init() == true))
{
    return false;                    // init() always ran, but failed
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
~~~~
### `Kor/KorMinimal.h`
Minimal include — macros, templates, forward declarations, and core platform code. Use this when you only need the foundations.

### `Kor/Kor.h`
Full include — all features, declarations, and implementations.

---

## Platform Code

Each platform has its own folder, accessible via the `KOR_PLATFORM_HEADER` macro. This keeps platform-specific code isolated without scattering `#ifdef` everywhere.

**Example — `Kor/Memory.h`** resolves to `Kor/Linux/LinuxMemory.h` on Linux automatically.

```cpp
#include "Kor/KorMinimal.h" // or "Kor/Build.h"
#include KOR_PLATFORM_HEADER(MyHeader)
```

Available platform macros: `KOR_PLATFORM_HEADER`, `KOR_PLATFORM_STRUCT`, `KOR_PLATFORM_CLASS`

---

## FAQ

### Why not just use the STL?
Personal reasons mostly — I wanted to understand how these things work at the implementation level, and I prefer UE-style naming and architecture. This library reflects my own taste and needs.

### Can I use it in my own project?
Yes, feel free. Keep in mind it's opinionated and shaped around my workflows.