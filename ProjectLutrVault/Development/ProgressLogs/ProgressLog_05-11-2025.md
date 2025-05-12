## Summary of Today's Achievements

### Dummy and Floating Status Bar
- Created `BP_Dummy` derived from `GDMinionCharacter`.
- Built a new AttributeSet for dummy health and related stats.
- Created a new floating status bar widget from `GDFloatingStatusBarWidget`.
- Connected the dummy's `AbilitySystemComponent` to update its health bar UI dynamically.
- Performed light project organization and documentation updates in the Obsidian vault.

### Damage & Dummies
- Created `GE_Damage`, a customizable GameplayEffect that accepts a set-by-caller value to apply variable damage.
- Connected the dummy’s UI to health changes using `Listen For Attribute Change` node.
- Set up new Object and Trace Channels for:
  - Enemy and Player pawns
  - Hitscan traces
  - Projectile colliders
- Implemented `BP_DummySpawner`, which spawns a dummy at its location and respawns a new one after death with a delay.

### Weapon Fire Replication
- Both client and server players can now shoot and damage dummy enemies!
- Reworked ability and damage logic to ensure that all gameplay-relevant functions (e.g., applying damage, firing traces) are executed only on the authoritative server side.
- Weapon reloading functions and triggers work across server and client.
- Remaining issue: reloading does not subtract from the ammo reserve on the client.

---

## Next Steps

- [ ] Fix ammo reserve attribute not updating on client during reload.
  - Move `ApplyGameplayEffectToSelf` for ammo drain to a `HasAuthority()` branch inside `GA_ReloadWeapon`.
  - Ensure the GameplayEffect is applied by the server and is replicated to the client.
- [ ] Add animation or reload feedback to the UI to reinforce reloading state.
- [ ] Begin implementing client-predicted reload support (optional).
- [ ] Test and polish dummy respawning behavior in multiplayer.
- [ ] Add cooldown or locking tag to prevent firing while reloading.
- [ ] Improve trace direction accuracy by passing camera aim vector to the server cleanly via GameplayEventData or C++ override.
