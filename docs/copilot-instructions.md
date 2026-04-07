# COPILOT INSTRUCTIONS

## Scope

- The code should be compatible with the ESP-IDF framework and follow its guidelines and best practices.
- The code should be written in C++ and follow the coding conventions and style guidelines of the project.
- Use C++ version 23.

## Environment

- A build mechanism should be updated that compiler everything, and runs all unit tests.

## File and Folder Structure

- One class per file, one file per class.

## Coding Conventions

- Use descriptive names for classes, methods, and variables.
- Use comments to explain the purpose and functionality of classes, methods, and complex code blocks.
- The code should be well-documented, with clear explanations of the purpose and functionality of each class, method, and complex code block.
- Follow the coding style and conventions of the project, such as naming conventions, indentation, and formatting.
- Avoid using magic numbers or hard-coded values; use constants or configuration files instead.
- Use error handling and logging to manage exceptions and provide insights into the application's behavior.

## Clean Code

- The code should follow the principles of object-oriented programming, such as encapsulation, inheritance, and polymorphism, to promote code reuse and maintainability.
- Follow the SOLID principles to promote maintainable and scalable code.
- Use interfaces to define contracts for classes and promote loose coupling.
- Use dependency injection to manage dependencies between classes and promote testability.
- Refactor code regularly to improve readability, maintainability, and performance.
- The code should be modular and reusable, allowing for easy extension and modification in the future.
- The code should be designed with scalability in mind, allowing for the addition of new features and functionality without significant refactoring.
- The code should be designed to handle errors gracefully, with appropriate error handling and logging mechanisms in place to provide insights into the application's behavior and facilitate debugging.
- The code should be designed to be maintainable, with clear separation of concerns, modular design, and adherence to coding standards and best practices.

- The code should be designed to be extensible, allowing for the addition of new features and functionality without significant refactoring or disruption to existing code.

## Security

- The code should be designed to be secure, with appropriate measures in place to protect against common vulnerabilities and attacks, such as input validation, authentication, and encryption.

## Unit Testing

- The code should be designed to be testable, with clear separation of concerns, modular design, and adherence to coding standards and best practices to facilitate unit testing and integration testing.
- Write unit tests for each class to ensure correctness and maintainability. Location: tests. The same folder structure as the source code (main).
- The code should be tested thoroughly, with unit tests covering all critical functionality and edge cases.
- Unit tests should be run on a Windows PC, using a CMakefile.
- All unit tests should pass before the code is committed and pushed to the repository.

## Modern C++ Features (C++11 → C++23)

The project targets C++23 and should progressively adopt modern language features where they improve
correctness, readability, and safety. The following guidelines apply, grouped by the standard that
introduced them.

### C++11 – Foundation (already used, continue applying)
- Use `nullptr` instead of `NULL` or `0` for pointer values.
- Use `override` and `final` on virtual methods and classes to catch mismatches at compile time.
- Use `= default` / `= delete` for special member functions instead of empty bodies or raw `private` declarations.
- Use `std::unique_ptr` / `std::shared_ptr` to express ownership; avoid raw `new` / `delete`.
- Use range-based `for` loops (`for (auto& item : collection)`) instead of index-based loops where applicable.
- Use `enum class` instead of plain `enum` to prevent implicit integer conversions.
- Use `constexpr` for compile-time constants and simple functions instead of `#define`.
- Use move semantics (`std::move`) when transferring ownership of objects such as callbacks and containers.
- Use `static_assert` for compile-time invariants (e.g., buffer size limits, struct sizes).
- Use `std::array<T, N>` instead of C-style arrays when the size is known at compile time.

### C++14 – Refinements
- Use `std::make_unique<T>()` instead of `new T()` to create `std::unique_ptr` objects.
- Use generic lambdas (`[](auto& x) { … }`) to reduce boilerplate in callbacks and algorithm calls.
- Use `auto` return-type deduction for short functions where the return type is obvious from the body.

### C++17 – Already targeted; apply consistently
- Mark every function that returns `esp_err_t` (or any other error/status code) with `[[nodiscard]]`
  so that callers cannot silently ignore errors.
- Use `std::string_view` instead of `const std::string&` or `const char*` for read-only string
  parameters and return values to avoid copies and null-termination issues.
- Use `std::optional<T>` to express optional return values instead of returning sentinel values such
  as `0`, `-1`, or `nullptr`.
- Use `if constexpr` for compile-time conditional code in templates instead of tag dispatch or SFINAE.
- Use structured bindings (`auto [a, b] = …`) when decomposing pairs, tuples, or simple structs.
- Use `std::variant` and `std::visit` to model a value that can hold one of several types.
- Use inline variables (`inline constexpr`) for constants defined in headers.

### C++20 – Adopt for new code
- Use `std::span<const uint8_t>` (or `std::span<T>`) instead of a raw pointer + length pair
  (`const uint8_t* data, size_t len`).  This applies everywhere data buffers are passed, including
  ESP-NOW send/receive APIs.
- Use concepts and `requires` clauses to constrain templates clearly:
  ```cpp
  template<std::invocable<const std::span<const uint8_t>&> Fn>
  void setReceiveCallback(Fn&& fn);
  ```
- Use designated initialisers for aggregate structs to make intent explicit:
  ```cpp
  esp_now_peer_info_t peer{ .peer_addr = {…}, .channel = 1 };
  ```
- Use `std::ranges` algorithms (`std::ranges::copy`, `std::ranges::find`, etc.) instead of the
  classic `<algorithm>` forms where readability improves.
- Use `[[likely]]` / `[[unlikely]]` hints on hot-path branches (e.g., success checks on
  `esp_err_t`).
- Use three-way comparison (`<=>`, `std::strong_ordering`) when implementing comparison operators.

### C++23 – Adopt for new code; back-fill where practical
- Use `std::expected<T, esp_err_t>` as the return type for functions that either produce a value
  or fail with an `esp_err_t` code.  This is preferred over output parameters or `std::optional`
  when the error reason must be communicated.
- Use `std::print` / `std::println` in host-side (PC / unit-test) code instead of `printf`; on
  the ESP32 target continue using the ESP-IDF `ESP_LOG*` macros.
- Use `std::string_view` literal suffix `sv` for compile-time string constants:
  ```cpp
  constexpr auto kTag = "MyClass"sv;
  ```
- Use explicit-object member functions (deducing `this`) to de-duplicate `const`/non-`const`
  overloads:
  ```cpp
  auto& getData(this auto& self) { return self.data_; }
  ```

## ESP-IDF 6 and ESPNow2 Migration

The project plans to migrate to **ESP-IDF 6** and the updated **ESP-NOW 2.0** API. The following
guidelines prepare the codebase for that transition.

### ESP-NOW 2.0 Receive Callback

The receive callback signature changed in ESP-IDF 5.0 and is the only supported form in ESP-IDF 6:

```c
// Deprecated (ESP-IDF 4.x — DO NOT USE in new code):
typedef void (*esp_now_recv_cb_t)(const uint8_t *mac_addr, const uint8_t *data, int len);

// Current (ESP-IDF 5.0+ / 6.0):
typedef void (*esp_now_recv_cb_t)(
    const esp_now_recv_info_t *esp_now_info,
    const uint8_t             *data,
    int                        data_len,
    int                        rssi);
```

- `esp_now_recv_info_t` contains `src_addr`, `des_addr`, and `rx_ctrl` (which includes RSSI and
  channel information).
- Always use the new form so that RSSI and destination-address information are available.

### ESPNow2 Encryption

- ESP-NOW 2.0 adds per-group encryption.  When adding a peer with `esp_now_add_peer`, set the
  `encrypt` field of `esp_now_peer_info_t` to `true` and configure the LMK (Local Master Key).
- Register send and receive callbacks using `esp_now_register_send_cb` and
  `esp_now_register_recv_cb` after `esp_now_init`.

### Abstraction Layer

- The `IEspNow` interface must expose `registerSendCallback` and `registerReceiveCallback` methods
  so that the bridge layer can register handlers without accessing the ESP-NOW API directly.
- Data buffers passed through the ESP-NOW abstraction should use `std::span<const uint8_t>` to
  avoid separate pointer + length parameters.

## Version Control

- Git will be done by myself, not by AI. I will commit and push the code to the repository after reviewing it.
