"""
配置 GM_ChemistBondGameMode 的 DefaultPawnClass = BP_PlayerAtom
配置 playground.umap 的 WorldSettings.DefaultGameMode = GM_ChemistBondGameMode
"""
import unreal

asset_lib = unreal.EditorAssetLibrary

# ── 1. GM_ChemistBondGameMode.DefaultPawnClass = BP_PlayerAtom ──────────────
bp_player_atom_class = unreal.load_class(
    None, "/Game/BP/GameFrame/BP_PlayerAtom.BP_PlayerAtom_C")
gm_blueprint = asset_lib.load_asset("/Game/BP/GameFrame/GM_ChemistBondGameMode")

if gm_blueprint and bp_player_atom_class:
    cdo = gm_blueprint.generated_class().get_default_object()
    cdo.set_editor_property("default_pawn_class", bp_player_atom_class)
    asset_lib.save_asset(
        "/Game/BP/GameFrame/GM_ChemistBondGameMode", only_if_is_dirty=False)
    unreal.log("[setup] GM_ChemistBondGameMode.DefaultPawnClass = BP_PlayerAtom  OK")
else:
    unreal.log_error(
        f"[setup] load failed: bp_class={bp_player_atom_class}  gm={gm_blueprint}")

# ── 2. playground.umap WorldSettings.DefaultGameMode = GM_ChemistBondGameMode ─
gm_class = unreal.load_class(
    None, "/Game/BP/GameFrame/GM_ChemistBondGameMode.GM_ChemistBondGameMode_C")

level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
level_subsystem.load_level("/Game/Map/playground")

world = unreal.EditorLevelLibrary.get_editor_world()
world_settings = world.get_world_settings() if world else None

if world_settings and gm_class:
    world_settings.set_editor_property("default_game_mode", gm_class)
    level_subsystem.save_current_level()
    unreal.log("[setup] playground.umap DefaultGameMode = GM_ChemistBondGameMode  OK")
else:
    unreal.log_error(
        f"[setup] world_settings={world_settings}  gm_class={gm_class}")
