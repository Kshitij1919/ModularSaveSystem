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

## Quick Start

### Blueprint

**Step 1 — Add the Saveable Interface to your Actor**

Open your Blueprint, go to **Class Settings** → **Implemented Interfaces** → **Add** → search for **Saveable Interface**.

![ClassSettings](https://github.com/user-attachments/assets/b4f031db-75c8-46e1-be97-189923586dd5)

---

**Step 2 — Implement GetSaveGuid**

Return your Actor's built-in `ActorGuid`. This uniquely identifies your actor in the save file.

![GetSaveGuid](https://github.com/user-attachments/assets/4d35855d-35b3-4fea-959d-25c7b09e9878)

---

**Step 3 — Implement GetSaveData**

Build a TMap<FString, FString> using the Make Map node. Add one entry per property you want to save — each property needs a unique string key and a string value. For non-string types such as float and bool, connect them directly to the value pin — UE5 will automatically insert the correct conversion node.

![GetSaveData](https://github.com/user-attachments/assets/41622567-a353-45ef-b0dd-96ce42964e4c)

---

**Step 4 — Implement RestoreSaveData**

Use **Find** to retrieve each value by its key from the incoming map. Check the result is valid using a **Branch** node before applying it back to your properties. For bool types use the **FString to Bool Default** helper node from the plugin's Blueprint Library.

> **Note:** For float properties, use **Kismet String Library → String to Float** to convert the retrieved string value before setting it.

![RestoreSaveData](https://github.com/user-attachments/assets/660008a5-dd6c-43dc-becf-b5128577fe74)

---

### C++

Inherit from `ISaveableInterface` and implement the three functions. Replace `YOURPROJECT_API` with your project's API macro.

**.h**
```cpp
#include "Interfaces/SaveableInterface.h"

UCLASS()
class YOURPROJECT_API AMyActor : public AActor, public ISaveableInterface
{
    GENERATED_BODY()

public:
    virtual TMap<FString, FString> GetSaveData_Implementation() const override;
    virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;
    virtual FGuid GetSaveGuid_Implementation() const override;
};
```

**.cpp**
```cpp
TMap<FString, FString> AMyActor::GetSaveData_Implementation() const
{
    TMap<FString, FString> Data;
    Data.Add(TEXT("MyKey"), MyStringValue);
    return Data;
}

void AMyActor::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
    const FString* Value = InData.Find(TEXT("MyKey"));
    if (Value)
    {
        MyStringValue = *Value;
    }
}

FGuid AMyActor::GetSaveGuid_Implementation() const
{
    return ActorGuid;
}
```

---

## Roadmap

This plugin is currently in **v0.1-beta**. The following features are planned for **v1.0**:

- Async Save and Load support
- Serialisation helper nodes for common UE types (`FVector`, `FRotator`, `FTransform`, `int32`, `float`)
- Detailed error reporting — result enum replacing the current bool return
- Max slot count and slot name validation

Post v1.0:
- Save slot metadata — timestamps, playtime, and custom fields per slot
- Automated test coverage
- Demo project
- Editor Save Slot Inspector
- Save data encryption
- Save data compression

---
