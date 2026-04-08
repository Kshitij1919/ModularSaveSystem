# Modular Save System
### v0.1-beta &nbsp;|&nbsp; UE5 &nbsp;|&nbsp; C++ &nbsp;|&nbsp; Blueprint Compatible &nbsp;|&nbsp; MIT License

> A save system that saves you from boilerplate save rituals by providing a cleaner, extensible foundation.

---

## Table of Contents

- [What It Solves](#what-it-solves)
- [Features](#features)
- [Known Limitations](#known-limitations)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Roadmap](#roadmap)

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

## Known Limitations

- **No async support** — the plugin currently lacks async save and load support. On levels with many saveable actors this may cause a noticeable hitch on the game thread.

- **No full native type support** — the plugin does not have full native UE data type support. Types such as `FVector`, `FRotator`, and `FTransform` require manual string conversion by the user. Serialisation helper nodes for all common types are planned for v1.0.

- **Level streaming incompatibility** — the plugin uses `ActorGuid` to uniquely identify saveable actors. This works correctly for standard levels but is not compatible with level streaming, where `ActorInstanceGuid` should be used instead. Developers using level streaming should be aware of this before integrating the plugin.

---

## Installation

> **Prerequisite:** This plugin requires a C++ UE5 project and Visual Studio 2022 with the **Game Development with C++** workload installed. Blueprint-only projects must be converted first — [see Epic's guide here](https://dev.epicgames.com/community/learning/courses/KJ/unreal-engine-converting-blueprint-to-c).

---

### Method 1 — Project Plugin (Recommended)

1. Download the zip from the **Code** section of this repository
2. Extract the zip
3. Navigate to your project's root directory
4. If a `Plugins` folder does not exist, create one
5. Copy the `ModularSaveSystem` folder into `[YourProject]/Plugins/`
6. Right click the `.uproject` file → **Generate Visual Studio project files**
7. Open your project → `Edit → Plugins` → search **Modular Save System** → Enable → Restart Editor

---

### Method 2 — Engine Plugin

1. Download the zip from the **Code** section of this repository
2. Extract the zip
3. Copy the `ModularSaveSystem` folder into `[UE5 Install Directory]/Engine/Plugins/`
4. Open any C++ project → `Edit → Plugins` → search **Modular Save System** → Enable → Restart Editor

---
