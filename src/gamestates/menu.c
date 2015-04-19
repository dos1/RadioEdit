/*! \file menu.c
 *  \brief Main Menu view.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro_ttf.h>
#include "../config.h"
#include "../utils.h"
#include "../timeline.h"
#include "menu.h"

int Gamestate_ProgressCount = 18;

void DrawMenuState(struct Game *game, struct MenuResources *data) {
	ALLEGRO_FONT *font = data->font;
	char* text = malloc(255*sizeof(char));
	struct ALLEGRO_COLOR color;
	switch (data->menustate) {
		case MENUSTATE_MAIN:
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Start game");
			DrawTextWithShadow(font, data->selected==1 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Options");
			DrawTextWithShadow(font, data->selected==2 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.7, ALLEGRO_ALIGN_CENTRE, "About");
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Exit");
			break;
		case MENUSTATE_OPTIONS:
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Video settings");
			DrawTextWithShadow(font, data->selected==1 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, "Audio settings");
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_AUDIO:
			if (game->config.music) snprintf(text, 255, "Music volume: %d0%%", game->config.music);
			else sprintf(text, "Music disabled");
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, text);
			if (game->config.fx) snprintf(text, 255, "Effects volume: %d0%%", game->config.fx);
			else sprintf(text, "Effects disabled");
			DrawTextWithShadow(font, data->selected==1 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, text);
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		case MENUSTATE_VIDEO:
			if (data->options.fullscreen) {
				sprintf(text, "Fullscreen: yes");
				color = al_map_rgba(0,0,0,128);
			}
			else {
				sprintf(text, "Fullscreen: no");
				color = data->selected==1 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255);
			}
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, text);
			sprintf(text, "Resolution: %dx", data->options.resolution);
			DrawTextWithShadow(font, color, game->viewport.width*0.5, game->viewport.height*0.6, ALLEGRO_ALIGN_CENTRE, text);
			DrawTextWithShadow(font, data->selected==3 ? al_map_rgb(255,255,180) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.8, ALLEGRO_ALIGN_CENTRE, "Back");
			break;
		default:
			data->selected=0;
			DrawTextWithShadow(font, data->selected==0 ? al_map_rgb(255,255,200) : al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5, ALLEGRO_ALIGN_CENTRE, "Not implemented yet");
			break;
	}
	free(text);
}

void Gamestate_Draw(struct Game *game, struct MenuResources* data) {

	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_clear_to_color(al_map_rgb(3, 213, 255));

	al_draw_bitmap(data->bg,0, 0,0);

	al_draw_bitmap(data->cloud,game->viewport.width*data->cloud_position/100, 10 ,0);

	al_draw_bitmap(data->forest,0, 0,0);
	al_draw_bitmap(data->grass,0, 0,0);

	DrawCharacter(game, data->cow, al_map_rgb(255,255,255), 0);

	al_draw_bitmap(data->speaker,104, 19,0);

	al_draw_bitmap(data->stage,0, 0,0);

	al_draw_bitmap(data->lines, 100, 136,0);

	al_draw_bitmap(data->cable,0,151,0);

	DrawCharacter(game, data->ego, al_map_rgb(255,255,255), 0);

	DrawTextWithShadow(data->font_title, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.15, ALLEGRO_ALIGN_CENTRE, "Radio Edit");

	DrawMenuState(game, data);
}

void Gamestate_Logic(struct Game *game, struct MenuResources* data) {
	data->cloud_position-=0.1;
	if (data->cloud_position<-40) { data->cloud_position=100; PrintConsole(game, "cloud_position"); }
	AnimateCharacter(game, data->ego, 1);
	AnimateCharacter(game, data->cow, 1);
	TM_Process(data->timeline);
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {

	struct MenuResources *data = malloc(sizeof(struct MenuResources));

	data->timeline = TM_Init(game, "main");

	data->options.fullscreen = game->config.fullscreen;
	data->options.fps = game->config.fps;
	data->options.width = game->config.width;
	data->options.height = game->config.height;
	data->options.resolution = game->config.width / 320;
	if (game->config.height / 180 < data->options.resolution) data->options.resolution = game->config.height / 180;

	data->bg = al_load_bitmap( GetDataFilePath(game, "bg.png") );
	(*progress)(game);

	data->forest = al_load_bitmap( GetDataFilePath(game, "forest.png") );
	(*progress)(game);

	data->grass = al_load_bitmap( GetDataFilePath(game, "grass.png") );
	(*progress)(game);

	data->speaker = al_load_bitmap( GetDataFilePath(game, "speaker.png") );
	(*progress)(game);

	data->stage = al_load_bitmap( GetDataFilePath(game, "stage.png") );
	(*progress)(game);

	data->cloud = al_load_bitmap( GetDataFilePath(game, "cloud.png") );
	(*progress)(game);

	data->lines = al_load_bitmap( GetDataFilePath(game, "lines.png") );
	(*progress)(game);

	data->cable = al_load_bitmap( GetDataFilePath(game, "cable.png") );
	(*progress)(game);

	data->sample = al_load_sample( GetDataFilePath(game, "menu.flac") );
	(*progress)(game);

	data->click_sample = al_load_sample( GetDataFilePath(game, "click.flac") );
	(*progress)(game);

	data->music = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->music, game->audio.music);
	al_set_sample_instance_playmode(data->music, ALLEGRO_PLAYMODE_LOOP);
	(*progress)(game);

	data->click = al_create_sample_instance(data->click_sample);
	al_attach_sample_instance_to_mixer(data->click, game->audio.fx);
	al_set_sample_instance_playmode(data->click, ALLEGRO_PLAYMODE_ONCE);
	(*progress)(game);

	data->font_title = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),game->viewport.height*0.16,0 );
	(*progress)(game);
	data->font_subtitle = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),game->viewport.height*0.08,0 );
	(*progress)(game);
	data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),game->viewport.height*0.05,0 );
	(*progress)(game);
	data->font_selected = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),game->viewport.height*0.1,0 );
	(*progress)(game);


	if (!data->click_sample){
		fprintf(stderr, "Audio clip sample#3 not loaded!\n" );
		exit(-1);
	}

	data->ego = CreateCharacter(game, "ego");
	RegisterSpritesheet(game, data->ego, "stand");
	RegisterSpritesheet(game, data->ego, "fix");
	RegisterSpritesheet(game, data->ego, "fix2");
	RegisterSpritesheet(game, data->ego, "fix3");
	LoadSpritesheets(game, data->ego);

	(*progress)(game);

	data->cow = CreateCharacter(game, "cow");
	RegisterSpritesheet(game, data->cow, "stand");
	RegisterSpritesheet(game, data->cow, "chew");
	RegisterSpritesheet(game, data->cow, "look");
	LoadSpritesheets(game, data->cow);

	(*progress)(game);

	al_set_target_backbuffer(game->display);
	return data;
}

void Gamestate_Stop(struct Game *game, struct MenuResources* data) {
	al_stop_sample_instance(data->music);
}

void ChangeMenuState(struct Game *game, struct MenuResources* data, enum menustate_enum state) {
	data->menustate=state;
	data->selected=0;
	PrintConsole(game, "menu state changed %d", state);
}

void Gamestate_Unload(struct Game *game, struct MenuResources* data) {
	al_destroy_bitmap(data->bg);
	al_destroy_bitmap(data->cloud);
	al_destroy_bitmap(data->grass);
	al_destroy_bitmap(data->forest);
	al_destroy_bitmap(data->stage);
	al_destroy_bitmap(data->speaker);
	al_destroy_bitmap(data->lines);
	al_destroy_bitmap(data->cable);
	al_destroy_font(data->font_title);
	al_destroy_font(data->font_subtitle);
	al_destroy_font(data->font);
	al_destroy_font(data->font_selected);
	al_destroy_sample_instance(data->music);
	al_destroy_sample_instance(data->click);
	al_destroy_sample(data->sample);
	al_destroy_sample(data->click_sample);
	DestroyCharacter(game, data->ego);
	DestroyCharacter(game, data->cow);
	TM_Destroy(data->timeline);
}

// TODO: refactor to single Enqueue_Anim
bool Anim_CowLook(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct MenuResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		ChangeSpritesheet(game, data->cow, "look");
		TM_AddQueuedBackgroundAction(data->timeline, &Anim_CowLook, TM_AddToArgs(NULL, 1, data), 54*1000, "cow_look");
	}
	return true;
}

bool Anim_FixGuitar(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct MenuResources *data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		ChangeSpritesheet(game, data->ego, "fix");
		TM_AddQueuedBackgroundAction(data->timeline, &Anim_FixGuitar, TM_AddToArgs(NULL, 1, data), 30*1000, "fix_guitar");
	}
	return true;
}


void Gamestate_Start(struct Game *game, struct MenuResources* data) {
	data->cloud_position = 100;
	SetCharacterPosition(game, data->ego, 22, 107, 0);
	SetCharacterPosition(game, data->cow, 35, 88, 0);
	SelectSpritesheet(game, data->ego, "stand");
	SelectSpritesheet(game, data->cow, "chew");
	ChangeMenuState(game,data,MENUSTATE_MAIN);
	TM_AddQueuedBackgroundAction(data->timeline, &Anim_FixGuitar, TM_AddToArgs(NULL, 1, data), 15*1000, "fix_guitar");
	TM_AddQueuedBackgroundAction(data->timeline, &Anim_CowLook, TM_AddToArgs(NULL, 1, data), 5*1000, "cow_look");
	al_play_sample_instance(data->music);
}

void Gamestate_ProcessEvent(struct Game *game, struct MenuResources* data, ALLEGRO_EVENT *ev) {
	TM_HandleEvent(data->timeline, ev);

	if (ev->type != ALLEGRO_EVENT_KEY_DOWN) return;

	if (ev->keyboard.keycode==ALLEGRO_KEY_UP) {
		data->selected--;
		if ((data->selected == 2) && ((data->menustate==MENUSTATE_VIDEO) || (data->menustate==MENUSTATE_OPTIONS) || (data->menustate==MENUSTATE_AUDIO))) {
			data->selected --;
		}
		if ((data->menustate==MENUSTATE_VIDEO) && (data->selected==1) && (data->options.fullscreen)) data->selected--;
		al_play_sample_instance(data->click);
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_DOWN) {
		data->selected++;
		if ((data->menustate==MENUSTATE_VIDEO) && (data->selected==1) && (data->options.fullscreen)) data->selected++;
		if ((data->selected == 2) && ((data->menustate==MENUSTATE_VIDEO) || (data->menustate==MENUSTATE_OPTIONS) || (data->menustate==MENUSTATE_AUDIO))) {
			data->selected ++;
		}


		al_play_sample_instance(data->click);
	}

	if (ev->keyboard.keycode==ALLEGRO_KEY_ENTER) {
		char *text;
		al_play_sample_instance(data->click);
		switch (data->menustate) {
			case MENUSTATE_MAIN:
				switch (data->selected) {
					case 0:
						LoadGamestate(game, "disclaimer");
						LoadGamestate(game, "intro");
						LoadGamestate(game, "map");
						StopGamestate(game, "menu");
						StartGamestate(game, "disclaimer");
						break;
					case 1:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					case 2:
						StopGamestate(game, "menu");
						LoadGamestate(game, "about");
						StartGamestate(game, "about");
						break;
					case 3:
						UnloadGamestate(game, "menu");
						break;
				}
				break;
			case MENUSTATE_AUDIO:
				text = malloc(255*sizeof(char));
				switch (data->selected) {
					case 0:
						game->config.music--;
						if (game->config.music<0) game->config.music=10;
						snprintf(text, 255, "%d", game->config.music);
						SetConfigOption(game, "SuperDerpy", "music", text);
						al_set_mixer_gain(game->audio.music, game->config.music/10.0);
						break;
					case 1:
						game->config.fx--;
						if (game->config.fx<0) game->config.fx=10;
						snprintf(text, 255, "%d", game->config.fx);
						SetConfigOption(game, "SuperDerpy", "fx", text);
						al_set_mixer_gain(game->audio.fx, game->config.fx/10.0);
						break;
					case 2:
						game->config.voice--;
						if (game->config.voice<0) game->config.voice=10;
						snprintf(text, 255, "%d", game->config.voice);
						SetConfigOption(game, "SuperDerpy", "voice", text);
						al_set_mixer_gain(game->audio.voice, game->config.voice/10.0);
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
				}
				free(text);
				break;
			case MENUSTATE_OPTIONS:
				switch (data->selected) {
					case 0:
						ChangeMenuState(game,data,MENUSTATE_VIDEO);
						break;
					case 1:
						ChangeMenuState(game,data,MENUSTATE_AUDIO);
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_MAIN);
						break;
					default:
						break;
				}
				break;
			case MENUSTATE_VIDEO:
				switch (data->selected) {
					case 0:
						data->options.fullscreen = !data->options.fullscreen;
						if (data->options.fullscreen)
							SetConfigOption(game, "SuperDerpy", "fullscreen", "1");
						else
							SetConfigOption(game, "SuperDerpy", "fullscreen", "0");
						al_set_display_flag(game->display, ALLEGRO_FULLSCREEN_WINDOW, data->options.fullscreen);
						SetupViewport(game);
						Console_Unload(game);
						Console_Load(game);
						PrintConsole(game, "Fullscreen toggled");
						break;
					case 1:
						data->options.resolution++;

						int max = 0, i = 0;

						for (i=0; i < al_get_num_video_adapters(); i++) {
							ALLEGRO_MONITOR_INFO aminfo;
							al_get_monitor_info(i , &aminfo);
							int desktop_width = aminfo.x2 - aminfo.x1 + 1;
							int desktop_height = aminfo.y2 - aminfo.y1 + 1;
							int localmax = desktop_width / 320;
							if (desktop_height / 180 < localmax) localmax = desktop_height / 180;
							if (localmax > max) max = localmax;
						}


						if (data->options.resolution > max) data->options.resolution = 1;
						text = malloc(255*sizeof(char));
						snprintf(text, 255, "%d", data->options.resolution * 320);
						SetConfigOption(game, "SuperDerpy", "width", text);
						snprintf(text, 255, "%d", data->options.resolution * 180);
						SetConfigOption(game, "SuperDerpy", "height", text);
						free(text);
						al_resize_display(game->display, data->options.resolution * 320, data->options.resolution * 180);

						if ((al_get_display_width(game->display) < (data->options.resolution * 320)) || (al_get_display_height(game->display) < (data->options.resolution * 180))) {
							SetConfigOption(game, "SuperDerpy", "width", "320");
							SetConfigOption(game, "SuperDerpy", "height", "180");
							data->options.resolution = 1;
							al_resize_display(game->display, 320, 180);
						}

						SetupViewport(game);
						Console_Unload(game);
						Console_Load(game);
						PrintConsole(game, "Resolution changed");
						break;
					case 3:
						ChangeMenuState(game,data,MENUSTATE_OPTIONS);
						break;
					default:
						break;
				}
				break;
			default:
				UnloadGamestate(game, "menu");
				return;
				break;
		}
	} else if (ev->keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
		switch (data->menustate) {
			case MENUSTATE_OPTIONS:
				ChangeMenuState(game,data,MENUSTATE_MAIN);
				break;
			case MENUSTATE_VIDEO:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			case MENUSTATE_AUDIO:
				ChangeMenuState(game,data,MENUSTATE_OPTIONS);
				break;
			default:
				UnloadGamestate(game, "menu");
				return;
		}
	}

	if (data->selected==-1) data->selected=3;
	if (data->selected==4) data->selected=0;
	return;
}


void Gamestate_Pause(struct Game *game, struct MenuResources* data) {}
void Gamestate_Resume(struct Game *game, struct MenuResources* data) {}
void Gamestate_Reload(struct Game *game, struct MenuResources* data) {}
