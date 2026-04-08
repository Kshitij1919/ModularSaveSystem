# Modular Save System
### v0.1-beta &nbsp;|&nbsp; UE5 &nbsp;|&nbsp; C++ &nbsp;|&nbsp; Blueprint Compatible &nbsp;|&nbsp; MIT License

> A save system that saves you from boilerplate save rituals by providing a cleaner, extensible foundation.

---

## What It Solves

This save system will save you from the redundant boilerplate load-cast-modify-save 
process by providing clean single function calls with proper success and failure feedback.

This save system provides a common, easy to implement per-actor data format — freeing 
you from designing your own, eliminating the God Object problem, and keeping your save 
data intact when your implementation changes.

---

## Features

- **Per-actor data format** — stores each actor's save data as a consistent key-value map. Same structure every time, regardless of what the actor is saving, making lookups straightforward and predictable.

- **Per-actor versioning** — lets you change an actor's save data format without breaking existing save files. Each actor tracks its own version independently.

- **Clean Save and Load** — single function calls that return success or failure. No hidden steps, no silent failures.

- **Any actor can be made saveable** — implement the Saveable Interface with a minimum of three functions. Fully supported in both C++ and Blueprint.

- **Blueprint helper nodes** — `FString` to `bool` and `FString` to `Enum` conversions with safe default fallbacks. Handles invalid input gracefully where UE5's native nodes do not.

- **Slot registry management** — three clean operations: check if a slot exists, save to create it, and delete it. All persisted automatically across sessions without any manual registry handling.

---
