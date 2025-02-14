#include "ReDeathmatch/Modes/Menus.inc"
#include "ReDeathmatch/Modes/Vote.inc"

enum {
    RoundMode_NotSet = -1, /**< Round game-mode is not set */
}

static g_currentRoundModeIdx = RoundMode_NotSet
static g_forcedModeIdx = RoundMode_NotSet

static redm_modes_switch[32]

RoundModes_Init() {
    register_dictionary("redm/modes.txt")

    bind_pcvar_string(
        create_cvar(
            "redm_modes_switch", "sequentially",
            .description = "Game mode change mode. \
                `0`, `disable` - disable modes, \
                `random` - random change, \
                `sequentially` - sequential mode change."
        ),
        redm_modes_switch,
        charsmax(redm_modes_switch)
    )

    ModesMenus_Init()
    ModeVote_Init()
}

/**
 * Handles mode voting and mode switching.
 */
RoundModes_RoundEnd() {
    new bool: voteStarted = ModeVote_RoundEnd_Post()

    if (voteStarted) {
        client_print(0, print_center, "%l %l.", "Selecting", "NextGameMode")

        return
    }

    if (strcmp(redm_modes_switch, "disable") == 0)
        return

    new nextModeIdx = GetNextMode()
    if (nextModeIdx == RoundMode_NotSet)
        return

    SetCurrentMode(nextModeIdx)

    new modeName[32]
    RoundModes_GetModeInfo(g_currentRoundModeIdx, modeName, charsmax(modeName))

    client_print_color(0, print_team_red,
        HasLangKey(modeName) ? "[Re:DM] %l ^3%l^1" : "[Re:DM] %l ^3%s^1",
        "NextMode",
        modeName
    )

    // TODO: better will be hook EndRoundMessage()
    client_print(0, print_center, "%l", "ModeChanges")
}

RoundModes_RestartRound() {
    if (!IsActive())
        return

    new votedModeIdx = ModeVote_RestartRound()
    if (votedModeIdx >= 0)
        SetCurrentMode(votedModeIdx)

    if (g_currentRoundModeIdx == RoundMode_NotSet)
        return

    if (!RoundModes_GetModeInfo(g_currentRoundModeIdx))
        return

    ApplyMode(g_currentRoundModeIdx)

    for (new p = 1; p <= MaxClients; p++) {
        EquipManager_Reset(p)
    }
}

RoundModes_OnSpawnEquip(const player) {
    if (g_currentRoundModeIdx == RoundMode_NotSet)
        return

    set_task(0.3, "RoundInfoMessage", player)
}

public RoundInfoMessage(const player) {
    new modeName[32]
    RoundModes_GetModeInfo(g_currentRoundModeIdx, modeName, charsmax(modeName))

    set_dhudmessage(
        .red = 200,
        .green = 200,
        .blue = 200,
        .y = 0.85,
        .holdtime = 4.0
    )

    SetGlobalTransTarget(player)

    show_dhudmessage(
        player,
        HasLangKey(modeName) ? "%l %l" : "%l %s",
        "CurrentMode",
        modeName
    )
}

/**
 * Retrieves the JSON array containing game modes from the current configuration.
 *
 * This function retrieves the JSON array named "modes" from the current configuration. 
 * It is often used to access and iterate over the game modes defined in the configuration.
 *
 * @param count The number of game modes in the JSON array.
 * 
 * @return A reference to the JSON array containing game modes, or `Invalid_JSON` if not found.
 */
static JSON: GetModesArr(& count = 0) {
    if (!json_object_has_value(Config_GetCurrent(), "modes"))
        return Invalid_JSON

    new JSON: arrModes = json_object_get_value(Config_GetCurrent(), "modes")

    count = json_array_get_count(arrModes)
    if (!count) {
        json_free(arrModes)

        return Invalid_JSON
    }
    
    return arrModes
}

/**
 * Retrieves information about a specific game mode.
 *
 * @param modeIdx The index of the game mode.
 * @param name The name of the game mode (output).
 * @param len The length of the output buffer.
 * 
 * @return Whether the operation was successful.
 */
bool: RoundModes_GetModeInfo(const modeIdx, name[] = "", len = 0) {
    new count
    new JSON: arrModes = GetModesArr(count)
    if (arrModes == Invalid_JSON)
        return false

    if (modeIdx < 0 || modeIdx >= count) {
        json_free(arrModes)

        return false
    }

    new JSON: objMode = json_array_get_value(arrModes, modeIdx)
    json_object_get_string(objMode, "name", name, len)

    json_free(arrModes)
    json_free(objMode)

    return true
}

static GetNextMode() {
    new count
    new JSON: arrModes = GetModesArr(count)
    if (arrModes == Invalid_JSON)
        return RoundMode_NotSet

    if (RoundModes_IsModeForced()) {
        json_free(arrModes)

        return g_forcedModeIdx
    }

    new currentIdx = g_currentRoundModeIdx

    if (strcmp(redm_modes_switch, "random") == 0) {
        while (currentIdx == g_currentRoundModeIdx) {
            currentIdx = random_num(0, count - 1)
        }
    } else if (strcmp(redm_modes_switch, "sequentially") == 0) {
        ++currentIdx
        currentIdx %= count
    }

    return currentIdx
}

static bool: ApplyMode(const modeIdx) {
    if (!IsActive())
        return false

    if (modeIdx < 0)
        return false

    Config_Reload()

    new JSON: arrModes = GetModesArr()
    if (arrModes == Invalid_JSON)
        return false

    new JSON: objMode = json_array_get_value(arrModes, modeIdx)
    if (json_object_has_value(objMode, "cvars")) {
        new JSON: objCvars = json_object_get_value(objMode, "cvars")

        CvarsHandler_ApplyCvars(objCvars)

        json_free(objCvars)
    }

    json_free(objMode)
    json_free(arrModes)

    return true
}

/**
 * Retrieves the current round mode index.
 */
RoundModes_GetCurrentMode() {
    if (g_forcedModeIdx > RoundMode_NotSet)
        return g_forcedModeIdx

    return g_currentRoundModeIdx
}

/**
 * Sets a forced round mode index.
 *
 * @param modeIdx The index of the forced round mode.
 */
RoundModes_SetForcedMode(const modeIdx = RoundMode_NotSet) {
    return (g_forcedModeIdx = modeIdx)
}

static SetCurrentMode(const modeIdx = RoundMode_NotSet) {
    return (g_currentRoundModeIdx = modeIdx)
}

/**
 * Resets the current round mode to the default state.
 *
 * @return Always returns `RoundMode_NotSet` to indicate that the current round mode has been reset.
 */
RoundModes_ResetCurrentMode() {
    return (g_currentRoundModeIdx = RoundMode_NotSet)
}

/**
 * Checks if a round mode is forced.
 *
 * @return  If a mode is
 *         forced, it returns `true`; otherwise, it returns `false`.
 */
bool: RoundModes_IsModeForced() {
    // TODO: find better
    return g_forcedModeIdx > RoundMode_NotSet
}
