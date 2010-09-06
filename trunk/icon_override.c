/*
http://eion.robbmob.com/purple-blist-sort.png
*/

#define PURPLE_PLUGINS

#include <glib.h>
#include <plugin.h>
#include <account.h>
#include <accountopt.h>

#ifndef _
#	define _(a) a
#endif

#define NEW_ICON_ID "eionrobb_new_prpl_icon"
static GHashTable *original_list_icon = NULL;

const char *
new_list_icon(PurpleAccount *account, PurpleBuddy *buddy)
{
	const char*(* list_icon_func)(PurpleAccount *account, PurpleBuddy *buddy);
	const char *prpl_id;
	const char *new_icon;
	PurpleAccount *acct;
	
	if (account != NULL)
		acct = account;
	else if (buddy != NULL)
		acct = buddy->account;
	else
		return "";
	
	new_icon = purple_account_get_string(account, NEW_ICON_ID, NULL);
	if (new_icon && *new_icon)
		return new_icon;
	
	prpl_id = account->protocol_id;
	list_icon_func = g_hash_table_lookup(original_list_icon, prpl_id);
	return list_icon_func(account, buddy);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	GList *proto_list;
	PurplePlugin *proto;
	PurpleAccountOption *option;
	PurplePluginProtocolInfo *prpl_info;
	
	//Hook into every protocol's prpl_info->list_icon function
	original_list_icon = g_hash_table_new(g_str_hash, g_str_equal);
	
	proto_list = purple_plugins_get_protocols();
	
	for(; proto_list; proto_list = proto_list->next)
	{
		proto = proto_list->data;
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(proto);
	
		g_hash_table_insert(original_list_icon, (gpointer)purple_plugin_get_id(proto), prpl_info->list_icon);
		prpl_info->list_icon = new_list_icon;

		//Add in some protocol preferences
		option = purple_account_option_string_new(_("Protocol Icon"), NEW_ICON_ID, "");
		prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
	}
	
	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	//Unhook
	GList *proto_list;
	gpointer func;
	GList *list, *llist;
	PurplePluginProtocolInfo *prpl_info;
	PurpleAccountOption *option;
	PurplePlugin *proto;
	
	proto_list = purple_plugins_get_protocols();
	for(; proto_list; proto_list = proto_list->next)
	{
		proto = (PurplePlugin *) proto_list->data;
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(proto);
		func = g_hash_table_lookup(original_list_icon, purple_plugin_get_id(proto));
		if (func)
		{
			prpl_info->list_icon = func;
			g_hash_table_remove(original_list_icon, purple_plugin_get_id(proto));
		}
		
		//Remove from the protocol options screen
		list = prpl_info->protocol_options;
		while(list != NULL)
		{
			option = (PurpleAccountOption *) list->data;
			if (g_str_equal(purple_account_option_get_setting(option), NEW_ICON_ID))
			{
				llist = list;
				if (llist->prev)
					llist->prev->next = llist->next;
				if (llist->next)
					llist->next->prev = llist->prev;
				
				purple_account_option_destroy(option);
				
				list = g_list_next(list);
				g_list_free_1(llist);
			} else {
				list = g_list_next(list);
			}
		}
	}
	g_hash_table_destroy(original_list_icon);
	original_list_icon = NULL;
	
	return TRUE;
}

static void
plugin_init(PurplePlugin *plugin)
{
}

static PurplePluginInfo info = 
{
	PURPLE_PLUGIN_MAGIC,
	2,
	2,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_LOWEST,

	"eionrobb-icon-override",
	"Protocol Icon Override",
	"0.1",
	"Customise protocol icons",
	"Lets you change protocol icons per-account so that you can tell the difference between, say, a personal XMPP account and one used for work",
	"Eion Robb <eionrobb@gmail.com>",
	"http://pidgin-icon-override.googlecode.com/", //URL
	
	plugin_load,
	plugin_unload,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(icon_override, plugin_init, info);
