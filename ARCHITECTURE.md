
# Architecture — Modular Save System

This document is for contributors and developers who want to understand
the internal structure of the plugin and the reasoning behind key design
decisions.

---

## High Level Overview

[ Any Actor + ISaveableInterface ]
        |
        | (Subsystem calls through interface)
        v
[ USaveSubsystem ]
        |
        | (Reads and writes)
        v
[ USaveGameData ]

The Subsystem is the only class that talks to the other two.
The Interface and USaveGameData have no knowledge of each other.

---

## The Three Layers

### ISaveableInterface
A UInterface that any actor can implement to opt into the save system.
It defines the contract — what data to save, how to restore it, and what
version the actor is on. The subsystem never casts to a concrete actor type.
It only ever calls through this interface.

### USaveSubsystem
A GameInstance Subsystem that owns all save and load logic. When saving
or loading, the subsystem receives the caller actor as an argument, calls
through the interface to collect or restore its data, and reads or writes
to USaveGameData. It is the only entry point into the plugin for both
Blueprint and C++ users.

### USaveGameData
A USaveGame subclass that acts as a dumb data container. It holds a
`TMap<FGuid, FActorSaveData>` and knows nothing about the game world.
It is plugin-private — only USaveSubsystem touches it directly.

---

## Key Design Decisions

### 1. Interface Over Casting
The subsystem never casts to a concrete actor type. Any actor that
implements `ISaveableInterface` is automatically supported. This keeps
the subsystem decoupled from game-specific code and allows contributors
to make any actor saveable without modifying plugin internals.

### 2. FActorSaveData as a Struct Wrapper
Unreal Engine does not support nested TMaps as UPROPERTY. A raw
`TMap<FGuid, TMap<FString, FString>>` is illegal. `FActorSaveData` is a
struct that wraps `TMap<FString, FString>` alongside an `int32` version
number, satisfying Unreal's type system while keeping all per-actor data
in one place.

### 3. Per-Actor Versioning
Each actor carries its own version number inside `FActorSaveData`, not a
single global version on the subsystem. This means a format change on one
actor does not invalidate save data for every other actor in the level.
Each actor is solely responsible for its own migration logic via
`OnSaveVersionMismatch`.

### 4. FGuid as the Save Key
Actors are identified by their `ActorGuid`, not their name or class.
Names are not guaranteed unique. `ActorInstanceGuid` is transient and
evaluates to all zeros at runtime. `ActorGuid` is stable across sessions
and compile builds, making it the only reliable key for persistent
actor identification.

---

## What This Plugin Is Not

This plugin is a save foundation, not a complete save solution. It does
not handle async saving, complex type serialisation, or level streaming.
See the Known Limitations section in the README for the full list.
