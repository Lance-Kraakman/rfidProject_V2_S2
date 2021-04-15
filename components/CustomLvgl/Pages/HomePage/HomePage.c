/*
 * HomePage.c
 *
 *  Created on: 12/04/2021
 *      Author: lance
 */
#include "HomePage.h"
#include "../fonts/lemon_30.c"

static lv_obj_t *homeScreenLabel;
static lv_obj_t * mainScreen;
static lv_style_t st;

extern lv_font_t lemon_30;

void lv_ex_get_started_1(void)
{

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    //lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                  /*Set the labels text*/
}

void lv_welcome_page(char *displayTitle) {

//	lv_style_t st_bg;
//    lv_style_init(&st_bg);
//    lv_style_set_bg_color(&st_bg, LV_STATE_DEFAULT, LV_COLOR_BLACK);
//    lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &st_bg); // Apply a style to watchamling

	mainScreen = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(mainScreen, 310, 230);
	lv_obj_align(mainScreen, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_style_init(&st);
    //st.map->text.font = &code_new_roman_2;
    lv_style_set_text_font(&st, LV_STATE_DEFAULT, &lv_font_montserrat_36);
    lv_style_set_text_color(&st, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&st, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_add_style(mainScreen, LV_BTN_PART_MAIN, &st); // Apply a style to watchamling

    homeScreenLabel = lv_label_create(mainScreen, NULL);          /*Add a label to the button*/
	lv_label_set_text(homeScreenLabel, displayTitle);

	lv_label_set_align(homeScreenLabel, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(homeScreenLabel, NULL, LV_ALIGN_CENTER, 0, 0);

}

void updateHomeScreenText(char *text) {
	lv_label_set_text(homeScreenLabel, text);
    lv_obj_align(homeScreenLabel, mainScreen, LV_ALIGN_CENTER, 0, 0); // Re-Align After you update the text - Cant do this because it doesnt remember what the parent was!

}


