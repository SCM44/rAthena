// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}
BUILDIN_FUNC(bg_move_queue)
{
	const char*  from_bg_name = script_getstr(st,2);
	const char* target_bg_name = script_getstr(st,3);
	bg_move_team_queue(from_bg_name, target_bg_name);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bgcheck) {
	script_pushint(st,bg_flag);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bgstart) {
	if (bg_flag)
		return SCRIPT_CMD_SUCCESS;
	bg_flag = true;
	bg_start();

	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bgend) {
	if (!bg_flag)
		return SCRIPT_CMD_SUCCESS;// Agit already Ended.
	bg_flag = false;
	bg_end();
	
	return SCRIPT_CMD_SUCCESS;
}
/// for bg
BUILDIN_FUNC(bg_flagemblem) {
	TBL_NPC* nd;
	int g_id = script_getnum(st,2);

	if( script_hasdata(st,3) )
		nd = npc_name2id(script_getstr(st,3));
	else
		nd = map_id2nd(st->oid);

	if(g_id < 0) return true;
	if( nd == NULL ) {
		ShowError("script:flagemblem: npc %d not found\n", st->oid);
	} else if( nd->subtype != NPCTYPE_SCRIPT ) {
		ShowError("script:flagemblem: unexpected subtype %d for npc %d '%s'\n", nd->subtype, st->oid, nd->exname);
	} else {
		std::shared_ptr<s_battleground_data> bgd = util::umap_find(bg_team_db, g_id);
		if(bgd) {
			bool changed;
			g_id = (bgd->g->guild_id);
			changed = ( nd->u.scr.guild_id != g_id )?true:false;
			nd->u.scr.guild_id = g_id;
			clif_guild_emblem_area(&nd->bl);
			
			/* guild flag caching */
			if( g_id ) /* adding a id */
				guild_flag_add(nd);
			else if( changed ) /* removing a flag */
				guild_flag_remove(nd);
		}
	}
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(pcblock)
{
	int id = 0, flag, type;
	TBL_PC *sd = NULL;

	type = script_getnum(st,2);
	flag = script_getnum(st,3);
	if( script_hasdata(st,4) )
		id = script_getnum(st,4);

	if( id && (sd = map_id2sd(id)) == NULL )
		return SCRIPT_CMD_SUCCESS;
	else
		script_rid2sd(sd);

	if( sd == NULL )
		return SCRIPT_CMD_SUCCESS;

	switch( type )
	{
		case 0: sd->ud.state.blockedmove = flag > 0; break;
		case 1: 
			sd->state.only_walk = flag > 0; 
			sd->ud.state.blockedskill = flag > 0; 
			pc_stop_attack(sd); //Stop attacking
			break;
	}
 
	return SCRIPT_CMD_SUCCESS;
}
int viewpointmap_sub(struct block_list *bl, va_list ap)
{
	struct map_session_data *sd;
	int npc_id, type, x, y, id, color;
	npc_id = va_arg(ap,int);
	type = va_arg(ap,int);
	x = va_arg(ap,int);
	y = va_arg(ap,int);
	id = va_arg(ap,int);
	color = va_arg(ap,int);
	sd = (struct map_session_data *)bl;
	clif_viewpoint(sd,npc_id,type,x,y,id,color);
	return 0;
}

BUILDIN_FUNC(viewpointmap)
{
	int type,x,y,id,color,m;
	const char *map_name;

	map_name = script_getstr(st,2);
	if( (m = map_mapname2mapid(map_name)) < 0 )
		return 0; // Invalid Map

	type=script_getnum(st,3);
	x=script_getnum(st,4);
	y=script_getnum(st,5);
	id=script_getnum(st,6);
	color=script_getnum(st,7);

	map_foreachinmap(viewpointmap_sub,m,BL_PC,st->oid,type,x,y,id,color);
	return 0;
}
BUILDIN_FUNC(checkspace)
{
	int nameid = 0, amount;
	struct map_session_data *sd;

	if( !script_rid2sd(sd) )
		script_pushint(st,0);
	else if( sd->storage.amount > MAX_STORAGE )
		script_pushint(st,0); // Storage at max
	else
	{
		struct script_data *data = script_getdata(st,2);
		struct item_data *id;
		struct item it;
		int i;

		get_val(st,data);
		if( data_isstring(data) )
		{
			const char *name = conv_str(st,data);
			std::shared_ptr<item_data> id = item_db.searchname( name );
			if( id )
				nameid = id->nameid;
		}
		else
			nameid = conv_num(st,data);

		memset(&it,0,sizeof(it));
		amount = script_getnum(st,3);
		it.nameid = nameid;
		it.identify = script_getnum(st,4);
		it.refine = script_getnum(st,5);
		it.attribute = script_getnum(st,6);
		it.card[0] = (short)script_getnum(st,7);
		it.card[1] = (short)script_getnum(st,8);
		it.card[2] = (short)script_getnum(st,9);
		it.card[3] = (short)script_getnum(st,10);

		if( nameid < 500 || amount <= 0 || (id = itemdb_exists(nameid)) == NULL || !itemdb_canstore(&it, pc_get_group_level(sd)) || !itemdb_isstackable2(id) )
		{
			script_pushint(st,0);
			return 0;
		}

		if( itemdb_isstackable2(id) )
		{
			ARR_FIND(0,MAX_STORAGE,i,compare_item(&sd->storage.u.items_storage[i],&it,0));
			if( i < MAX_STORAGE )
			{ // Item on Storage
				script_pushint(st,amount + sd->storage.u.items_storage[i].amount > MAX_AMOUNT ? 0 : 1);
				return 0;
			}
		}

		ARR_FIND(0,MAX_STORAGE,i,sd->storage.u.items_storage[i].nameid == 0);
		if( i >= MAX_STORAGE )
		{
			script_pushint(st,0);
			return 0;
		}
	
		script_pushint(st,1); // Can be Stored
	}

	return 0;
}

BUILDIN_FUNC(storeitem)
{
	int nameid = 0, amount;
	struct item it;
	struct map_session_data *sd;
	struct script_data *data;
	int result = 1;

	data = script_getdata(st,2);
	get_val(st,data);
	if( data_isstring(data) )
	{ // "<item name>"
		const char *name = conv_str(st,data);
		std::shared_ptr<item_data> item_data = item_db.searchname( name );
		if( item_data == nullptr )
		{
			ShowError("buildin_storeitem: Nonexistant item %s requested.\n", name);
			result = 0; //No item created.
		}
		else
			nameid = item_data->nameid;
	}
	else if( data_isint(data) )
	{ // <item id>
		nameid = conv_num(st,data);
		if( nameid <= 0 || !itemdb_exists(nameid) )
		{
			ShowError("buildin_storeitem: Nonexistant item %d requested.\n", nameid);
			result = 0;
		}
	}
	else
	{
		ShowError("buildin_storeitem: invalid data type for argument #1 (%d).", data->type);
		result = 0;
	}

	if( !itemdb_isstackable(nameid) )
		result = 0;
	if( (amount = script_getnum(st,3)) <= 0 )
		result = 0;

	if( script_hasdata(st,4) )
		sd = map_id2sd(script_getnum(st,4)); // <Account ID>
	else
		script_rid2sd(sd); // Attached player

	if( sd == NULL ) // no target
		result = 0;

	if( result )
	{
		memset(&it,0,sizeof(it));
		it.nameid = nameid;
		it.identify = 1;
		if( storage_storageopen(sd) == 1 ) {
			result = 0;
		} else {
			result = storage_additem2(sd,&it,amount);
			storage_storageclose(sd);
			if( result )
				log_pick_pc(sd, LOG_TYPE_SCRIPT, amount, &it);
		}
	}

	script_pushint(st,result);
	return 0;
}

BUILDIN_FUNC(storeitem2)
{
	int nameid = 0, amount = 0;
	int iden = 0,ref = 0,attr = 0,c1 = 0,c2 = 0,c3 = 0,c4 = 0;
	struct map_session_data *sd;
	struct script_data *data;
	struct item it;
	int result = 1;

	data = script_getdata(st,2);
	get_val(st,data);
	if( data_isstring(data) )
	{
		const char *name = conv_str(st,data);
		std::shared_ptr<item_data> item_data = item_db.searchname( name );
		if( item_data == nullptr )
		{
			ShowError("buildin_storeitem2: Nonexistant item %s requested.\n", name);
			result = 0;
		}
		else
			nameid = item_data->nameid;
	}
	else if( data_isint(data) )
	{
		nameid = conv_num(st,data);
		if( nameid <= 0 || !itemdb_exists(nameid) )
		{
			ShowError("buildin_storeitem2: Nonexistant item %d requested.\n", nameid);
			result = 0;
		}
	}
	else
	{
		ShowError("buildin_storeitem2: invalid data type for argument #1 (%d).", data->type);
		result = 0;
	}

	if( !itemdb_isstackable(nameid) )
		result = 0;
	else if( (amount = script_getnum(st,3)) <= 0 )
		result = 0;
	else
	{
		iden = script_getnum(st,4);
		ref = script_getnum(st,5);
		attr = script_getnum(st,6);
		c1 = (short)script_getnum(st,7);
		c2 = (short)script_getnum(st,8);
		c3 = (short)script_getnum(st,9);
		c4 = (short)script_getnum(st,10);
	}

	if( script_hasdata(st,11) )
		sd = map_id2sd(script_getnum(st,11));
	else
		script_rid2sd(sd);

	if( sd == NULL )
		result = 0;

	if( result )
	{
		memset(&it,0,sizeof(it));
		it.nameid = nameid;
		it.identify = 1; // always Identify
		it.refine = ref;
		it.attribute = attr;
		it.card[0] = (short)c1;
		it.card[1] = (short)c2;
		it.card[2] = (short)c3;
		it.card[3] = (short)c4;
		if( storage_storageopen(sd) == 1 ) {
			result = 0;
		} else {
			result = storage_additem2(sd,&it,amount);
			storage_storageclose(sd);
			if( result )
				log_pick_pc(sd, LOG_TYPE_SCRIPT, amount, &it);
		}
	}
	script_pushint(st,result);
	return 0;
}

/*==========================================
 * itembound <item id>,<amount>{,<character ID>};
 * itembound "<item name>",<amount>{,<character ID>};
 *------------------------------------------*/
BUILDIN_FUNC(itembound)
{
	int nameid,amount,i,flag;
	struct item it;
	TBL_PC *sd;
	struct script_data *data;

	data=script_getdata(st,2);
	get_val(st,data);
	if( data_isstring(data) )
	{ // "<item name>"
		const char *name=conv_str(st,data);
		std::shared_ptr<item_data> item_data = item_db.searchname( name );
		if( item_data == nullptr ){
			ShowError("buildin_itembound: Nonexistant item %s requested.\n", name);
			return 1; //No item created.
		}
		nameid=item_data->nameid;
	}
	else if( data_isint(data) )
	{ // <item id>
		nameid=conv_num(st,data);
		if( nameid <= 0 || !itemdb_exists(nameid) )
		{
			ShowError("buildin_getitem: Nonexistant item %d requested.\n", nameid);
			return 1; //No item created.
		}
	}
	else
	{
		ShowError("buildin_itembound: invalid data type for argument #1 (%d).", data->type);
		return 1;
	}

	if( itemdb_isstackable(nameid) || itemdb_type(nameid) == IT_PETEGG )
	{
		ShowError("buildin_itembound: invalid item type. Bound only work for non stackeable items (Item %d).", nameid);
		return 1;
	}

	// <amount>
	if( (amount=script_getnum(st,3)) <= 0)
		return 0; //return if amount <=0, skip the useles iteration

	memset(&it,0,sizeof(it));
	it.nameid = nameid;
	it.bound = 1;
	it.identify = 1;

	if( script_hasdata(st,4) )
		sd = map_id2sd(script_getnum(st,4)); // <Account ID>
	else
		script_rid2sd(sd); // Attached player

	if( sd == NULL ) // no target
		return 0;

	for( i = 0; i < amount; i++ )
	{
		if( (flag = pc_additem(sd, &it, 1, LOG_TYPE_SCRIPT)) )
		{
			clif_additem(sd, 0, 0, flag);
			if( pc_candrop(sd,&it) )
				map_addflooritem(&it,1,sd->bl.m,sd->bl.x,sd->bl.y,0,0,0,0,0);
		}
	}

	return 0;
}

BUILDIN_FUNC(itembound2)
{
	int nameid,amount,i,flag;
	int iden,ref,attr,c1,c2,c3,c4;
	std::shared_ptr<item_data> item_data;
	struct item item_tmp;
	TBL_PC *sd;
	struct script_data *data;

	if( script_hasdata(st,11) )
		sd = map_id2sd(script_getnum(st,11)); // <Account ID>
	else
		script_rid2sd(sd); // Attached player

	if( sd == NULL ) // no target
		return 0;

	data = script_getdata(st,2);
	get_val(st,data);
	if( data_isstring(data) )
	{
		const char *name = conv_str(st,data);
		item_data = item_db.searchname( name );
		if( item_data != nullptr )
			nameid = item_data->nameid;
		else
			nameid = UNKNOWN_ITEM_ID;
	}
	else
		nameid = conv_num(st,data);

	amount = script_getnum(st,3);
	iden = script_getnum(st,4);
	ref = script_getnum(st,5);
	attr = script_getnum(st,6);
	c1 = (short)script_getnum(st,7);
	c2 = (short)script_getnum(st,8);
	c3 = (short)script_getnum(st,9);
	c4 = (short)script_getnum(st,10);
	
	if( nameid < 0 || (item_data = item_db.find(nameid)) == nullptr || itemdb_isstackable2( item_data.get() ) )
		return 0;

	memset(&item_tmp,0,sizeof(item_tmp));
	item_tmp.nameid = nameid;
	if( item_data->type == IT_WEAPON || item_data->type == IT_ARMOR )
		ref = cap_value(ref,0,MAX_REFINE);
	else if( item_data->type == IT_PETEGG )
	{
		ShowError("buildin_itembound2: invalid item type. Pet Egg cannot be set as rental items.\n");
		return 1;
	}
	else
	{ // Should not happen
		iden = 1;
		ref = attr = 0;
	}

	item_tmp.identify = iden;
	item_tmp.refine = ref;
	item_tmp.attribute = attr;
	item_tmp.card[0] = (short)c1;
	item_tmp.card[1] = (short)c2;
	item_tmp.card[2] = (short)c3;
	item_tmp.card[3] = (short)c4;
	item_tmp.bound = 1;

	for( i = 0; i < amount; i++ )
	{
		if ((flag = pc_additem(sd, &item_tmp, 1, LOG_TYPE_SCRIPT)))
		{
			clif_additem(sd, 0, 0, flag);
			if( pc_candrop(sd,&item_tmp) )
				map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y,0,0,0,0,0);
		}
	}

	return 0;
}
/*
 *==========================================
 */
BUILDIN_FUNC(countstorageitem) {
	TBL_PC* sd = NULL;
	int i, type, count = 0;
	struct item_data* item_data;
	struct s_storage* gstor;
	struct s_storage* stor;
	struct item tmp_it;
	type = script_getnum(st, 2);
	if (script_hasdata(st, 11))
		map_id2sd(script_getnum(st, 11));
	else
		script_rid2sd(sd);
	if (!sd) return SCRIPT_CMD_SUCCESS;
	if (sd != NULL && (item_data = itemdb_exists(script_getnum(st, 3))) != NULL) {
		tmp_it.nameid = item_data->nameid;
		tmp_it.identify = script_getnum(st, 4);
		tmp_it.refine = script_getnum(st, 5);
		tmp_it.attribute = script_getnum(st, 6);
		tmp_it.card[0] = script_getnum(st, 7);
		tmp_it.card[1] = script_getnum(st, 8);
		tmp_it.card[2] = script_getnum(st, 9);
		tmp_it.card[3] = script_getnum(st, 10);

		switch (type) {
		case 0:
			stor = &sd->storage;
			if (stor == NULL) {
				return SCRIPT_CMD_SUCCESS;
			}
			if (sd->state.storage_flag) {
				if (sd->state.storage_flag == 1) {
					sd->state.storage_flag = 0;
					storage_storageclose(sd);
				}
				else {
					sd->state.storage_flag = 0;
					storage_guild_storageclose(sd);
				}
			}
			sd->state.storage_flag = 1;
			for (i = 0; i < MAX_STORAGE; i++) {
				if ((&stor->u.items_storage[i] != NULL)
					&& stor->u.items_storage[i].amount > 0
					&& compare_item(&stor->u.items_storage[i], &tmp_it, 1))
					count += stor->u.items_storage[i].amount;
			}
			sd->state.storage_flag = 0;
			storage_storageclose(sd);
			script_pushint(st, count);
			break;
		case 1:
			if (sd->status.guild_id && (gstor = guild2storage2(sd->status.guild_id))) {
				for (i = 0; i < MAX_GUILD_STORAGE; i++) {
					if ((&gstor->u.items_storage[i] != NULL)
						&& gstor->u.items_storage[i].amount > 0
						&& compare_item(&gstor->u.items_storage[i], &tmp_it, 1)
						)
						count += gstor->u.items_storage[i].amount;
				}
				script_pushint(st, count);
			}
			break;
		default:
			ShowError("Invalid type specified in countstorageitem");
			script_pushint(st, -1);
			return SCRIPT_CMD_FAILURE;
		}
	}
	return SCRIPT_CMD_SUCCESS;
}
/*==========================================
 * inv2stor,item_id,item_nb,attribute,refine,identify,c0,c1,c2,c3{,charid}
 *------------------------------------------*/
BUILDIN_FUNC(inv2stor) {
	TBL_PC* sd = NULL;
	int i;
	struct item_data* item_data;
	struct item tmp_it;
	int item_id = script_getnum(st, 2);
	int item_nb = script_getnum(st, 3);
	if (script_hasdata(st, 11))
		map_id2sd(script_getnum(st, 11));
	else
		script_rid2sd(sd);

	nullpo_ret(sd);

	if ((item_data = itemdb_exists(item_id)) != NULL) {
		tmp_it.nameid = item_data->nameid;
		tmp_it.identify = script_getnum(st, 4);
		tmp_it.refine = script_getnum(st, 5);
		tmp_it.attribute = script_getnum(st, 6);
		tmp_it.card[0] = script_getnum(st, 7);
		tmp_it.card[1] = script_getnum(st, 8);
		tmp_it.card[2] = script_getnum(st, 9);
		tmp_it.card[3] = script_getnum(st, 10);

		ARR_FIND(0, MAX_INVENTORY, i, ((&sd->inventory.u.items_inventory[i] != NULL)
			&& compare_item(&sd->inventory.u.items_inventory[i], &tmp_it, 1)));
		if (i < MAX_INVENTORY) {
			storage_storageadd(sd, &sd->storage, i, item_nb);
			storage_storageclose(sd);
		}
	}
	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

/*==========================================
 * inv2stor,item_id,item_nb,attribute,refine,identify,c0,c1,c2,c3{,charid}
*------------------------------------------*/
BUILDIN_FUNC(stor2inv) {
	TBL_PC* sd = NULL;
	int i;
	struct item_data* item_data;
	struct item tmp_it;
	int item_id = script_getnum(st, 2);
	int item_nb = script_getnum(st, 3);

	if (script_hasdata(st, 11))
		map_id2sd(script_getnum(st, 11));
	else
		script_rid2sd(sd);
	nullpo_ret(sd);

	if ((item_data = itemdb_exists(item_id)) != NULL) {
		tmp_it.nameid = item_data->nameid;
		tmp_it.identify = script_getnum(st, 4);
		tmp_it.refine = script_getnum(st, 5);
		tmp_it.attribute = script_getnum(st, 6);
		tmp_it.card[0] = script_getnum(st, 7);
		tmp_it.card[1] = script_getnum(st, 8);
		tmp_it.card[2] = script_getnum(st, 9);
		tmp_it.card[3] = script_getnum(st, 10);
		ARR_FIND(0, MAX_STORAGE, i, ((&sd->storage.u.items_storage[i] != NULL)
			&& compare_item(&sd->storage.u.items_storage[i], &tmp_it, 1))
		);
		if (i < MAX_STORAGE) {
			storage_storageget(sd, &sd->storage, i, item_nb);
		}
	}
	script_pushint(st, 1);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(unitisfighting)
{
	struct block_list* bl;
	struct unit_data *ud;

	if (script_rid2bl(2, bl)) {
		ud = unit_bl2ud(bl);
		script_pushint(st, (ud->attacktimer != INVALID_TIMER));
	} else
		script_pushint(st, 0);
	
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_getitem)
{
	int bg_id, nameid, amount;

	bg_id = script_getnum(st,2);
	nameid = script_getnum(st,3);
	amount = script_getnum(st,4);

	bg_team_getitem(bg_id, nameid, amount);
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bg_reward)
{
	int team_id, nameid, amount, kafrapoints, quest_id, add_value, bg_arena;
	const char *var;
	
	team_id = script_getnum(st,2);
	nameid = script_getnum(st,3);
	amount = script_getnum(st,4);
	kafrapoints = script_getnum(st,5);
	quest_id = script_getnum(st,6);
	var = script_getstr(st,7);
	add_value = script_getnum(st,8);
	bg_arena = script_getnum(st,9);

	bg_team_rewards(team_id, nameid, amount, kafrapoints, quest_id, var, add_value, bg_arena, 0);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_reward_all)
{
	int team_id1, team_id2, nameid, amount, bg_result;
	const char *bg_map;
	
	bg_map = script_getstr(st,2);
	team_id1 = script_getnum(st,3);
	team_id2 = script_getnum(st,4);
	nameid = script_getnum(st,5);
	amount = script_getnum(st,6);
	bg_result = script_getnum(st,7);

	bg_team_rewards_all(bg_map, team_id1, team_id2, nameid, amount, bg_result);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bgannounce)
{
	const char *mes       = script_getstr(st,2);
	const char *fontColor = script_hasdata(st,3) ? script_getstr(st,3) : "0xFFFFFF";
	int         fontType  = script_hasdata(st,4) ? script_getnum(st,4) : 0x190; // default fontType (FW_NORMAL)
	int         fontSize  = script_hasdata(st,5) ? script_getnum(st,5) : 12;    // default fontSize
	int         fontAlign = script_hasdata(st,6) ? script_getnum(st,6) : 0;     // default fontAlign
	int         fontY     = script_hasdata(st,7) ? script_getnum(st,7) : 0;     // default fontY

	clif_broadcast2(NULL, mes, (int)strlen(mes)+1, strtol(fontColor, (char **)NULL, 0), fontType, fontSize, fontAlign, fontY, BG_LISTEN);
	return 0;
}
BUILDIN_FUNC(bg_clean)
{
	int bg_id = script_getnum(st,2);
	bg_team_clean(bg_id, false);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_logincount)
{
	struct map_session_data *sd;
	int i = 0;

	if(script_rid2sd(sd))
		i = battleground_countlogin(sd,true);

	script_pushint(st,i);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_team_reveal)
{
	int bg_id;

	bg_id = script_getnum(st,2);
	std::shared_ptr<s_battleground_data> bg = util::umap_find(bg_team_db, bg_id);
	if( !bg )
		return SCRIPT_CMD_SUCCESS;

	bg->reveal_pos = true; // Reveal Position Mode
	return SCRIPT_CMD_SUCCESS;
}
BUILDIN_FUNC(bg_leavebg)
{
	if (!battle_config.feature_bgqueue) {
		script_pushint(st, false);
		return SCRIPT_CMD_SUCCESS;
	}

	const char* name = script_getstr(st, 2);
	struct map_session_data* sd = NULL;

	if (!script_rid2sd(sd)) {
		ShowError("bg_joinbg: player not attached.\n");
		script_pushint(st, false);
		return SCRIPT_CMD_FAILURE;
	}

	bool success;

	if (sd->bg_queue_id > 0) {
		std::shared_ptr<s_battleground_queue> queue = bg_search_queue(sd->bg_queue_id);

		if (queue && queue->state == QUEUE_STATE_SETUP_DELAY) {
			script_pushint(st, false);
			return SCRIPT_CMD_SUCCESS; // Make the cancel button do nothing if the entry window is open. Otherwise it'll crash the game when you click on both the queue status and entry status window.
		} else
			success = bg_queue_leave(sd);

		clif_bg_queue_cancel_result(success, name, sd);
		script_pushint(st, true);
	}
	else {
		script_pushint(st, false);
	}

	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_modeselection)
{
	script_pushint(st, battle_config.bg_mode_selection);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_current_mode)
{
	int mode = bg_get_mode();
	script_pushint(st, mode);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_set_current_mode)
{
	int mode = script_getnum(st, 2);
	bg_set_mode(mode);
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_joinbg)
{
	if (!battle_config.feature_bgqueue)
		return SCRIPT_CMD_SUCCESS;

	const char* name = script_getstr(st, 2);
	struct map_session_data* sd = NULL;

	if (!script_rid2sd(sd)) {
		ShowError("bg_joinbg: player not attached.\n");
		return SCRIPT_CMD_FAILURE;
	}

	if (sd->bg_queue_id > 0) {
		//ShowWarning("clif_parse_bg_queue_apply_request: Received duplicate queue application: %d from player %s (AID:%d CID:%d).\n", type, sd->status.name, sd->status.account_id, sd->status.char_id);
		clif_bg_queue_apply_result(BG_APPLY_DUPLICATE, name, sd); // Duplicate application warning
		return SCRIPT_CMD_SUCCESS;
	}

	bg_queue_join_solo(name, sd);

	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(bg_premature_end)
{
	int bgid = script_getnum(st, 2);

	for (const auto& queue : bg_queues) {
		if (queue->id != bgid || queue->state == QUEUE_STATE_SETUP_DELAY || queue->state == QUEUE_STATE_ENDED)
			continue;
		if (queue->state == QUEUE_STATE_ACTIVE && queue->map) {
			npc_event_do(queue->map->bgendcallscript.c_str());
			break;
		}
	}

	return SCRIPT_CMD_SUCCESS;
}

/*==========================================
 * Ranking Reset
 *------------------------------------------*/
BUILDIN_FUNC(bg_rankpoints)
{
	struct map_session_data *sd;
	std::shared_ptr<s_battleground_data> bg;
	const char *type;
	int i, add_value;

	if( script_hasdata(st,4) )
		sd = map_id2sd(script_getnum(st,4));
	else
		script_rid2sd(sd);

	if( sd == NULL )
		return 0;

	if( !sd->bg_id || !(bg = util::umap_find(bg_team_db, sd->bg_id)) )
		return 0;
	ARR_FIND(0,MAX_BG_MEMBERS,i,bg->members[i].sd == sd);
	if( i >= MAX_BG_MEMBERS )
		return 0;

	type = script_getstr(st,2);
	add_value = script_getnum(st,3);

	// Just +1 Fame Point
	if( !strcmpi(type,"fame") )
		pc_addfame(sd,add_value,3);
	// Normal Ranking actions
	else if( !strcmpi(type,"eos_flags") )
	{
		add2limit(sd->status.bgstats.eos_flags,add_value,USHRT_MAX);
		pc_addfame(sd,5,3);
	}
	else if( !strcmpi(type,"sc_stole") )
	{
		add2limit(sd->status.bgstats.sc_stole,add_value,USHRT_MAX);
		pc_addfame(sd,1,3);
	}
	else if( !strcmpi(type,"sc_captured") )
	{
		add2limit(sd->status.bgstats.sc_captured,add_value,USHRT_MAX);
		pc_addfame(sd,5,3);
	}
	else if( !strcmpi(type,"sc_droped") )
	{
		add2limit(sd->status.bgstats.sc_droped,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"ctf_taken") )
	{
		add2limit(sd->status.bgstats.ctf_taken,add_value,USHRT_MAX);
		pc_addfame(sd,1,3);
	}
	else if( !strcmpi(type,"ctf_captured") )
	{
		add2limit(sd->status.bgstats.ctf_captured,add_value,USHRT_MAX);
		pc_addfame(sd,25,3);
	}
	else if( !strcmpi(type,"ctf_droped") )
	{
		add2limit(sd->status.bgstats.ctf_droped,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"dom_off_kills") )
	{
		add2limit(sd->status.bgstats.dom_off_kills,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"dom_def_kills") )
	{
		add2limit(sd->status.bgstats.dom_def_kills,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"pb_kills") )
	{
		add2limit(sd->status.bgstats.pb_kills,add_value,USHRT_MAX);
		pc_addfame(sd,3,3);
	}
	else if( !strcmpi(type,"pb_kill_surface") )
	{
		add2limit(sd->status.bgstats.pb_kill_surface,add_value,USHRT_MAX);
		pc_addfame(sd,15,3);
	}
	else if( !strcmpi(type,"pb_scored") )
	{
		add2limit(sd->status.bgstats.pb_scored,add_value,USHRT_MAX);
		pc_addfame(sd,25,3);
	}
	else if( !strcmpi(type,"pb_score_own") )
	{
		add2limit(sd->status.bgstats.pb_score_own,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"pb_score_penalty") )
	{
		add2limit(sd->status.bgstats.pb_score_penalty,add_value,USHRT_MAX);
		pc_addfame(sd,20,3);
	}
	else if( !strcmpi(type,"pb_deaths") )
	{
		add2limit(sd->status.bgstats.pb_deaths,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"pb_death_surface") )
	{
		add2limit(sd->status.bgstats.pb_death_surface,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"td_death_wfumbi") )
	{
		add2limit(sd->status.bgstats.td_death_wfumbi,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"td_death_fumbi") )
	{
		add2limit(sd->status.bgstats.td_death_fumbi,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"td_deaths") )
	{
		add2limit(sd->status.bgstats.td_deaths,add_value,USHRT_MAX);
	}
	else if( !strcmpi(type,"td_scored") )
	{
		add2limit(sd->status.bgstats.td_scored,add_value,USHRT_MAX);
		pc_addfame(sd,25,3);
	}
	else if( !strcmpi(type,"td_taken") )
	{
		add2limit(sd->status.bgstats.td_taken,add_value,USHRT_MAX);
		pc_addfame(sd,1,3);
	}
	else if( !strcmpi(type,"td_kill_wfumbi") )
	{
		add2limit(sd->status.bgstats.td_kill_wfumbi,add_value,USHRT_MAX);
		pc_addfame(sd,6,3);
	}
	else if( !strcmpi(type,"td_kill_fumbi") )
	{
		add2limit(sd->status.bgstats.td_kill_fumbi,add_value,USHRT_MAX);
		pc_addfame(sd,5,3);
	}
	else if( !strcmpi(type,"td_kills") )
	{
		add2limit(sd->status.bgstats.td_kills,add_value,USHRT_MAX);
		pc_addfame(sd,1,3);
	}

	return 0;
}

BUILDIN_FUNC(bg_rankpoints_area)
{
	const char *str, *type;
	int m, x0, y0, x1, y1, bg_id;
	int i = 0, add_value;
	int type_val;
	std::shared_ptr<s_battleground_data> bg;
	struct map_session_data *sd;

	bg_id = script_getnum(st,2);
	str = script_getstr(st,3);

	if( !(bg = util::umap_find(bg_team_db, bg_id)) || (m = map_mapname2mapid(str)) < 0 )
	{
		script_pushint(st,0);
		return 0;
	}

	x0 = script_getnum(st,4);
	y0 = script_getnum(st,5);
	x1 = script_getnum(st,6);
	y1 = script_getnum(st,7);
	type = script_getstr(st,8);

	if( !strcmpi(type,"eos_bases") )
		type_val = 1;
	else if( !strcmpi(type,"dom_bases") )
		type_val = 2;
	else if( !strcmpi(type,"pb_sixyard") )
		type_val = 3;
	else if( !strcmpi(type,"pb_penalty") )
		type_val = 4;
	else return 0; // Invalid Type

	add_value = script_getnum(st,9);
	
	for (const auto& pl_sd : bg->members) {
		sd = pl_sd.sd;

		if( sd == NULL )
			continue;
		if( sd->bl.m != m || sd->bl.x < x0 || sd->bl.y < y0 || sd->bl.x > x1 || sd->bl.y > y1 )
			continue;

		switch( type_val )
		{
		case 1:
			add2limit(sd->status.bgstats.eos_bases,add_value,USHRT_MAX);
			pc_addfame(sd,10,3);
			break;
		case 2:
			add2limit(sd->status.bgstats.dom_bases,add_value,USHRT_MAX);
			pc_addfame(sd,10,3);
			break;
		case 3:
			add2limit(sd->status.bgstats.pb_sixyard,add_value,USHRT_MAX);
			pc_addfame(sd,10,3);
			break;
		case 4:
			add2limit(sd->status.bgstats.pb_penalty,add_value,USHRT_MAX);
			pc_addfame(sd,10,3);
			break;
		}
	}

	return 0;
}
