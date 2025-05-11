### Focused (Next Few Sessions)
- [x] Implement damage gameplay effect and ensure it works with both hitscan logic
- [x] Create a dummy enemy with health and respawn functionality
- [x] Add floating health bar to dummy that updates with damage
- [x] Finalize ammo consumption and reload logic with reserve ammo integration
- [ ] Polish HUD to display health, mag ammo, and reserve ammo with live updates
- [ ] Build a small, test-ready combat arena for showcasing core loop (movement, shooting, reloading)
- [ ] Ensure weapon equip/fire/reload systems replicate properly in multiplayer
- [ ] Write basic documentation in Obsidian on how to create a weapon and an ability (for future collaborators)

### Core Systems
- [x] Consume reserve ammo via attribute on reload
- [ ] Prevent firing during reload (using tags or local gate)
- [ ] Add fire rate enforcement (cooldown or timer logic)
- [ ] Add fire mode support (semi-auto, burst, full-auto)
- [ ] Add recoil and spread modifiers per weapon

### HUD and UI
- [ ] Add player health bar (bind to health attribute)
- [ ] Add hitmarker for successful hits
- [ ] Add low ammo indicator
- [ ] Add visual indicator for current ability slots
- [ ] Add reload progress or indicator

### Enemies and AI
- [ ] Create dummy enemy with health attribute
- [ ] Apply damage via gameplay effect
- [ ] Add floating health bar to dummy
- [ ] Implement dummy respawn after delay
- [ ] Add basic AI behavior (idle, chase, melee)

### Environment
- [ ] Create a test arena for movement and combat
- [ ] Add weapon pickup trigger or interaction terminal
- [ ] Add dummy spawn pads or patrol points

### Multiplayer
- [ ] Test projectile and hitscan replication
- [ ] Verify ASC attribute replication across clients
- [ ] Ensure weapon equip, fire, and reload replicate correctly
- [ ] Sync HUD ammo and health values across clients
- [ ] Test ability activation latency and prediction

### Gameplay Abilities
- [ ] Add gameplay effect for damage
- [ ] Add gameplay tag restrictions for firing and reloading
- [ ] Create additional test abilities (e.g. blink, heal)
- [ ] Add cooldown system for abilities
- [ ] Add optional effects on reload or fire (e.g. buffs, traits)

### Weapon System
- [ ] Finalize modular weapon part system
- [ ] Add support for attachments (sight, barrel, mag)
- [ ] Add weapon inspection/debug widget
- [ ] Begin serialization of `WeaponInstance` for saving and loading
- [ ] Add secondary weapon slot or swap function

### Documentation
- [ ] Continue writing Obsidian vault notes for each system
- [ ] Add system-specific onboarding guides
- [ ] Create dev checklist template for new weapons or abilities
- [ ] Link GitHub issues or commits to progress logs (optional)
