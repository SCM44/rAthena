// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}

<<<<<<< HEAD

void atcommand_battlestats_sub(int fd, struct map_session_data* sd) {
	status_data* status = status_get_status_data(&sd->bl);
	char output[CHAT_SIZE_MAX];

#define battlestats_msg(__msg__) (__msg__); clif_displaymessage(fd, output)

	battlestats_msg(sprintf(output, "Battle Status: %s", sd->status.name));

	clif_displaymessage(fd, "|--- Basic Info ---|");
	battlestats_msg(sprintf(output, "   -> ATK: %d + %d", pc_leftside_atk(sd), pc_rightside_atk(sd)));
	battlestats_msg(sprintf(output, "   -> MATK: %d + %d", pc_leftside_matk(sd), pc_rightside_matk(sd)));
	battlestats_msg(sprintf(output, "   -> HIT: %d", status->hit));
	battlestats_msg(sprintf(output, "   -> CRIT: %d", status->cri / 10));
	battlestats_msg(sprintf(output, "   -> DEF: %d + %d", pc_leftside_def(sd), pc_rightside_def(sd)));
	battlestats_msg(sprintf(output, "   -> MDEF: %d + %d", pc_leftside_mdef(sd), pc_rightside_mdef(sd)));
	battlestats_msg(sprintf(output, "   -> FLEE: %d + %d", status->flee, status->flee2 / 10));
	battlestats_msg(sprintf(output, "   -> ASPD: %d", (2000 - status->amotion) / 10));

	clif_displaymessage(fd, "|--- Bonus Physical Damages ---|");
	if (sd->right_weapon.addsize[SZ_MEDIUM] + sd->left_weapon.addsize[SZ_MEDIUM]) {
		battlestats_msg(sprintf(output, "   -> Medium Size: %d%%", sd->right_weapon.addsize[SZ_MEDIUM] + sd->left_weapon.addsize[SZ_MEDIUM]));
	}
	if (sd->right_weapon.addsize[SZ_ALL] + sd->left_weapon.addsize[SZ_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Sizes: %d%%", sd->right_weapon.addsize[SZ_ALL] + sd->left_weapon.addsize[SZ_ALL]));
	}
	if (sd->right_weapon.addrace[RC_PLAYER_HUMAN] + sd->left_weapon.addrace[RC_PLAYER_HUMAN]) {
		battlestats_msg(sprintf(output, "   -> Player Human: %d%%", sd->right_weapon.addrace[RC_PLAYER_HUMAN] + sd->left_weapon.addrace[RC_PLAYER_HUMAN]));
	}
	if (sd->right_weapon.addrace[RC_PLAYER_DORAM] + sd->left_weapon.addrace[RC_PLAYER_DORAM]) {
		battlestats_msg(sprintf(output, "   -> Player Doram: %d%%", sd->right_weapon.addrace[RC_PLAYER_DORAM] + sd->left_weapon.addrace[RC_PLAYER_DORAM]));
	}
	if (sd->right_weapon.addrace[RC_ALL] + sd->left_weapon.addrace[RC_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Races: %d%%", sd->right_weapon.addrace[RC_ALL] + sd->left_weapon.addrace[RC_ALL]));
	}
	if (sd->right_weapon.addclass[CLASS_NORMAL] + sd->left_weapon.addclass[CLASS_NORMAL]) {
		battlestats_msg(sprintf(output, "   -> Normal Class: %d%%", sd->right_weapon.addclass[CLASS_NORMAL] + sd->left_weapon.addclass[CLASS_NORMAL]));
	}
	if (sd->right_weapon.addclass[CLASS_ALL] + sd->left_weapon.addclass[CLASS_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Classes: %d%%", sd->right_weapon.addclass[CLASS_ALL] + sd->left_weapon.addclass[CLASS_ALL]));
	}

	clif_displaymessage(fd, "|--- Bonus Magic Damages ---|");
	if (sd->indexed_bonus.magic_addsize[SZ_MEDIUM]) {
		battlestats_msg(sprintf(output, "   -> Medium Size: %d%%", sd->indexed_bonus.magic_addsize[SZ_MEDIUM]));
	}
	if (sd->indexed_bonus.magic_addsize[SZ_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Sizes: %d%%", sd->indexed_bonus.magic_addsize[SZ_ALL]));
	}
	if (sd->indexed_bonus.magic_addrace[RC_PLAYER_HUMAN]) {
		battlestats_msg(sprintf(output, "   -> Player Human: %d%%", sd->indexed_bonus.magic_addrace[RC_PLAYER_HUMAN]));
	}
	if (sd->indexed_bonus.magic_addrace[RC_PLAYER_DORAM]) {
		battlestats_msg(sprintf(output, "   -> Player Doram: %d%%", sd->indexed_bonus.magic_addrace[RC_PLAYER_DORAM]));
	}
	if (sd->indexed_bonus.magic_addrace[RC_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Races: %d%%", sd->indexed_bonus.magic_addrace[RC_ALL]));
	}
	if (sd->indexed_bonus.magic_addclass[CLASS_NORMAL]) {
		battlestats_msg(sprintf(output, "   -> Normal Class: %d%%", sd->indexed_bonus.magic_addclass[CLASS_NORMAL]));
	}
	if (sd->indexed_bonus.magic_addclass[CLASS_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Classes: %d%%", sd->indexed_bonus.magic_addclass[CLASS_ALL]));
	}

	clif_displaymessage(fd, "|--- Elemental Resistance ---|");
	struct {
		const char* format;
		int value;
	} ele_resist_info[] = {
		{ "   -> Neutral: %d%%", sd->indexed_bonus.subele_script[ELE_NEUTRAL] },
		{ "   -> Water: %d%%", sd->indexed_bonus.subele_script[ELE_WATER] },
		{ "   -> Earth: %d%%", sd->indexed_bonus.subele_script[ELE_EARTH] },
		{ "   -> Fire: %d%%", sd->indexed_bonus.subele_script[ELE_FIRE] },
		{ "   -> Wind: %d%%", sd->indexed_bonus.subele_script[ELE_WIND] },
		{ "   -> Poison: %d%%", sd->indexed_bonus.subele_script[ELE_POISON] },
		{ "   -> Holy: %d%%", sd->indexed_bonus.subele_script[ELE_HOLY] },
		{ "   -> Dark: %d%%", sd->indexed_bonus.subele_script[ELE_DARK] },
		{ "   -> Ghost: %d%%", sd->indexed_bonus.subele_script[ELE_GHOST] },
		{ "   -> Undead: %d%%", sd->indexed_bonus.subele_script[ELE_UNDEAD] },
		{ "   -> ALL: %d%%", sd->indexed_bonus.subele_script[ELE_ALL] },
	};
	for (int i = 0; i < ARRAYLENGTH(ele_resist_info); i++) {
		if (ele_resist_info[i].value == 0)
			continue;
		battlestats_msg(sprintf(output, ele_resist_info[i].format, ele_resist_info[i].value));
	}

	clif_displaymessage(fd, "|--- Race Resistance ---|");
	struct {
		const char* format;
		int value;
	} race_resist_info[] = {
		{ "   -> Formless: %d%%", sd->indexed_bonus.subrace[RC_FORMLESS] },
		{ "   -> Undead: %d%%", sd->indexed_bonus.subrace[RC_UNDEAD] },
		{ "   -> Beast: %d%%", sd->indexed_bonus.subrace[RC_BRUTE] },
		{ "   -> Plant: %d%%", sd->indexed_bonus.subrace[RC_PLANT] },
		{ "   -> Insect: %d%%", sd->indexed_bonus.subrace[RC_INSECT] },
		{ "   -> Fish: %d%%", sd->indexed_bonus.subrace[RC_FISH] },
		{ "   -> Demon: %d%%", sd->indexed_bonus.subrace[RC_DEMON] },
		{ "   -> DemiHuman: %d%%", sd->indexed_bonus.subrace[RC_DEMIHUMAN] },
		{ "   -> Angel: %d%%", sd->indexed_bonus.subrace[RC_ANGEL] },
		{ "   -> Dragon: %d%%", sd->indexed_bonus.subrace[RC_DRAGON] },
		{ "   -> Player Human: %d%%", sd->indexed_bonus.subrace[RC_PLAYER_HUMAN] },
		{ "   -> Player Doram: %d%%", sd->indexed_bonus.subrace[RC_PLAYER_DORAM] },
		{ "   -> ALL: %d%%", sd->indexed_bonus.subrace[RC_ALL] },
		{ "   -> Normal Class: %d%%", sd->indexed_bonus.subclass[CLASS_NORMAL] },
	};
	for (int i = 0; i < ARRAYLENGTH(race_resist_info); i++) {
		if (race_resist_info[i].value == 0)
			continue;
		battlestats_msg(sprintf(output, race_resist_info[i].format, race_resist_info[i].value));
	}

#undef battlestats_msg
}

ACMD_FUNC(battlestats) {
	nullpo_retr(-1, sd);
	atcommand_battlestats_sub(fd, sd);
	return 0;
}

ACMD_FUNC(enemystats) {
	struct map_session_data* pl_sd;

	nullpo_retr(-1, sd);

	memset(atcmd_player_name, '\0', sizeof atcmd_player_name);

	if (!message || !*message || sscanf(message, "%23[^\n]", atcmd_player_name) < 1) {
		clif_displaymessage(fd, "Please enter a player name (usage: @enemystats <char name>)");
		return -1;
	}

	pl_sd = map_nick2sd(atcmd_player_name, true);
	if (pl_sd == NULL ||
		strncmp(pl_sd->status.name, atcmd_player_name, NAME_LENGTH) != 0 ||
		(pc_has_permission(pl_sd, PC_PERM_HIDE_SESSION) && pc_get_group_level(pl_sd) > pc_get_group_level(sd))
		) {
		clif_displaymessage(fd, msg_txt(sd, 3)); // Character not found.
		return -1;
	}

	atcommand_battlestats_sub(fd, pl_sd);
	return 0;
}


=======
>>>>>>> bf22a4018d122d26125cb20e8dd71a833beeb988
ACMD_FUNC(battleinfo)
{
	if( sd->state.battleinfo )
	{
		clif_displaymessage(fd, "- Battle Information Display OFF - Kill/Death -");
		sd->state.battleinfo = 0;
	}
	else
	{
		clif_displaymessage(fd, "- Battle Information Display ON - Kill/Death -");
		sd->state.battleinfo = 1;
	}
	return 0;
}


// Battleground Extended by Vykimo
ACMD_FUNC(bgstart) {
	
	nullpo_retr(-1, sd);
	
	if (bg_flag) {
		clif_displaymessage(fd, "Battleground is currently in progress.");
		return -1;
	}

	bg_flag = true;
	bg_start();
	clif_displaymessage(fd, "Battleground has been initiated");

	return 0;
}

ACMD_FUNC(bgend) {
	
	nullpo_retr(-1, sd);
	
	if (!bg_flag) {
		clif_displaymessage(fd, "Battleground is currently not in progress.");
		return -1;
	}

	bg_flag = false;
	bg_end();
	clif_displaymessage(fd, "Battleground has been ended.");

	return 0;
}

ACMD_FUNC(listenbg)
{
	sd->state.bg_listen = (sd->state.bg_listen == 0);
	
	if( sd->state.bg_listen )
		clif_displaymessage(fd, "You will receive Battleground announcements.");
	else
		clif_displaymessage(fd, "You will not receive Battleground announcements.");

	return 0;
}

ACMD_FUNC(order)
{
	char tempmes[CHAT_SIZE_MAX];
	nullpo_retr(-1,sd);

	memset(tempmes, '\0', sizeof(tempmes));
	memset(atcmd_output, '\0', sizeof(atcmd_output));

	if( !message || !*message || sscanf(message, "%255[^\n]", tempmes) < 0)
	{
		clif_displaymessage(fd, "Please, enter a message (usage: @order <message>).");
		return -1;
	}
	

	if( map_getmapflag(sd->bl.m, MF_BATTLEGROUND) )
	{
		if( !sd->bmaster_flag )
		{
			clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
			return -1;
		}
		sprintf(atcmd_output, msg_txt(sd,270), "[BG Order]", tempmes);	// *%s %s*
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, sd->bmaster_flag->color, 0x190, 20, 0, 0, BG);
	}
	else
	{
		if( !sd->state.gmaster_flag )
		{
			clif_displaymessage(fd, "This command is reserved for Guild Leaders Only.");
			return -1;
		}
		sprintf(atcmd_output, msg_txt(sd,270), "[WOE Order]", tempmes);	// *%s %s*
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, 0xFF0000, 0x190, 20, 0, 0, GUILD);
	}

	return 0;
}
ACMD_FUNC(leader)
{
	struct map_session_data *pl_sd;
	nullpo_retr(-1,sd);
	if( !sd->bmaster_flag )
		clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
	else if( sd->ud.skilltimer != INVALID_TIMER )
		clif_displaymessage(fd, "Command not allow while casting a skill.");
	else if( !message || !*message )
		clif_displaymessage(fd, "Please, enter the new Leader name (usage: @leader <name>).");
	else if( (pl_sd = map_nick2sd((char *)message,false)) == NULL )
		clif_displaymessage(fd, msg_txt(sd,3)); // Character not found.
	else if( sd->bg_id != pl_sd->bg_id )
		clif_displaymessage(fd, "Destination Player is not in your Team.");
	else if( sd == pl_sd )
		clif_displaymessage(fd, "You are already the Team Leader.");
	else
	{ // Everytest OK!
		sprintf(atcmd_output, "Team Leader transfered to [%s]", pl_sd->status.name);
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, sd->bmaster_flag->color, 0x190, 20, 0, 0, BG);

		sd->bmaster_flag->leader_char_id = pl_sd->status.char_id;
		pl_sd->bmaster_flag = sd->bmaster_flag;
		sd->bmaster_flag = NULL;

		clif_name_area(&sd->bl);
		clif_name_area(&pl_sd->bl);
		return 0;
	}
	return -1;
}

ACMD_FUNC(reportafk)
{
	struct map_session_data *pl_sd;
	nullpo_retr(-1,sd);
	if( !sd->bg_id )
		clif_displaymessage(fd, "This command is reserved for Battleground Only.");
	else if( !sd->bmaster_flag && battle_config.bg_reportafk_leaderonly )
		clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
	else if( !message || !*message )
		clif_displaymessage(fd, "Please, enter the character name (usage: @reportafk/votekick <name>).");
	else if( (pl_sd = map_nick2sd((char *)message,false)) == NULL )
		clif_displaymessage(fd, msg_txt(sd,3)); // Character not found.
	else if( sd->bg_id != pl_sd->bg_id )
		clif_displaymessage(fd, "Destination Player is not in your Team.");
	else if( sd == pl_sd )
		clif_displaymessage(fd, "You cannot kick yourself.");
	else if( pl_sd->state.bg_afk == 0 )
		clif_displaymessage(fd, "The player is not AFK on this Battleground.");
	else
	{ // Everytest OK!
		std::shared_ptr<s_battleground_data> bg = util::umap_find(bg_team_db, sd->bg_id);
		if( !bg )
			return -1;

		bg_team_leave(pl_sd, false, true, 2);
		clif_displaymessage(pl_sd->fd, "You have been kicked from Battleground because of your AFK status.");
		pc_setpos(pl_sd,pl_sd->status.save_point.map,pl_sd->status.save_point.x,pl_sd->status.save_point.y,CLR_TELEPORT);
		clif_refresh(pl_sd);

		sprintf(atcmd_output, "- AFK [%s] Kicked -", pl_sd->status.name);
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, bg->color, 0x190, 20, 0, 0, BG);
		return 0;
	}
	return -1;
}
/*==========================================
 * Guild Skill Usage for Guild Masters
 *------------------------------------------*/
ACMD_FUNC(guildskill)
{
	int i, skillnum = 0, skilllv = 0;
	t_tick tick = gettick();

	const struct { char skillstr[3]; int id; } skills[] = {
		{ "BO",	10010 },
		{ "RG",	10011 },
		{ "RS",	10012 },
		{ "EC",	10013 },
	};

	// Check for Skill ID
	for( i = 0; i < ARRAYLENGTH(skills); i++ )
	{
		if( strncmpi(message, skills[i].skillstr, 3) == 0 )
		{
			skillnum = skills[i].id;
			break;
		}
	}
	if( !skillnum )
	{
		clif_displaymessage(fd, "Invalid Skill string. Use @guildskill EC/RS/RG/BO");
		return -1;
	}

	if( !map_getmapflag(sd->bl.m, MF_BATTLEGROUND) ) {
		if( sd->state.gmaster_flag )
			skilllv = guild_checkskill(sd->guild, skillnum);
		else {
			clif_displaymessage(fd, "This command is reserved for Guild Leaders Only.");
			return -1;
		}
	} else {
		std::shared_ptr<s_battleground_data> bg;
		if( (bg = sd->bmaster_flag) != NULL )
			skilllv = bg_checkskill(bg, skillnum);
		else {
			clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
			return -1;
		}
	}

	if( pc_cant_act(sd) || pc_issit(sd) || skill_isNotOk(skillnum, sd) || sd->ud.skilltimer != -1 || sd->sc.option&(OPTION_WEDDING|OPTION_XMAS|OPTION_SUMMER) || sd->state.only_walk || sd->sc.data[SC_BASILICA] )
		return -1;

	if( DIFF_TICK(tick, sd->ud.canact_tick) < 0 )
		return -1;

	if( sd->menuskill_id )
	{
		if( sd->menuskill_id == SA_TAMINGMONSTER )
			sd->menuskill_id = sd->menuskill_val = 0; //Cancel pet capture.
		else if( sd->menuskill_id != SA_AUTOSPELL )
			return -1; //Can't use skills while a menu is open.
	}

	sd->skillitem = sd->skillitemlv = 0;
	if( skilllv ) unit_skilluse_id(&sd->bl, sd->bl.id, skillnum, skilllv);
	return 0;
}

ACMD_FUNC(voteleader)
{
	struct map_session_data* pl_sd;
	nullpo_retr(-1, sd);
	if (sd->ud.skilltimer != INVALID_TIMER)
		clif_displaymessage(fd, "Command not allow while casting a skill.");
	else if (!sd->bg_id)
		clif_displaymessage(fd, "You should be in a Battelground in order to use the command.");
	else if (!message || !*message)
		clif_displaymessage(fd, "Please, enter the new Leader name (usage: @voteleader <name>).");
	else if ((pl_sd = map_nick2sd((char*)message, false)) == NULL)
		clif_displaymessage(fd, msg_txt(sd, 3)); // Character not found.
	else if (sd->bg_id != pl_sd->bg_id)
		clif_displaymessage(fd, "Destination Player is not in your Team.");
	else
	{ // Everytest OK!

		std::map<map_session_data*, map_session_data*>::iterator it;
		std::shared_ptr<s_battleground_data> bgteam = util::umap_find(bg_team_db, sd->bg_id);

		it = bgteam->voteleader.find(sd);
		if (it != bgteam->voteleader.end())
			bgteam->voteleader.erase(it);
		
		bgteam->voteleader.insert( std::pair<map_session_data*, map_session_data*> (sd, pl_sd));
		
		clif_displaymessage(fd, "Your vote for leadership was counted successfully");
		
		 if ((battle_config.bg_min_vote_leader > 0 && bgteam->voteleader.size() >= battle_config.bg_min_vote_leader) || bgteam->voteleader.size() >= bgteam->members.size()) {
			 struct map_session_data* leader_sd;
			 struct map_session_data* old_leader_sd;
			 int max_counter = 0;

			 for (auto& b : bgteam->members)
			 {
				 if (b.sd->bmaster_flag) {
					 old_leader_sd = b.sd;
					 break;
				 }
			 }
			 for (auto const& b : bgteam->voteleader)
			 {
				 int count = 0;
				 for (auto const& p : bgteam->voteleader) count += p.second == b.second;
				 if (count >= max_counter) {
					 max_counter = count;
					 leader_sd = b.second;
				 }
			 }
			 bgteam->voteleader.clear();
			 if (leader_sd && old_leader_sd) {
				 sprintf(atcmd_output, "Team %d Leader transfered from [%s] to [%s]", bgteam->id, old_leader_sd->status.name, leader_sd->status.name);
				 clif_broadcast2(&old_leader_sd->bl, atcmd_output, (int)strlen(atcmd_output) + 1, bgteam->color, 0x190, 20, 0, 0, BG);

				 if (leader_sd != old_leader_sd) {
					 old_leader_sd->bmaster_flag->leader_char_id = leader_sd->status.char_id;
					 leader_sd->bmaster_flag = old_leader_sd->bmaster_flag;
					 old_leader_sd->bmaster_flag = NULL;

					 clif_name_area(&old_leader_sd->bl);
					 clif_name_area(&leader_sd->bl);
				 }
			 }
		 }
		 else {

			 sprintf(atcmd_output, "[%d/%d] voted to change Team %d Leader, vote with @voteleader <char name>", bgteam->voteleader.size(), (battle_config.bg_min_vote_leader > 0) ? battle_config.bg_min_vote_leader : bgteam->members.size(), bgteam->id);
			 clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output) + 1, bgteam->color, 0x190, 20, 0, 0, BG);
		 }
		 return 0;
	}
	return -1;
}
