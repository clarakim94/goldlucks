/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "snsui.h"

typedef struct appdata {
	const char *name;
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *nf;
	Evas_Object *scroller;
	Evas_Object *tabbar;
	Evas_Object *first_view_rect;
	Evas_Object *second_view_rect;
	Evas_Object *third_view_rect;
	Evas_Object *fourth_view_rect;
} appdata_s;

static void
win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void
drawer_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	elm_panel_hidden_set(obj, EINA_TRUE);
}

static Evas_Object *
create_conform(Evas_Object *parent)
{
	Evas_Object *conform, *bg;

	if (parent == NULL) return NULL;

	conform = elm_conformant_add(parent);
	evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(parent, conform);

	bg = elm_bg_add(conform);
	elm_object_style_set(bg, "indicator/headerbg");
	elm_object_part_content_set(conform, "elm.swallow.indicator_bg", bg);
	evas_object_show(bg);

	evas_object_show(conform);
	return conform;
}

static void
btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *panel = data;
	if (!elm_object_disabled_get(panel)) elm_panel_toggle(panel);
}

static Evas_Object *
create_drawers_btn(Evas_Object *parent, Evas_Smart_Cb func, void *data)
{
	Evas_Object *btn = elm_button_add(parent);
	if (!btn) return NULL;
	elm_object_style_set(btn, "naviframe/drawers");
	evas_object_smart_callback_add(btn, "clicked", func, data);

	return btn;
}

static Evas_Object *
create_drawer_layout(Evas_Object *parent)
{
	Evas_Object *layout;
	layout = elm_layout_add(parent);
	elm_layout_theme_set(layout, "layout", "drawer","panel");
	evas_object_show(layout);

	return layout;
}

static Evas_Object *
create_bg(Evas_Object *parent)
{
	// Small white rectangle pointing the current tab
	Evas_Object *rect;
	rect = evas_object_rectangle_add(evas_object_evas_get(parent));
	evas_object_color_set(rect, 0, 0, 0, 0);
	evas_object_show(rect);

	return rect;
}

static void
panel_scroll_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Panel_Scroll_Info *ev = event_info;
	Evas_Object *bg = data;
	int col = 127 * ev->rel_x;
	evas_object_color_set(bg, 0, 0, 0, col);
}
static void
popup_btn_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *popup = data;
	evas_object_del(popup);
}

static void
list_item_clicked1(void *data, Evas_Object *obj, void *event_info) {
	//Evas_Object *win = data;
	/* popup */
	Evas_Object *popup;
	Evas_Object *btn1;
	popup = elm_popup_add(data);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, NULL);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(popup,"'Analysis' button is clicked");

	/* ok button */
	btn1 = elm_button_add(popup);
	elm_object_style_set(btn1, "popup");
	elm_object_text_set(btn1, "OK");
	elm_object_part_content_set(popup, "button1", btn1);
	evas_object_smart_callback_add(btn1, "clicked", popup_btn_clicked_cb, popup);
	evas_object_show(popup);
}
static void
list_item_clicked2(void *data, Evas_Object *obj, void *event_info) {
	//Evas_Object *win = data;
	/* popup */
	Evas_Object *popup;
	Evas_Object *btn1;
	popup = elm_popup_add(data);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, NULL);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(popup,"'To Share' button is clicked");

	/* ok button */
	btn1 = elm_button_add(popup);
	elm_object_style_set(btn1, "popup");
	elm_object_text_set(btn1, "OK");
	elm_object_part_content_set(popup, "button1", btn1);
	evas_object_smart_callback_add(btn1, "clicked", popup_btn_clicked_cb, popup);
	evas_object_show(popup);
}
static void
list_item_clicked3(void *data, Evas_Object *obj, void *event_info) {
	/* popup */
	Evas_Object *popup;
	Evas_Object *btn1;
	popup = elm_popup_add(data);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, NULL);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(popup,"'Fixed Expenses' button is clicked");

	/* ok button */
	btn1 = elm_button_add(popup);
	elm_object_style_set(btn1, "popup");
	elm_object_text_set(btn1, "OK");
	elm_object_part_content_set(popup, "button1", btn1);
	evas_object_smart_callback_add(btn1, "clicked", popup_btn_clicked_cb, popup);
	evas_object_show(popup);
}
static void
list_item_clicked4(void *data, Evas_Object *obj, void *event_info) {
	//Evas_Object *win = data;
	/* popup */
	Evas_Object *popup;
	Evas_Object *btn1;
	popup = elm_popup_add(data);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, NULL);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(popup,"'Settings' button is clicked");

	/* ok button */
	btn1 = elm_button_add(popup);
	elm_object_style_set(btn1, "popup");
	elm_object_text_set(btn1, "OK");
	elm_object_part_content_set(popup, "button1", btn1);
	evas_object_smart_callback_add(btn1, "clicked", popup_btn_clicked_cb, popup);
	evas_object_show(popup);
}
static Evas_Object *
create_panel(Evas_Object *parent)
{
	// List includes 20 objects
	Evas_Object *panel, *list;
	//int i;
	char buf[64];

	/* Panel */
	panel = elm_panel_add(parent);
	elm_panel_scrollable_set(panel, EINA_TRUE);

	/* Default is visible, hide the content in default. */
	elm_panel_hidden_set(panel, EINA_TRUE);
	elm_panel_orient_set(panel, ELM_PANEL_ORIENT_LEFT);

	/* Panel content */
	list = elm_list_add(panel);
	evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND,  EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);

	sprintf(buf, "<font_size=80>Gold Lucks</font_size>");
	elm_list_item_append(list, buf, NULL, NULL, NULL,NULL);
	sprintf(buf, "ANALYSIS");
	elm_list_item_append(list, buf, NULL, NULL, list_item_clicked1, parent);
	sprintf(buf, "TO SHARE");
	elm_list_item_append(list, buf, NULL, NULL, list_item_clicked2, parent);
	sprintf(buf, "FIXED EXPENSES");
	elm_list_item_append(list, buf, NULL, NULL, list_item_clicked3, parent);
	sprintf(buf, "SETTINGS");
	elm_list_item_append(list, buf, NULL, NULL, list_item_clicked4, parent);
	evas_object_show(list);

	elm_object_content_set(panel, list);
	evas_object_show(panel);
	return panel;
}

static void
tabbar_first_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_item_part_text_set(elm_naviframe_top_item_get(ad->nf), "elm.text.title", "News Feed");
	elm_scroller_page_bring_in(ad->scroller, 0, 0);
}

static void
tabbar_second_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_item_part_text_set(elm_naviframe_top_item_get(ad->nf), "elm.text.title", "Friends");
	elm_scroller_page_bring_in(ad->scroller, 1, 0);
}

static void
tabbar_third_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_item_part_text_set(elm_naviframe_top_item_get(ad->nf), "elm.text.title", "Message");
	elm_scroller_page_bring_in(ad->scroller, 2, 0);
}

static void
tabbar_fourth_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_item_part_text_set(elm_naviframe_top_item_get(ad->nf), "elm.text.title", "Notice");
	elm_scroller_page_bring_in(ad->scroller, 3, 0);
}

static Evas_Object *
create_tabbar(Evas_Object *parent, appdata_s *ad)
{
	Evas_Object *tabbar;

	tabbar = elm_toolbar_add(parent);
	elm_toolbar_shrink_mode_set(tabbar, ELM_TOOLBAR_SHRINK_EXPAND);
	elm_toolbar_transverse_expanded_set(tabbar, EINA_TRUE);
	elm_toolbar_select_mode_set(tabbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
	elm_toolbar_item_append(tabbar, ICON_DIR"/00_controlbar_icon_playlist.png", NULL, tabbar_first_cb, ad);
	elm_toolbar_item_append(tabbar, ICON_DIR"/00_controlbar_icon_artists.png", NULL, tabbar_second_cb, ad);
	elm_toolbar_item_append(tabbar, ICON_DIR"/00_controlbar_icon_dialer.png", NULL, tabbar_third_cb, ad);
	elm_toolbar_item_append(tabbar, ICON_DIR"/00_controlbar_icon_more.png", NULL, tabbar_fourth_cb, ad);
	evas_object_show(tabbar);

	return tabbar;
}

static void
anim_stop_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	Elm_Object_Item *item;
	int index, page = 0;

	elm_scroller_current_page_get(ad->scroller, &page, NULL);

	item = elm_toolbar_first_item_get(ad->tabbar);

	for (index = 0; index < page; index++)
		item = elm_toolbar_item_next_get(item);
	elm_toolbar_item_selected_set(item, EINA_TRUE);
}

static Evas_Object *
create_scroller(Evas_Object *parent, appdata_s *ad)
{
	Evas_Object *scroller;

	scroller = elm_scroller_add(parent);
	elm_object_style_set(scroller, "tabbar");
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_OFF);
	elm_scroller_page_size_set(scroller, 0, 0);
	elm_scroller_page_scroll_limit_set(scroller, 1, 0);
	elm_scroller_single_direction_set(scroller, ELM_SCROLLER_SINGLE_DIRECTION_HARD);
	evas_object_smart_callback_add(scroller, "scroll,anim,stop", anim_stop_cb, ad);
	evas_object_show(scroller);

	return scroller;
}

static Evas_Object *
min_set(Evas_Object *obj, Evas_Object *box, Evas_Coord w, Evas_Coord h)
{
	Evas_Object *table, *rect;

	table = elm_table_add(box);
	evas_object_show(table);

	rect = evas_object_rectangle_add(evas_object_evas_get(table));
	evas_object_size_hint_min_set(rect, w, h);
	evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_table_pack(table, rect, 0, 0, 1, 1);

	evas_object_size_hint_weight_set(obj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(obj, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_table_pack(table, obj, 0, 0, 1, 1);

	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, table);

	return rect;
}
// box repeated in a list 20 times
static Evas_Object *
create_box_content(Evas_Object *parent)
{
	Evas_Object *box, *box2, *label, *btn;

	box = elm_box_add(parent);
	// put a label into the box
	label = elm_label_add(box);
	evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
	elm_object_text_set(label, "SAVE OUR EWHA");
	elm_box_pack_end(box, label);//////////////
	evas_object_show(label);

	// put a box2 including buttons("I like it", "Message", "Share") into the box
	box2 = elm_box_add(box);
	evas_object_size_hint_weight_set(box2, EVAS_HINT_EXPAND, 0);
	evas_object_size_hint_align_set(box2, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_horizontal_set(box2, EINA_TRUE);
	elm_box_padding_set(box2, ELM_SCALE_SIZE(15), 0);
	elm_box_pack_end(box, box2);/////////////
	evas_object_show(box2);

	btn = elm_button_add(box2);
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_text_set(btn, "I like it");
	elm_box_pack_end(box2, btn);/////////////
	evas_object_show(btn);

	btn = elm_button_add(box2);
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_text_set(btn, "Message");
	elm_box_pack_end(box2, btn);/////////////
	evas_object_show(btn);

	btn = elm_button_add(box2);
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_text_set(btn, "Share");
	elm_box_pack_end(box2, btn);/////////////
	evas_object_show(btn);

	evas_object_show(box);

	return box;
}

static Evas_Object *
create_first_view(Evas_Object *parent, appdata_s *ad)
{
	Evas_Object *scroller, *box, *content;
	int i;

	scroller = elm_scroller_add(parent);
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
	elm_scroller_single_direction_set(scroller, ELM_SCROLLER_SINGLE_DIRECTION_HARD);

	box = elm_box_add(scroller);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	// padding between boxes
	elm_box_padding_set(box, 0, ELM_SCALE_SIZE(30));
	// put box into scroller
	elm_object_content_set(scroller, box);

	// little box repeated in a list 20 times
	for (i = 0; i < 20; i++) {
		content = create_box_content(box);
		min_set(content, box, 0, 0);
	}

	evas_object_show(scroller);

	return scroller;
}

static void
gl_sel(void *data, Evas_Object *obj, void *event_info)
{
	elm_genlist_item_selected_set((Elm_Object_Item *)event_info, EINA_FALSE);
}

static char*
gl_friends_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.text"))
		return strdup("Friend");
	else if (!strcmp(part, "elm.text.sub"))
		return strdup("Will you add the friend?");
	return NULL;
}

static char *
gl_notice_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.text"))
		return strdup("Account setting");
	else if (!strcmp(part, "elm.text.end"))
		return strdup("On");
	return NULL;
}

static Evas_Object *
gl_icon_get(void *data, Evas_Object *obj, const char *part)
{
	Evas_Object *icon = elm_image_add(obj);
	elm_image_file_set(icon, ICON_DIR"/picture.jpg", NULL);
	evas_object_size_hint_align_set(icon, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	return icon;
}

static Evas_Object *
gl_content_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.swallow.end")) {
		Evas_Object *button = elm_button_add(obj);
		elm_object_text_set(button, "+");
		evas_object_propagate_events_set(button, EINA_FALSE);
		evas_object_repeat_events_set(button, EINA_FALSE);
		evas_object_size_hint_min_set(button, 60, 80);
		return button;
	}
	return gl_icon_get(data, obj, part);
}

static Evas_Object *
create_second_view(Evas_Object *parent, appdata_s *ad)
{
	Evas_Object *genlist;
	int index;

	if (ad == NULL) return NULL;

	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
	itc->item_style = "type1";
	itc->func.text_get = gl_friends_text_get;
	itc->func.content_get = gl_content_get;

	genlist = elm_genlist_add(parent);
	elm_scroller_single_direction_set(genlist, ELM_SCROLLER_SINGLE_DIRECTION_HARD);
	for (index = 0; index < NUM_OF_ITEMS; index++) {
		elm_genlist_item_append(
				genlist,
				itc,
				NULL,
				NULL,
				ELM_GENLIST_ITEM_NONE,
				gl_sel,
				NULL);
	}
	elm_genlist_item_class_free(itc);
	evas_object_show(genlist);

	return genlist;
}

static void
gg_sel(void *data, Evas_Object *obj, void *event_info)
{
	elm_gengrid_item_selected_set((Elm_Object_Item *)event_info, EINA_FALSE);
}

static Evas_Object *
gg_content_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.swallow.icon"))
		return gl_icon_get(data, obj, part);
	return gl_icon_get(data, obj, part);
}

static Evas_Object *
create_third_view(Evas_Object *parent, appdata_s *ad)
{
	Evas_Object *gengrid;
	int index;

	if (ad == NULL) return NULL;

	Elm_Gengrid_Item_Class *itc = elm_gengrid_item_class_new();
	itc->item_style = "default";
	itc->func.content_get = gg_content_get;

	gengrid = elm_gengrid_add(parent);
	elm_gengrid_item_size_set(gengrid, ELM_SCALE_SIZE(150), ELM_SCALE_SIZE(150));
	elm_gengrid_align_set(gengrid, 0.5, 0.5);
	elm_scroller_single_direction_set(gengrid, ELM_SCROLLER_SINGLE_DIRECTION_HARD);
	for (index = 0; index < NUM_OF_ITEMS; index++) {
		elm_gengrid_item_append(
				gengrid,
				itc,
				NULL,
				gg_sel,
				NULL);
	}
	elm_gengrid_item_class_free(itc);

	evas_object_show(gengrid);

	return gengrid;
}

static char *
gl_title_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.text"))
		return strdup("Setting");

	return NULL;
}

static Evas_Object *
create_fourth_view(Evas_Object *parent, appdata_s *ad)
{
	Evas_Object *genlist;
	Elm_Object_Item *item = NULL;
	int index;

	if (ad == NULL) return NULL;

	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
	Elm_Genlist_Item_Class *itc2 = elm_genlist_item_class_new();
	Elm_Genlist_Item_Class *itc3 = elm_genlist_item_class_new();

	itc->item_style = "type1";
	itc->func.text_get = gl_notice_text_get;

	itc2->item_style = "group_index";

	itc3->item_style = "group_index";
	itc3->func.text_get = gl_title_text_get;

	genlist = elm_genlist_add(parent);
	elm_scroller_single_direction_set(genlist, ELM_SCROLLER_SINGLE_DIRECTION_HARD);
	for (index = 0; index < NUM_OF_ITEMS; index++) {
		if (index % 6 == 0) {
			item = elm_genlist_item_append(
					genlist,
					itc2,
					NULL,
					NULL,
					ELM_GENLIST_ITEM_NONE,
					gl_sel,
					NULL);
			elm_genlist_item_select_mode_set(item,
					ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
		} else if (index % 6 == 1) {
			item = elm_genlist_item_append(
					genlist,
					itc3,
					NULL,
					NULL,
					ELM_GENLIST_ITEM_NONE,
					gl_sel,
					NULL);
			elm_genlist_item_select_mode_set(item,
					ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
		} else {
			item = elm_genlist_item_append(
					genlist,
					itc,
					NULL,
					NULL,
					ELM_GENLIST_ITEM_NONE,
					gl_sel,
					NULL);
		}
	}
	elm_genlist_item_class_free(itc);
	elm_genlist_item_class_free(itc2);
	elm_genlist_item_class_free(itc3);

	evas_object_show(genlist);

	return genlist;
}

static void
view_size_reset(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	Evas_Coord w, h;

	evas_object_geometry_get(ad->win, NULL, NULL, &w, NULL);
	evas_object_geometry_get(ad->scroller, NULL, NULL, NULL, &h);

	elm_scroller_page_size_set(ad->scroller, w, h);
	evas_object_size_hint_min_set(ad->first_view_rect, w, h);
	evas_object_size_hint_min_set(ad->second_view_rect, w, h);
	evas_object_size_hint_min_set(ad->third_view_rect, w, h);
	evas_object_size_hint_min_set(ad->fourth_view_rect, w, h);
}

static Evas_Object *
create_main_view(appdata_s *ad)
{
	Evas_Object *layout, *box, *box2;
	Evas_Object *first_view, *second_view, *third_view, *fourth_view;

	/* Layout for drawer */
	layout = create_drawer_layout(ad->nf);

	/* Box */
	box = elm_box_add(layout);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(layout, "elm.swallow.content", box);

	/* Tabbar */
	ad->tabbar = create_tabbar(box, ad);
	evas_object_size_hint_weight_set(ad->tabbar, EVAS_HINT_EXPAND, 0);
	evas_object_size_hint_align_set(ad->tabbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, ad->tabbar);

	/* Scroller */
	ad->scroller = create_scroller(box, ad);
	evas_object_size_hint_weight_set(ad->scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, ad->scroller);

	/* Box */
	box2 = elm_box_add(ad->scroller);
	elm_box_horizontal_set(box2, EINA_TRUE);
	evas_object_size_hint_weight_set(box2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box2, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_content_set(ad->scroller, box2);

	/* First view */
	first_view = create_first_view(box2, ad);
	ad->first_view_rect = min_set(first_view, box2, 0, 0);

	/* Second view */
	second_view = create_second_view(box2, ad);
	ad->second_view_rect = min_set(second_view, box2, 0, 0);

	/* third view */
	third_view = create_third_view(box2, ad);
	ad->third_view_rect = min_set(third_view, box2, 0, 0);

	/* Fourth view */
	fourth_view = create_fourth_view(box2, ad);
	ad->fourth_view_rect = min_set(fourth_view, box2, 0, 0);

	return layout;
}

static void
create_base_gui(appdata_s *ad)
{
	Evas_Object *layout, *bg, *drawer, *btn;
	Elm_Object_Item *nf_it;

	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	ad->conform = create_conform(ad->win);

	/* Naviframe */
	ad->nf = elm_naviframe_add(ad->conform);
	elm_object_content_set(ad->conform, ad->nf);

	layout = create_main_view(ad);
	nf_it = elm_naviframe_item_push(ad->nf, "News Feed", NULL, NULL, layout, "basic");

	/* Drawer bg */
	bg = create_bg(layout);
	elm_object_part_content_set(layout, "elm.swallow.bg", bg);

	/* Drawer */
	drawer = create_panel(layout);
	eext_object_event_callback_add(drawer, EEXT_CALLBACK_BACK, drawer_back_cb, ad);
	evas_object_smart_callback_add(drawer, "scroll", panel_scroll_cb, bg);
	elm_object_part_content_set(layout, "elm.swallow.right", drawer);

	/* Drawers Button */
	btn = create_drawers_btn(ad->nf, btn_cb, drawer);
	elm_object_item_part_content_set(nf_it, "title_left_btn", btn);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
	evas_object_event_callback_add(ad->win, EVAS_CALLBACK_RESIZE, view_size_reset, ad);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	elm_app_base_scale_set(1.8);
	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *user_data)
{
    /* Release all resources (*/
	appdata_s *ad;

	if (!user_data)
		return;

	ad = user_data;

	if (ad->win)
		evas_object_del(ad->win);
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);

	return ret;
}
