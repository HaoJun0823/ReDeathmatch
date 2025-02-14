---
title: Home
description: Enables deathmatch style (CSDM) gameplay in Counter-Strike 1.6 (respawning, gun selection, spawn protection, etc).
icon: material/shape-square-plus
---

# 
<p align="center">
    <a href="https://github.com/wopox1337/ReDeathmatch">
        <img
            width="500px"
            alt="Gun logo"
            src="https://user-images.githubusercontent.com/18553678/233882657-0ee4d8ea-2492-4af7-8db5-32430689c131.png"
        >
    </a>
</p>

<p align="center">
    AMXModX plugins to provide Deathmatch gameplay in <a href="https://store.steampowered.com/app/10/CounterStrike/">Counter-Strike 1.6</a> optimized to work with <a href="https://github.com/s1lentq/ReGameDLL_CS">ReGameDLL_CS</a>.
</p>

<p align="center">
    <a href="https://github.com/wopox1337/ReDeathmatch/releases/latest">
        <img
            src="https://img.shields.io/github/downloads/wopox1337/ReDeathmatch/total?label=Download%40latest&style=flat-square&logo=github&logoColor=white"
            alt="Build status"
        >
    </a>
    <a href="https://github.com/wopox1337/ReDeathmatch/actions">
        <img
            src="https://img.shields.io/github/actions/workflow/status/wopox1337/ReDeathmatch/CI.yml?branch=master&style=flat-square&logo=github&logoColor=white"
            alt="Build status"
        >
    </a>
    <a href="https://github.com/wopox1337/ReDeathmatch/releases">
        <img
            src="https://img.shields.io/github/v/release/wopox1337/ReDeathmatch?include_prereleases&style=flat-square&logo=github&logoColor=white"
            alt="Release"
        >
    </a>
    <a href="https://www.amxmodx.org/downloads-new.php">
        <img
            src="https://img.shields.io/badge/AMXModX-%3E%3D1.9.0-blue?style=flat-square"
            alt="AMXModX dependency"
        >
        </a>
</p>

## About
The mod is a completely rewritten implementation of [CSDM ReAPI](https://github.com/wopox1337/CSDM-ReAPI), to replace legacy code.

Mod made a look back on the successful experience [CSDM 2.1.2 by BAILOPAN](https://www.bailopan.net/csdm), but using modern features of the new [ReGameDLL_CS](https://github.com/s1lentq/ReGameDLL_CS).

Many features have long been built and optimized to work directly in ReGameDLL_CS, mod now only switches the game settings and provides a comfortable way to control.

## Features
- Store game settings (CVars);
- Round modes (*NEW*);
- Config hot-reload;
- Randomized, preset spawning (you can add new spawn points, spawn presets);
- Spawn protection (configurable by time and player rendering);
- Interactive spawn editor;
- Configurable weapon menus;
- Team Deathmatch as well as FFA (Free-for-all Deathmatch);
- Large pieces are optimized in ReGameDLL_CS;
- Multi-language support;
- Extraconfigs support:
    - For an individual map (`redm/extraconfigs/de_dust2.json`);
    - For map prefix (`redm/extraconfigs/prefix_aim.json`).
- Counter-Strike: Condition Zero support out of the box;
- Support for setting a grouping for spawns;
- Ability to use the mod as a basis for the development of other modes (for example, `GunGame`);
