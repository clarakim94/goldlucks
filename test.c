#include "test.h"
#include <sqlite3.h>
#include <stdlib.h>

typedef struct appdata {
   Evas_Object *win;
   Evas_Object *conform;
   Evas_Object *label;

   Evas_Object *entry1;//?대쫫
   Evas_Object *entry2;//?곸뼱?먯닔
   Evas_Object *entry3;//?섑븰?먯닔
   Evas_Object *list;
   sqlite3 *db; // Database handle
   char *current_key;
} appdata_s;

typedef struct recdata {
   char key[10];
   char name[255];
   char english[10];
   char math[10];
} recdata_s;

appdata_s *m_ad;

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
   appdata_s *ad = data;
   /* Let window go to hide state. */
   //elm_win_lower(ad->win);
   elm_win_iconified_set(ad->win,EINA_TRUE);
}

//static void my_box_pack(Evas_Object* box, Evas_Object* child, double h_weight, double v_weight, double h_align, double v_align){
//   evas_object_size_hint_weight_set(child, h_weight, v_weight);
//   evas_object_size_hint_align_set(child,h_align,v_align);
//   evas_object_show(child);
//   elm_box_pack_end(box,child);
//}

static void my_table_pack(Evas_Object *table, Evas_Object *child, int x, int y,
      int w, int h) {
   evas_object_size_hint_align_set(child, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(child, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_table_pack(table, child, x, y, w, h);
   evas_object_show(child);
}

static int CreateTable(appdata_s *ad) {
   char *ErrMsg;
   char *sql =
         "CREATE TABLE IF NOT EXISTS Report(KEY INTEGER PRIMARY KEY, NAME TEXT NOT NULL, ENGLISH INT NOT NULL, MATH INT NOT NULL);";
   int ret = sqlite3_exec(ad->db, sql, NULL, 0, &ErrMsg);
   if(!ret){
      dlog_print(DLOG_INFO, LOG_TAG, "create success");
   }
   if(ret){
      dlog_print(DLOG_INFO,LOG_TAG,"create fail");
      sqlite3_free(ErrMsg);
   }
   return 0;
}

static void list_item_clicked(void *data, Evas_Object *obj, void *event_info) {
   recdata_s* rd = (recdata_s*) data;

   m_ad->current_key = rd->key;
   elm_object_text_set(m_ad->entry1, rd->name);
   elm_object_text_set(m_ad->entry2, rd->english);
   elm_object_text_set(m_ad->entry3, rd->math);
}

/*1媛쒖쓽 ?덉퐫?쒕? 諛쏆븘??泥섎━?섎뒗 肄쒕갚 ?⑥닔
 * 3踰덉㎏ ?뚮씪誘명꽣??諛곗뿴濡??곗씠?곌? ?꾨떖*/
static int db_read_cb(void *counter, int argc, char **argv, char **azColName) {
   char buf[255];
   dlog_print(DLOG_INFO, LOG_TAG, "enter cb");
   recdata_s* rd = malloc(sizeof(recdata_s));
   /*recdata_s 援ъ“泥대? ?앹꽦?댁꽌 媛??꾨뱶 ?곗씠?곕? ??ν븯怨?    * 紐⑤뱺 ?곗씠?곕? ?섎굹??臾몄옄?대줈 ?⑹퀜??List ?꾩젽???덈줈????ぉ??異붽??⑸땲??
     */
   strcpy(rd->key, argv[0]);
   strcpy(rd->name, argv[1]);
   strcpy(rd->english, argv[2]);
   strcpy(rd->math, argv[3]);
   sprintf(buf, "%s / %s / %s / %s", argv[0], argv[1], argv[2], argv[3]);
   elm_list_item_append(m_ad->list, buf, NULL, NULL, list_item_clicked, (void*) rd);
   dlog_print(DLOG_INFO, LOG_TAG, "list append");
   elm_list_go(m_ad->list);
   dlog_print(DLOG_INFO, LOG_TAG, "list go");
   evas_object_show(m_ad->list);
   dlog_print(DLOG_INFO, LOG_TAG, "show");
   return 0;
}

/*DB????λ맂 紐⑤뱺 ?곗씠?곕? ?쎌뼱??List?꾩젽??異쒕젰?섎뒗 ?⑥닔
 * ?깆씠 ?ㅽ뻾???뚯? Button???뚮??????몄텧*/
static void read_db(appdata_s *ad) {
   char *sql = "select * from Report";
   int counter = 0;
   char *ErrMsg;

   elm_list_clear(ad->list);
   int ret = sqlite3_exec(ad->db, sql, db_read_cb, &counter, &ErrMsg);
   if(ret){
         dlog_print(DLOG_INFO, LOG_TAG, "select fail");
         sqlite3_free(ErrMsg);
   }
   //return 0;
}

static void init_db(appdata_s *ad) {
   sqlite3_shutdown();
   sqlite3_config(SQLITE_CONFIG_URI, 1);
   sqlite3_initialize();

   char * resource = app_get_data_path();
   int siz = strlen(resource) + 10;
   char * path = malloc(sizeof(char) * siz);

   strncat(path, resource, siz);
   strncat(path, "test.db", siz);
   sqlite3_open(path, &ad->db);

   free(path);
   CreateTable(ad);
   read_db(ad);
}

static int InsertRecord(appdata_s *ad, unsigned char *name, int english,
      int math) {
   char sql[256];
   char *ErrMsg;
   snprintf(sql, 256, "INSERT INTO Report VALUES(NULL,\'%s\',%d,%d);",
         name, english, math);
   int ret = sqlite3_exec(ad->db, sql, NULL, 0, &ErrMsg);
   if(ret){
      dlog_print(DLOG_INFO, LOG_TAG, ErrMsg);
      sqlite3_free(ErrMsg);
      return 1;
   }
   return 0;
}

static void btn_add_cb(void *data, Evas_Object *obj, void *event_info) {
   appdata_s *ad = data;

   char* s_name = elm_object_text_get(ad->entry1);
   char* s_english = elm_object_text_get(ad->entry2);
   int n_english = atoi(s_english);
   char* s_math = elm_object_text_get(ad->entry3);
   int n_math = atoi(s_math);

   InsertRecord(ad, s_name, n_english, n_math);


   read_db(ad);
}

static int DelRecord(appdata_s *ad) {
   char sql[256];
   char *ErrMsg;
   snprintf(sql, 256, "DELETE FROM Report WHERE KEY=\'%s\';",
         ad->current_key);
   int ret = sqlite3_exec(ad->db, sql, NULL, 0, &ErrMsg);
   if(ret){
         dlog_print(DLOG_INFO, LOG_TAG, "Delete fail");
         sqlite3_free(ErrMsg);
   }
   return 0;
}

static void btn_del_cb(void *data, Evas_Object *obj, void *event_info) {
   appdata_s *ad = data;
   DelRecord(ad);
   read_db(ad);
}

static int UpdateRecord(appdata_s *ad, unsigned char *name,
      unsigned char *english, unsigned char *math) {
   char sql[256];
   char *ErrMsg;
   snprintf(sql, 256, "UPDATE Report SET NAME=\'%s\', ENGLISH=\'%s\',MATH=\'%s\' WHERE KEY=\'%s\';",
         name, english, math, ad->current_key);
   int ret = sqlite3_exec(ad->db, sql, NULL, 0, &ErrMsg);

   return ret;
}

static void btn_update_cb(void *data, Evas_Object *obj, void *event_info) {
   appdata_s *ad = data;

   char* s_name = elm_object_text_get(ad->entry1);
   char* s_english = elm_object_text_get(ad->entry2);
   char* s_math = elm_object_text_get(ad->entry3);

   UpdateRecord(ad, s_name, s_english, s_math);

   read_db(ad);
}

static void
create_base_gui(appdata_s *ad)
{
   m_ad = ad;

   elm_policy_set(ELM_POLICY_QUIT,ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   /* Window */
   /* Create and initialize elm_win.
      elm_win is mandatory to manipulate window. */
   ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
   elm_win_autodel_set(ad->win, EINA_TRUE);

   int rots[4] = { 0, 90, 180, 270 };
   elm_win_wm_rotation_available_rotations_set(ad->win, (const int *) (&rots),
         4);

   eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

   /* Conformant */
   /* Create and initialize elm_conformant.
      elm_conformant is mandatory for base gui to have proper size
      when indicator or virtual keypad is visible. */
   ad->conform = elm_conformant_add(ad->win);
   elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
   elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
   evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(ad->win, ad->conform);
   evas_object_show(ad->conform);

   {
      /* child object - indent to how relationship */
      /* A box to put things in verticallly - default mode for box */
      Evas_Object *box = elm_box_add(ad->win);
      evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
      elm_object_content_set(ad->conform, box);
      evas_object_show(box);

      /* Table */
      Evas_Object * table = elm_table_add(ad->win);
      /* Make table homogenous - every cell will be the same size */
      elm_table_homogeneous_set(table, EINA_TRUE);
      /* Set padding of 10 pixels multiplied by scale factor of UI */
      elm_table_padding_set(table, 10 * elm_config_scale_get(),10 * elm_config_scale_get());
      /* Let the table child allocation area expand within in the box */
      evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
      /* Set table to fiill width but align to bottom of box */
      evas_object_size_hint_align_set(table, EVAS_HINT_FILL, 0.0);
      elm_box_pack_end(box,table);
      evas_object_show(table);

      {
         /* Bg-1 */
         Evas_Object *bg = elm_bg_add(ad->conform);
         elm_bg_color_set(bg, 210, 210, 210);
         my_table_pack(table, bg, 0, 0, 1, 1);

         /* Entry-1 */
         ad->entry1 = elm_entry_add(ad->conform);
         elm_object_part_text_set(ad->entry1, "elm.guide", "Name");
         my_table_pack(table, ad->entry1, 0, 0, 1, 1);

         /* Bg-2 */
         bg = elm_bg_add(ad->conform);
         elm_bg_color_set(bg, 210, 210, 210);
         my_table_pack(table, bg, 1, 0, 1, 1);

         /* Entry-2 */
         ad->entry2 = elm_entry_add(ad->conform);
         elm_object_part_text_set(ad->entry2, "elm.guide", "English");
         my_table_pack(table, ad->entry2, 1, 0, 1, 1);

         /* Bg-3 */
         bg = elm_bg_add(ad->conform);
         elm_bg_color_set(bg, 210, 210, 210);
         my_table_pack(table, bg, 2, 0, 1, 1);

         /* Entry-3 */
         ad->entry3 = elm_entry_add(ad->conform);
         elm_object_part_text_set(ad->entry3, "elm.guide", "Math");
         my_table_pack(table, ad->entry3, 2, 0, 1, 1);

         /* Button-Add */
         Evas_Object *btn = elm_button_add(ad->conform);
         elm_object_text_set(btn, "Add");
         evas_object_smart_callback_add(btn, "clicked", btn_add_cb, ad);
         my_table_pack(table, btn, 0, 1, 1, 1);

         /* Button-Update */
         btn = elm_button_add(ad->conform);
         elm_object_text_set(btn, "Update");
         evas_object_smart_callback_add(btn, "clicked", btn_update_cb, ad);
         my_table_pack(table, btn, 1, 1, 1, 1);

         /* Button-Del */
         btn = elm_button_add(ad->conform);
         elm_object_text_set(btn, "Del");
         evas_object_smart_callback_add(btn, "clicked", btn_del_cb, ad);
         my_table_pack(table, btn, 2, 1, 1, 1);

         /* List */
         ad->list = elm_list_add(ad->conform);
         elm_list_mode_set(ad->list, ELM_LIST_COMPRESS);
         elm_list_go(ad->list);
         my_table_pack(table, ad->list, 0, 2, 3, 8);
      }
   }


   /* Show window after base gui is set up */
   evas_object_show(ad->win);

   init_db(ad);
}

static bool
app_create(void *data)
{
   /* Hook to take necessary actions before main event loop starts
      Initialize UI resources and application's data
      If this function returns true, the main loop of application starts
      If this function returns false, the application is terminated */
   appdata_s *ad = data;

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
app_terminate(void *data)
{
   /* Release all resources. */
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

   //ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
   //ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
   //ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
   ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
   //ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

   ret = ui_app_main(argc, argv, &event_callback, &ad);
   if (ret != APP_ERROR_NONE) {
      dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
   }

   return ret;
}
