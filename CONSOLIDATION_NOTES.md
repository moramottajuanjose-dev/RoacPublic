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

## Update — sistema de disparo implementado

`RHProjectileBase`, `RHMachineGun` y `RHWeaponMountComponent` ahora tienen
lógica real (antes eran esqueletos vacíos):

- **`RHProjectileBase`** (`Weapons/`): actor con `USphereComponent`
  (colisión), `UStaticMeshComponent` (visual) y `UProjectileMovementComponent`
  (movimiento en línea recta, sin gravedad). Al golpear otro actor, busca un
  `URHHealthComponent` en él y le aplica `Damage`; expone
  `OnImpactEffects(Hit)` como `BlueprintImplementableEvent` para que
  Blueprints/VFX se enganchen sin tocar C++. Se autodestruye tras
  `LifeSpanSeconds` si no impacta nada.
- **`RHMachineGun`**: subclase de `RHProjectileBase` con valores por defecto
  ajustados (rápida, poco daño, vida corta) — es la bala que dispara el mount
  por defecto.
- **`RHWeaponMountComponent`**: se cambió de `UActorComponent` a
  `USceneComponent` — necesitaba una transform propia en el mundo para poder
  spawnear proyectiles desde el punto de montaje real (antes no tenía
  posición). Ahora `FirePrimary()`/`FireSecondary()` spawnean
  `ProjectileClass`/`SecondaryProjectileClass` en `GetComponentTransform()`,
  respetando `FireRate`/`SecondaryFireRate` como cooldown (usa
  `GetWorld()->GetTimeSeconds()`, no `FTimerHandle`, para evitar timers
  colgados si el componente se destruye a mitad de cooldown).
- **`ARHVehicleBase`** ahora attachea `FrontWeaponMount`/`RearWeaponMount` al
  root en posiciones aproximadas de parachoques (250cm adelante/atrás) y les
  asigna `ARHMachineGun` como `ProjectileClass` por defecto, para que el
  vehículo dispare algo jugable sin configuración adicional en Blueprint.

**`RHWeaponComponent`** (el otro esqueleto en `Weapons/`) se deja sin usar —
quedó como resabio de una idea temprana duplicada con
`RHWeaponMountComponent`. Ninguna otra clase lo referencia; se puede borrar
con seguridad más adelante, o reaprovechar como sistema de munición/energía
si se necesita.

## Lo que falta (pendiente real, no placeholder)

1. **Enhanced Input assets**: `VehicleMappingContext` y los `UInputAction*`
   (`AccelerateAction`, `BrakeAction`, etc.) son punteros a assets que deben
   crearse en el editor (`.uasset`, no se pueden generar como texto) y
   asignarse en el Blueprint hijo de `ARHVehicleBase` o en sus defaults.
2. **Mesh/VFX de los proyectiles**: `RHProjectileBase::ProjectileMesh` no
   tiene ninguna `UStaticMesh` asignada (no hay ningún `.uasset` en el repo);
   sin eso, la bala existe y hace daño pero es invisible. Asignar un mesh
   simple (o trail de Niagara) en Blueprint.
3. **Malla del vehículo, ruedas Chaos, físicas**: `ARHVehicleBase` hereda de
   `AChaosWheeledVehiclePawn` pero no configura `UChaosVehicleMovementComponent`
   (ruedas, motor, suspensión) — eso se hace normalmente en un Blueprint hijo
   o en un Data Asset, no en el `.cpp` base.
4. Todas las clases `Core/`, `UI/`, `Gameplay/` siguen siendo esqueletos
   vacíos — funcionalmente no hacen nada todavía.
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
