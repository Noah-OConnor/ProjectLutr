### Fire and Reload Systems

- **Equipping, firing, and reloading** weapons now function as intended, dynamically driven by gameplay input and weapon state.
- Reorganized file structure and Blueprint/data asset hierarchy to better support modular weapons and clean event binding.
- Separated logic cleanly between `BP_WeaponBase`, weapon data instances, and the player character/controller.

### Reload Uses Gameplay Ability

- Reloading is now handled via a **Gameplay Ability (`GA_Reload`)** instead of a raw function call.
- The ability triggers a reload animation (or delay placeholder), updates both **magazine and reserve ammo**, and respects fire/reload gating logic.
- The HUD is updated automatically after reload via **event dispatchers** and GAS attribute change listeners.

### Obsidian Vault Setup

- Created `ObsidianVault` folder within the GitHub project for internal documentation.
- Migrated GDD structure and converted current dev logs, notes, and to-do items to Markdown files.
- Verified internal linking and vault structure supports daily logs, system breakdowns, and centralized team documentation.

---

## Next Steps

- Finalize HUD widgets for health and ammo
- Add gameplay effect for weapon damage and integrate with projectile impact or hitscan result
- Create dummy enemy Blueprint that:
    - Has health via GAS attributes
    - Can receive and process damage
    - Respawns after delay
- Add basic AI behavior for dummy (idle → chase → melee or patrol)
- Begin testing multiplayer-specific systems (replication of fire, reload, damage, ASC state)

