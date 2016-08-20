#include "goldlucks.h"
typedef struct item_data{
   int index;
   Elm_Object_Item *item;
}item_data_s;
typedef struct
{
   Evas_Object *win;
   Evas_Object *bg;
   Evas_Object *conform;
   Evas_Object *layout;
} window_obj;

typedef struct{
   window_obj *win;
   Evas_Object *naviframe;
   Elm_Object_Item *naviframe_item;

   Evas_Object *layout;
   Evas_Object *genlist;

   char *title_entry;
   char *description_entry;

   Elm_Object_Item *genlist_description_item;

}create_view_data;

enum _ENTRY_TYPE {
    TITLE_ENTRY,
    LOCATION_ENTRY,
    DESCRIPTION_ENTRY
};

typedef enum _ENTRY_TYPE ENTRY_TYPE;
/*these were in the config.h*/
static const char *TITLE                = "Title";
static const char *TITLE_HINT           = "Enter the title";
static const char *DESCRIPTION          = "Description";
static const char *DESCRIPTION_HINT     = "Enter the description";
#define FORMAT_BUFFER                   128
#define LABEL_FORMAT                    "<p font_size=%d>%s</p>"
#define ENTRY_LABEL_FORMAT              "<p color=#000 font_size=%d>%s</p>"
#define STYLE_FORMAT                    "DEFAULT='font_size=%d'"
#define ENTRY_SPACER_WIDTH              32
#define ENTRY_MIN_HEIGHT                140
#define ELM_ENTRY_MIN_HEIGHT ELM_SCALE_SIZE(ENTRY_MIN_HEIGHT)
#define ELM_ENTRY_SPACER_WIDTH ELM_SCALE_SIZE(ENTRY_SPACER_WIDTH)
#define ELM_BOX_PADDING ELM_SCALE_SIZE(BOX_PADDING)
#define TEXT_ITEM_DEFAULT_SIZE          40
#define TEXT_MAX_GOOD_SIZE              60
#define SUBJECT_CHAR_LIMIT              100
#define SUBJECT_BYTE_LIMIT              256

const int get_system_font_size()
{
   int fontSize = 0;
   Eina_Bool res = edje_text_class_get("entry", NULL, &fontSize);
   if (!res)
   {
      fontSize = TEXT_ITEM_DEFAULT_SIZE;
   }
   else if (fontSize < 0)
   {
      fontSize = -fontSize * TEXT_ITEM_DEFAULT_SIZE / 100;
   }

   if (fontSize > TEXT_MAX_GOOD_SIZE)
   {
      fontSize = TEXT_MAX_GOOD_SIZE;
   }

   return fontSize;
}

static Evas_Object *_prepare_entry_box (create_view_data *item_data, const char *name, const char *hint, Evas_Smart_Cb func){
   create_view_data *data = (create_view_data *)item_data;

   Evas_Object *box = elm_box_add(data->genlist);
   elm_box_horizontal_set(box, EINA_TRUE);
   evas_object_size_hint_min_set(box,0,ELM_ENTRY_MIN_HEIGHT);
   evas_object_show(box);

   Evas_Object *rect1 = evas_object_rectangle_add(box);
   evas_object_color_set(rect1,0,0,0,0);
   evas_object_size_hint_min_set(rect1,ELM_ENTRY_SPACER_WIDTH,ELM_ENTRY_MIN_HEIGHT);
   evas_object_show(rect1);

   Evas_Object *rect2 = evas_object_rectangle_add(box);
   evas_object_color_set(rect2,0,0,0,0);
   evas_object_size_hint_min_set(rect2,ELM_ENTRY_SPACER_WIDTH,ELM_ENTRY_MIN_HEIGHT);
   evas_object_show(rect2);

   Evas_Object *label = elm_label_add(box);
   char buffer [FORMAT_BUFFER]={0};
   snprintf(buffer,sizeof(buffer)-1,ENTRY_LABEL_FORMAT,get_system_font_size(),name);
   evas_object_show(label);

   Evas_Object *entry = elm_entry_add(box);
   elm_entry_single_line_set(entry,EINA_FALSE);
   elm_entry_scrollable_set(entry,EINA_TRUE);
   elm_scroller_policy_set(entry,ELM_SCROLLER_POLICY_OFF,ELM_SCROLLER_POLICY_AUTO);
   snprintf(buffer,sizeof(buffer)-1,STYLE_FORMAT,get_system_font_size());
   elm_entry_text_style_user_push(entry,buffer);
   Elm_Entry_Filter_Limit_Size limit = {SUBJECT_CHAR_LIMIT,SUBJECT_BYTE_LIMIT};
   elm_entry_markup_filter_append(entry,elm_entry_filter_limit_size,&limit);
   elm_entry_cursor_end_set(entry);
   elm_entry_cnp_mode_set(entry,ELM_CNP_MODE_PLAINTEXT);

   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry,EVAS_HINT_FILL,EVAS_HINT_FILL);

   snprintf(buffer,sizeof(buffer)-1,LABEL_FORMAT,get_system_font_size(),hint);
   elm_object_part_text_set(entry,"elm.guide",buffer);
   evas_object_smart_callback_add(entry,"changed",func,data);
   evas_object_show(entry);

   elm_box_pack_end(box,rect1);
   elm_box_pack_end(box,label);
   elm_box_pack_end(box,rect2);
   elm_box_pack_end(box,entry);

   return box;

}

static void _title_entry_changed_cb(void *data,Evas_Object *obj,void *event_info){
   create_view_data *ad = (create_view_data *)data;
   if(ad->title_entry != NULL){
      free(ad->title_entry);
      ad->title_entry=NULL;
   }
   ad->title_entry = strdup(elm_entry_entry_get(obj));
}

static Evas_Object *_entry_add(create_view_data *data, ENTRY_TYPE entry_type){
   Evas_Object *box = NULL;
   switch(entry_type){
   case TITLE_ENTRY:
      box = _prepare_entry_box(data,TITLE,TITLE_HINT,_title_entry_changed_cb);
      break;
   }

   return box;
}

static Evas_Object *_title_content_cb(void *item_data,Evas_Object *obj, const char *part){

   create_view_data * data = (create_view_data*)item_data;

   return _entry_add(data,TITLE_ENTRY);

}

void
memo_view_cb(void *data, Evas_Object *obj, void *event_info){
   Evas_Object *scroller, *layout,*btn, *box, *label;
   Evas_Object *nf = data;
   Elm_Object_Item *nf_it;

   evas_object_del(obj);
   obj = NULL;


   scroller = elm_scroller_add(nf);

   /*Header and btns for naviframe*/
   nf_it = elm_naviframe_item_push(nf,"Schedule MEMO",NULL,NULL,scroller,NULL);

   btn = elm_button_add(nf);
   elm_object_style_set(btn, "naviframe/title_cancel");
   evas_object_smart_callback_add(btn, "clicked", btn_back_cb, nf);
   elm_object_item_part_content_set(nf_it, "title_left_btn", btn);

   btn = elm_button_add(nf);
   elm_object_style_set(btn, "naviframe/title_done");
   evas_object_smart_callback_add(btn, "clicked", btn_back_cb, nf);
   elm_object_item_part_content_set(nf_it, "title_right_btn", btn);


   box = elm_box_add(scroller);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(scroller,box);

//   /*Label*/
//   label = elm_label_add(box);
//   elm_object_text_set(label, "<align=center>Title</align>");
//   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, 0);
//   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
//   evas_object_show(label);
//   elm_box_pack_end(box, label);

   /*Bg-1*/
   Evas_Object *bg = elm_bg_add(box);
   elm_bg_color_set(bg,170,220,255);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(bg);
   elm_box_pack_end(box,bg);


   /*Entry-1*/
   Evas_Object *entry = elm_entry_add(box);
   elm_object_part_text_set(entry, "elm.guide", "Input Title");
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(entry);
   elm_box_pack_end(box, entry);

   /*Gen-List*/
   Evas_Object *genlist = elm_genlist_add(box);
   evas_object_size_hint_weight_set(genlist,EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist,EVAS_HINT_FILL,EVAS_HINT_FILL);
   evas_object_show(genlist);
   elm_box_pack_end(box, entry);

   /*create item class*/
   Elm_Genlist_Item_Class
   itc_title = {
         .item_style = "full",
         .func.content_get = _title_content_cb,
   };

   elm_genlist_item_append(genlist,&itc_title,data,NULL,ELM_GENLIST_ITEM_NONE,NULL,data);



}
