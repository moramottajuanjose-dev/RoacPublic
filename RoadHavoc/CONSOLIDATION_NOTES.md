# RoadHavoc — Consolidation Notes

Este repo consolida los 11 "commits" y los dos paquetes de documentación
(`RoadHavoc_DeveloperKit.zip`, `RoadHavoc_Documentacion.zip`) generados en
sesiones anteriores, en un único proyecto de Unreal Engine coherente.

## Qué se hizo

- **Un solo árbol de carpetas** (`Source/RoadHavoc/...`) en vez de 11 zips
  sueltos y parcialmente solapados.
- **Se resolvió un conflicto de arquitectura real** entre commits:
  - Los *Commits 0006/0007* le daban a `ARHVehicleBase` sus propios campos
    `Health` / `Nitro` y métodos `ApplyDamage/Repair/Fire/UseNitro/LookBack`
    directamente en la clase del vehículo.
  - Los *Commits 0008–0011* construyeron componentes independientes
    (`URHHealthComponent`, `URHNitroComponent`, `URHCameraComponent`,
    `URHWeaponMountComponent`) pensados para ser *usados por* `ARHVehicleBase`,
    pero esa integración (anunciada como "Commit 0012") nunca se entregó en el
    chat original.
  - **Decisión:** se descartó el enfoque de campos-en-la-clase (0006/0007) a
    favor del enfoque basado en componentes (0008–0011), que es más modular,
    reutilizable en Blueprints y es el que tiene la lógica más desarrollada
    (delegates, clamping, regeneración de nitro, etc).
  - `RHVehicleBase.h/.cpp` fueron **reescritos desde cero** en este
    consolidado para ser el punto de integración real: crea y registra los 4
    componentes, un `SpringArmComponent` + `CameraComponent` propios (que
    antes no existían en ningún commit), y engancha Enhanced Input
    (`Accelerate`, `Brake`, `Steer`, `Fire`, `Nitro`, `LookBack`) a esos
    componentes y a las funciones nativas de Chaos Vehicles
    (`SetThrottleInput`, `SetBrakeInput`, `SetSteeringInput`).
- **Para el resto de clases** (Core, Weapons, UI, Gameplay) solo existía una
  versión en todo el historial, así que se copiaron tal cual — siguen siendo
  esqueletos (`UCLASS() class X : public Y { GENERATED_BODY() };`) sin lógica.
- Se eliminaron las copias antiguas y duplicadas de
  `RHHealthComponent/RHNitroComponent/RHCameraComponent/RHWeaponMountComponent/RHVehicleBase`
  del Commit 0003 (versión stub, superada por 0006–0011).

## Lo que falta (pendiente real, no placeholder)

1. **Enhanced Input assets**: `VehicleMappingContext` y los `UInputAction*`
   (`AccelerateAction`, `BrakeAction`, etc.) son punteros a assets que deben
   crearse en el editor (`.uasset`, no se pueden generar como texto) y
   asignarse en el Blueprint hijo de `ARHVehicleBase` o en sus defaults.
2. **`RHWeaponMountComponent::FirePrimary/FireSecondary`** siguen siendo
   `// TODO` — no spawnean proyectiles todavía. `RHProjectileBase` /
   `RHMachineGun` / `RHWeaponComponent` (carpeta `Weapons/`) tampoco tienen
   implementación; falta decidir cómo se conectan al mount.
3. **Malla del vehículo, ruedas Chaos, físicas**: `ARHVehicleBase` hereda de
   `AChaosWheeledVehiclePawn` pero no configura `UChaosVehicleMovementComponent`
   (ruedas, motor, suspensión) — eso se hace normalmente en un Blueprint hijo
   o en un Data Asset, no en el `.cpp` base.
4. Todas las clases `Core/`, `Weapons/`, `UI/`, `Gameplay/` siguen siendo
   esqueletos vacíos — funcionalmente no hacen nada todavía.
5. No hay ningún `.uasset` (mapas, materiales, meshes, Blueprints) — solo
   código C++ y configs, tal como estaba en los zips originales.

## Estructura final

```
RoadHavoc/
├── RoadHavoc.uproject
├── Config/
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   └── DefaultInput.ini
├── Docs/                          (6 documentos de diseño técnico)
├── CONSOLIDATION_NOTES.md         (este archivo)
└── Source/RoadHavoc/
    ├── RoadHavoc.Build.cs / .Target.cs / RoadHavocEditor.Target.cs
    ├── RoadHavoc.h / .cpp
    ├── Core/            GameInstance, GameMode, GameState, PlayerController, PlayerState, GameSubsystem
    ├── Vehicle/         RHVehicleBase (integrado), RHHealthComponent, RHNitroComponent,
    │                    RHCameraComponent, RHWeaponMountComponent
    ├── Weapons/         RHWeaponComponent, RHProjectileBase, RHMachineGun
    ├── UI/              RHHUDBase
    └── Gameplay/        RHInputConfig, RHSpawnManager, RHMatchManager
```
