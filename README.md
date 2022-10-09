# Unreal-Multiplayer

This project is a c++ mod using RusselJ's [unreal mod loader](https://github.com/RussellJerome/UnrealModLoader) with socket.io to create a pseudo online multiplayer solution for games made in the unreal engine and compatible with the unreal mod loader

There are 2 ways this might go: 
1. I already have socket.io bindings set up with unreal mod loader as custom events that can be called from engine
For each game a mod can be set up that uses this and can sync other things to whatever the modder wants to send

2. A universal coremod that can spawn a player and synce positions - extra functionality can be added via hooks
Hopefully this will be the case but if unreal mod loader continues to crash the game on GetTransform methods I may have to resort to the first one
