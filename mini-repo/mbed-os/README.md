# Training compile-check scaffold

## The core idea

"Does this code compile" and "does this code flash and run on the robot"
are different problems. Only the second one needs the real mbed-os source
tree, a target definition for your STM32 board, and the ARM GCC toolchain --
which is exactly the part that's a pain to keep identical across WSL,
native Windows, and macOS.

For a compile check, you don't need any of that. If you compile with a
plain host compiler and only *link nothing* (an object file, not an
executable), all you actually need is:

- the trainee's `.cpp`
- every header it `#include`s, resolvable, with matching types/signatures
- a C++17 compiler: g++ (WSL), clang++ (macOS), or MSVC/MinGW (Windows) --
  all three are either preinstalled or a single trivial install, and CMake
  picks whichever is present automatically.

The only header that's actually a problem on host is `mbed.h` (and whatever
mbed peripheral classes get touched directly -- `I2C`, `BufferedSerial`,
`CAN`, `ThisThread`, pin names). Real mbed-os headers assume an ARM target
and pull in CMSIS/HAL code tied to actual silicon, so they won't compile on
a desktop compiler as-is. Everything else in your repo -- `PID`, subsystem
business logic, math utilities -- is almost certainly plain C++ that
compiles anywhere once that one boundary is faked.

## What's in `mock_mbed/`

A minimal stand-in for the mbed API surface, scoped to exactly what's used
in the code you shared (`I2C`, `BufferedSerial`, `ThisThread::sleep_for`,
`pin_mode`/`PinMode`, pin name constants, `us_ticker_read`). Signatures match
real mbed-os so that code compiling against this mock will also compile
against the real thing -- the bodies are harmless no-ops since you only
care about type-checking here, not real electrical behavior.

**This will not cover your whole repo out of the box.** It covers only what
was visible in the files you pasted. As trainees' code touches more of your
codebase (CAN, PWM, more mbed peripherals), you'll hit compiler errors
naming the exact missing symbol -- add just enough of a declaration to
`mock_mbed/mbed.h` to satisfy it, matching the real mbed-os signature. This
is genuinely how host-testing shims for embedded projects normally get
built: start minimal, let the compiler tell you what's missing, extend.

## What NOT to mock

Only mock the true hardware boundary. Your own wrapper classes
(`CANHandler`, `DJIMotor`, `MA4`, `PID`, subsystem classes like
`ChassisSubsystem`) should be copied into `real_headers/` **unchanged** --
don't fake these. If one of those headers itself pulls in `mbed.h`
directly, the mock will satisfy that transitively. If one of those headers
has no mbed dependency at all (pure math, like `PID` likely is), it just
works with zero changes. Keeping the real headers real means trainees are
actually coding against your true interfaces, not an approximation of them.

## Setup (identical on all three platforms)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./self_check        # or self_check.exe on Windows
```

- **WSL**: `sudo apt install build-essential cmake` if not already present.
- **macOS**: Xcode Command Line Tools (`xcode-select --install`) + `brew install cmake`.
- **Windows**: Visual Studio Build Tools (or MinGW-w64) + CMake installer.
  CMake auto-detects whichever is installed; no manual toolchain wiring.

None of this touches mbed-cli, mbed-tools, or an ARM toolchain -- that setup
stays exactly as it is today for your actual flash-to-robot builds.

## Making your editor stop showing squiggles

Squiggly red underlines under `#include "mbed.h"` are your editor's language
server (not the compiler) failing to find the header -- a separate problem
from whether the code actually compiles. `CMakeLists.txt` already has
`CMAKE_EXPORT_COMPILE_COMMANDS ON`, which generates `build/compile_commands.json`
listing the exact include paths used for each file. Point your editor at it:

- **VS Code + CMake Tools extension**: configures this automatically once
  you "Configure" the project through the extension -- nothing to do.
- **VS Code + Microsoft C/C++ extension (manual)**: add to
  `.vscode/c_cpp_properties.json`:
  ```json
  { "configurations": [ { "compileCommands": "${workspaceFolder}/build/compile_commands.json" } ] }
  ```
- **VS Code + clangd extension** (common alternative, often preferred for
  embedded work): clangd looks for `compile_commands.json` in the workspace
  root by default. Either symlink it there
  (`ln -s build/compile_commands.json .` on macOS/WSL, or
  `mklink compile_commands.json build\compile_commands.json` on Windows), or
  set `"clangd.arguments": ["--compile-commands-dir=build"]` in VS Code
  settings.
- **CLion**: opens `CMakeLists.txt` directly and figures this out itself --
  nothing to configure.

After pointing the editor at it, reload/restart the language server (VS
Code: "Developer: Reload Window", or restart the clangd server from the
command palette).

You'll need to re-run `cmake --build build` (or otherwise re-configure)
whenever you add a new trainee `add_library(...)` target, since that's what
regenerates `compile_commands.json` with the new file's paths.

## Grading a trainee submission

1. Drop their `.cpp` (and any header they wrote) into the scaffold.
2. Add an `add_library(their_name_check OBJECT their_file.cpp)` line to
   `CMakeLists.txt`.
3. `cmake --build build --target their_name_check`
4. Clean build → their code compiles. That's the bar for this stage.

## Debugging triage once you merge into the real repo

Because the mock only affects one build (yours), a mismatch between "passed
here" and "passed there" is diagnostic, not random:

| Compiles against mock? | Compiles against real mbed-os? | Likely cause |
|---|---|---|
| Yes | Yes | Normal case. Any remaining bugs are logic/tuning, debug as usual. |
| Yes | **No** | Your mock's signature drifted from the real one (missing overload, wrong default arg, etc). This is an infra issue on your end, not the trainee's -- fix `mock_mbed/`, don't blame the submission. |
| No | -- | Trainee's code has a real compile error. Straightforward. |
| Yes | Yes, but wrong behavior only on hardware | Runtime/timing/electrical issue neither build catches -- expected, since the mock never claimed to model real behavior. |

The "compiles here, fails for real" row is the one worth watching, since
it's the only one that could unfairly cost a trainee time. Keeping your
mock signatures copy-pasted from mbed-os's actual header declarations
(rather than reconstructed from memory) keeps that row rare.

## A portability note from your actual code

`ChassisSubsystem.cpp` calls `abs(yawError)` on a `float` without `<cmath>`
explicitly included. Depending on what else got included first, `abs()` can
resolve to the C `int abs(int)` overload (silently truncating) instead of a
floating-point-aware `abs`. GCC, Clang, and MSVC don't always agree on which
overload wins in that situation -- worth switching to `std::abs()` with
`<cmath>` included explicitly so behavior is identical across all three
toolchains you're now targeting. Small thing, but exactly the kind of gap
that only shows up when you start compiling on more than one compiler.
