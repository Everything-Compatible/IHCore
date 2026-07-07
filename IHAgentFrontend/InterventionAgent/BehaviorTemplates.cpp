// BehaviorTemplates.cpp
// 作用：把 §5.2.1 的 18+1 模板逐一翻译为 BehaviorChain 数据。

#include "pch.h"
#include "BehaviorTemplates.h"

namespace ihagent {

// 工具：构造单个节点
static BehaviorNode node(Action a, ActionParams p = {}) {
    BehaviorNode n; n.action = a; n.params = p; return n;
}

// 工具：构造分支点
static BranchPoint branch(int at, Trigger tr, const std::string& rule, Fallback fb) {
    BranchPoint b; b.at_node = at; b.trigger = tr; b.rule_name = rule; b.fallback = fb; return b;
}

const std::map<BehaviorTemplateId, BehaviorChain>& BehaviorTemplates::all() {
    static std::map<BehaviorTemplateId, BehaviorChain> table;
    static bool inited = false;
    if (inited) return table;
    inited = true;

    // ============ 进攻类 ============
    {
        BehaviorChain c; c.template_id = "offense_push";
        c.nodes = {
            node(Action::GatherAt, { {{"target","assembly_point"}} }),
            node(Action::MoveTo, { {{"target","front_line"}, {"formation","line"}} }),
            node(Action::EstablishFront),
            node(Action::AdvanceTo, { {{"target","next_objective"}, {"step","safe"}} }),
            node(Action::ConsolidatePosition),
        };
        c.branches = {
            branch(1, Trigger::StrongResistance, "decide_hold_or_push", Fallback::LLM),
            branch(3, Trigger::FlankExposed, "decide_protect_flank_or_continue", Fallback::LLM),
        };
        c.end.clauses = {"objective_reached", "losses_exceed(50%)", "strategic_retreat_ordered"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_trench_push";
        c.nodes = {
            node(Action::GatherAttackForceWithMCV),
            node(Action::MoveTo, { {{"target","forward_position"}, {"mcv_in_middle","true"}} }),
            node(Action::DeployMCV, { {{"unpack","true"}, {"build","barracks|war_factory"}} }),
            node(Action::EstablishDefensivePerimeter, { {{"build","sentry_guns|bunkers|flak"}} }),
            node(Action::AttackFromForwardBase, { {{"target","next_target"}} }),
            node(Action::PackMCVAndAdvance),
        };
        c.branches = {
            branch(2, Trigger::EnemyContact, "decide_defend_or_repack", Fallback::LLM),  // 占位: trigger 实际应为 enemy_attacks_during_deploy
            branch(4, Trigger::StrongResistance, "decide_fortify_or_pull_back", Fallback::LLM),
        };
        c.end.clauses = {"final_objective_taken", "mcv_destroyed", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_harass";
        c.nodes = {
            node(Action::MoveTo, { {{"target","target_area"}, {"path","safe"}} }),
            node(Action::Strike, { {{"target","economy_unit|building"}, {"mode","hit_and_run"}} }),
            node(Action::RetreatTo, { {{"target","safe_zone"}} }),
            node(Action::Wait, { {{"cooldown","10s"}} }),
            node(Action::GoToNode, { {{"node","1"}} }),
        };
        c.branches = {
            branch(1, Trigger::EnemyReinforcementArrives, "decide_fight_or_flee", Fallback::Rule),
            branch(2, Trigger::PursuitDetected, "decide_escape_route", Fallback::LLM),
        };
        c.end.clauses = {"target_area_secured", "unit_count_drops_below(30%)", "mission_timeout(120s)"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_flank";
        c.nodes = {
            node(Action::GatherAt, { {{"target","start_point"}} }),
            node(Action::MoveTo, { {{"target","flank_route"}, {"avoid_detection","true"}} }),
            node(Action::Strike, { {{"target","enemy_rear|weakest_point"}} }),
            node(Action::ConsolidatePosition),
        };
        c.branches = {
            branch(1, Trigger::DetectionRisk, "decide_speed_up_or_abort", Fallback::Rule),
            branch(2, Trigger::EnemyTurnsToFace, "decide_deep_strike_or_pull_back", Fallback::LLM),
        };
        c.end.clauses = {"rear_destroyed", "unit_losses_exceed(50%)", "surrounded"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_feint";
        c.nodes = {
            node(Action::MoveTo, { {{"target","feint_target"}, {"approach","visible"}} }),
            node(Action::Strike, { {{"mode","demonstrate"}} }),
            node(Action::Wait, { {{"purpose","monitor_enemy_response"}} }),
            node(Action::HoldOrWithdraw),
        };
        c.branches = {
            branch(1, Trigger::EnemyReinforces, "decide_intensify_feint_or_switch", Fallback::LLM),
            branch(2, Trigger::MainForceReady, "decide_commit_or_withdraw", Fallback::LLM),
        };
        c.end.clauses = {"enemy_force_drawn", "feint_exposed", "main_objective_achieved"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_siege";
        c.nodes = {
            node(Action::Encircle, { {{"target","target"}, {"assign_sectors","true"}} }),
            node(Action::CutReinforcements, { {{"routes","block_routes"}} }),
            node(Action::Attrition, { {{"strike_with","artillery|air"}} }),
            node(Action::Assault, { {{"mode","breach"}} }),
        };
        c.branches = {
            branch(0, Trigger::InsufficientForceToEncircle, "decide_partial_siege_or_wait", Fallback::LLM),
            branch(2, Trigger::EnemyBreakoutAttempt, "decide_reinforce_or_let_go", Fallback::Rule),
        };
        c.end.clauses = {"target_destroyed", "enemy_surrenders", "allied_reinforcement_arrives"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_surgical_strike";
        c.nodes = {
            node(Action::SelectInsertionMethod, { {{"method","airborne|chronosphere|high_speed_ground"}} }),
            node(Action::InfiltrateTo, { {{"target","target_vicinity"}, {"avoid_main_force","true"}} }),
            node(Action::FocusFire, { {{"primary_target","high_value_building|unit"}} }),
            node(Action::Exfiltrate),
        };
        c.branches = {
            branch(1, Trigger::InsertionSpottedEarly, "decide_abort_or_adjust_route", Fallback::LLM),
            branch(2, Trigger::PrimaryTargetDestroyedAndForceAlive, "decide_secondary_target_or_extract", Fallback::LLM),
        };
        c.end.clauses = {"primary_target_destroyed", "insertion_force_destroyed", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "offense_superweapon";
        c.nodes = {
            node(Action::PositionSupportForce, { {{"outside_blast_radius","true"}} }),
            node(Action::FireSuperWeapon, { {{"target","nuke|lightning_storm|chronosphere"}} }),
            node(Action::WaitForImpact, { {{"evaluate_damage","true"}} }),
            node(Action::MoveInAndMopUp),
            node(Action::HoldOrWithdraw),
        };
        c.branches = {
            branch(2, Trigger::TargetNotDestroyed, "decide_commit_ground_or_wait_next_superweapon", Fallback::LLM),
            branch(3, Trigger::EnemyReinforcementFloodsIn, "decide_hold_or_pull_back", Fallback::Rule),
        };
        c.end.clauses = {"target_area_secured", "superweapon_ready_for_next", "player_override"};
        table[c.template_id] = c;
    }

    // ============ 防守类 ============
    {
        BehaviorChain c; c.template_id = "defense_hold";
        c.nodes = {
            node(Action::OccupyTerrain, { {{"terrain","choke|high_ground"}} }),
            node(Action::DeployFormations, { {{"formation","defensive_line"}} }),
            node(Action::HoldFireUntilOptimalRange),
            node(Action::Resist, { {{"mode","enemy_assault"}} }),
        };
        c.branches = {
            branch(2, Trigger::EnemyBypasses, "decide_stay_or_reposition", Fallback::LLM),
            branch(3, Trigger::LineBreached, "decide_fall_back_or_hold", Fallback::Rule),
        };
        c.end.clauses = {"enemy_retreats", "position_overrun", "reinforcements_arrive_to_relieve"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "defense_base";
        c.nodes = {
            node(Action::AlertAllDefenders),
            node(Action::ConvergeOnThreat, { {{"mode","sector_assignment"}} }),
            node(Action::EliminateInvaders),
            node(Action::RepairCritical),
        };
        c.branches = {
            branch(1, Trigger::MultipleBreachPoints, "decide_split_or_focus", Fallback::LLM),
            branch(2, Trigger::InvaderTooStrong, "decide_hold_or_evacuate", Fallback::LLM),
        };
        c.end.clauses = {"invaders_cleared", "critical_buildings_lost", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "defense_retreat";
        c.nodes = {
            node(Action::Disengage, { {{"mode","cease_fire_withdraw"}} }),
            node(Action::MoveTo, { {{"target","rally_point"}, {"formation","column"}, {"retreat","true"}} }),
            node(Action::Regroup),
        };
        c.branches = {
            branch(0, Trigger::EnemyPursuesAggressively, "decide_sacrifice_rearguard_or_full_retreat", Fallback::Rule),
            branch(1, Trigger::RetreatRouteBlocked, "decide_fight_through_or_alternate", Fallback::LLM),
        };
        c.end.clauses = {"safe_at_rally", "total_loss", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "defense_escort";
        c.nodes = {
            node(Action::Rendezvous, { {{"with","escortee"}} }),
            node(Action::FormEscortFormation, { {{"around","escortee"}} }),
            node(Action::MoveTo, { {{"target","destination"}, {"scan_for_threats","true"}} }),
            node(Action::DefendEscortee),
            node(Action::DeliverAndReturn),
        };
        c.branches = {
            branch(2, Trigger::AmbushDetected, "decide_counter_ambush_or_detour", Fallback::LLM),
            branch(3, Trigger::EscorteeHeavilyDamaged, "decide_repair_in_place_or_fall_back", Fallback::Rule),
        };
        c.end.clauses = {"escortee_delivered_safely", "escortee_destroyed", "player_override"};
        table[c.template_id] = c;
    }

    // ============ 经济类 ============
    {
        BehaviorChain c; c.template_id = "eco_expand";
        c.nodes = {
            node(Action::Prepare, { {{"force","mcv_or_expand_force"}} }),
            node(Action::MoveTo, { {{"target","new_mine_location"}} }),
            node(Action::DeployDefense, { {{"with","sentry_tanks|bunkers"}} }),
            node(Action::EstablishMining, { {{"route","harvester_route"}} }),
        };
        c.branches = {
            branch(1, Trigger::EnemyPresenceDetected, "decide_contest_or_abort", Fallback::LLM),
            branch(2, Trigger::InsufficientDefenseBudget, "decide_minimal_defense_or_delay", Fallback::Rule),
        };
        c.end.clauses = {"mine_operational", "expansion_aborted", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "eco_harv_protect";
        c.nodes = {
            node(Action::AssignEscortToHarvester, { {{"harv_id","?"}} }),
            node(Action::PatrolAlong, { {{"route","mining_route"}} }),
            node(Action::EngageThreats, { {{"trigger","proximity"}} }),
            node(Action::HarvesterStatusCheck),
        };
        c.branches = {
            branch(2, Trigger::ThreatExceedsEscortCapability, "decide_fight_or_call_backup", Fallback::Rule),
            branch(3, Trigger::HarvesterStrandedDeepEnemy, "decide_rescue_or_write_off", Fallback::LLM),
        };
        c.end.clauses = {"harvester_decommissioned", "escort_reassigned", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "eco_produce";
        c.nodes = {
            node(Action::Prepare, { {{"force","mcv_or_engineer_team"}} }),
            node(Action::MoveTo, { {{"target","build_site"}} }),
            node(Action::DeployAndBuild, { {{"factory_type","war_factory|barracks|refinery"}} }),
            node(Action::SetProductionQueue, { {{"initial_units","?"}} }),
        };
        c.branches = {
            branch(1, Trigger::EnemyReinforcementArrives, "decide_defend_or_relocate", Fallback::LLM),
            branch(2, Trigger::InsufficientFunds, "decide_wait_or_build_cheaper", Fallback::Rule),
        };
        c.end.clauses = {"factory_operational", "build_aborted", "player_override"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "eco_tech_up";
        c.nodes = {
            node(Action::AllocateGuardForce, { {{"target","tech_building"}} }),
            node(Action::InitiateResearch, { {{"tech_path","t2|t3|superweapon"}} }),
            node(Action::PatrolAndDefend),
            node(Action::CompleteAndReport),
        };
        c.branches = {
            branch(2, Trigger::TechBuildingUnderAttack, "decide_pull_everyone_or_reinforce", Fallback::LLM),
            branch(2, Trigger::EnemyEconomicStrikeElsewhere, "decide_stay_or_respond", Fallback::LLM),
        };
        c.end.clauses = {"tech_unlocked", "tech_building_lost", "player_override"};
        table[c.template_id] = c;
    }

    // ============ 侦察类 ============
    {
        BehaviorChain c; c.template_id = "recon_scout";
        c.nodes = {
            node(Action::MoveTo, { {{"target","scout_area"}, {"path","safe|stealth"}} }),
            node(Action::Reconnaissance, { {{"mode","pulse_check"}, {"avoid_combat","true"}} }),
            node(Action::ReportFindings),
            node(Action::WithdrawOrLinger),
        };
        c.branches = {
            branch(1, Trigger::EnemyDetectorNearby, "decide_evade_or_accept_loss", Fallback::Rule),
            branch(1, Trigger::HighValueIntelSpotted, "decide_stay_for_detail_or_report", Fallback::LLM),
        };
        c.end.clauses = {"area_scouted", "scout_lost", "intel_delivered"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "recon_harass";
        c.nodes = {
            node(Action::MoveTo, { {{"target","target_area"}} }),
            node(Action::ScoutAndIdentify, { {{"weak_target","?"}} }),
            node(Action::QuickStrike, { {{"engage_if_favorable","true"}} }),
            node(Action::RetreatTo, { {{"target","safe_zone"}} }),
        };
        c.branches = {
            branch(1, Trigger::NoWeakTargetFound, "decide_expand_search_or_abort", Fallback::Rule),
            branch(2, Trigger::EnemyResponseTooStrong, "decide_disengage_immediately_or_fight", Fallback::Rule),
        };
        c.end.clauses = {"strike_complete", "unit_losses_exceed(40%)", "no_target_found_timeout"};
        table[c.template_id] = c;
    }

    // ============ 机动类 ============
    {
        BehaviorChain c; c.template_id = "maneuver_regroup";
        c.nodes = {
            node(Action::MoveTo, { {{"target","rally_point"}, {"path","individual"}} }),
            node(Action::WaitForStragglers, { {{"timeout","15s"}} }),
            node(Action::ReformFormation),
        };
        c.end.clauses = {"all_units_present", "timeout", "next_task_assigned"};
        table[c.template_id] = c;
    }
    {
        BehaviorChain c; c.template_id = "maneuver_reposition";
        c.nodes = {
            node(Action::Prepare, { {{"assumption","no_combat"}} }),
            node(Action::MoveTo, { {{"target","new_sector"}, {"path","optimal"}, {"avoid_enemy_contact","true"}} }),
            node(Action::DeployInNewPosition),
        };
        c.branches = {
            branch(1, Trigger::EnemyInterceptUnexpected, "decide_fight_through_or_detour", Fallback::Rule),
        };
        c.end.clauses = {"positioned_at_new_sector", "player_override"};
        table[c.template_id] = c;
    }

    // ============ 特种 ============
    {
        BehaviorChain c; c.template_id = "special_ambush";
        c.nodes = {
            node(Action::ConcealInTerrain, { {{"ambush_site","?"}} }),
            node(Action::WaitForEnemy, { {{"zone","kill_zone"}} }),
            node(Action::SpringAmbush, { {{"mode","coordinated_strike"}} }),
            node(Action::MopUpOrWithdraw),
        };
        c.branches = {
            branch(1, Trigger::WrongEnemyGroupAppears, "decide_attack_anyway_or_wait", Fallback::LLM),
            branch(2, Trigger::AmbushPartiallySuccessful, "decide_pursue_or_consolidate", Fallback::Rule),
        };
        c.end.clauses = {"enemy_destroyed", "ambush_failed", "discovered_and_engaged"};
        table[c.template_id] = c;
    }

    // ============ 通用接管 ============
    {
        BehaviorChain c; c.template_id = "llm_full_control";
        c.continuous_llm_takeover = true;  // 进入接管模式
        c.end.clauses = {"mission_complete", "player_override", "timeout(M)"};
        table[c.template_id] = c;
    }

    return table;
}

// 按ID获取模板（找不到返回 nullptr）
const BehaviorChain* BehaviorTemplates::get(const BehaviorTemplateId& id) {
    const auto& t = all();
    auto it = t.find(id);
    return it == t.end() ? nullptr : &it->second;
}

} // namespace ihagent