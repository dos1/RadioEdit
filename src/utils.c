/*! \file utils.c
 *  \brief Helper functions.
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

#include <allegro5/allegro_primitives.h>
#include "stdio.h"
#include "config.h"
#include "math.h"
#include "utils.h"

void DrawVerticalGradientRect(float x, float y, float w, float h, ALLEGRO_COLOR top, ALLEGRO_COLOR bottom) {
	ALLEGRO_VERTEX v[] = {
		{.x = x, .y = y, .z = 0, .color = top},
		{.x = x + w, .y = y, .z = 0, .color = top},
		{.x = x, .y = y + h, .z = 0, .color = bottom},
		{.x = x + w, .y = y + h, .z = 0, .color = bottom}};
	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void DrawHorizontalGradientRect(float x, float y, float w, float h, ALLEGRO_COLOR left, ALLEGRO_COLOR right) {
	ALLEGRO_VERTEX v[] = {
		{.x = x, .y = y, .z = 0, .color = left},
		{.x = x + w, .y = y, .z = 0, .color = right},
		{.x = x, .y = y + h, .z = 0, .color = left},
		{.x = x + w, .y = y + h, .z = 0, .color = right}};
	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void DrawTextWithShadow(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, char const *text) {
	al_draw_text(font, al_map_rgba(0,0,0,128), (int)x+1, (int)y+1, flags, text);
	al_draw_text(font, color, (int)x, (int)y, flags, text);
}

/*! \brief Scales bitmap using software linear filtering method to current target. */
void ScaleBitmap(ALLEGRO_BITMAP* source, int width, int height) {
	if ((al_get_bitmap_width(source)==width) && (al_get_bitmap_height(source)==height)) {
		al_draw_bitmap(source, 0, 0, 0);
		return;
	}
	int x, y;
	al_lock_bitmap(al_get_target_bitmap(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
	al_lock_bitmap(source, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

	/* linear filtering code written by SiegeLord */

	ALLEGRO_COLOR interpolate(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, float frac) {
		return al_map_rgba_f(c1.r + frac * (c2.r - c1.r),
												 c1.g + frac * (c2.g - c1.g),
												 c1.b + frac * (c2.b - c1.b),
												 c1.a + frac * (c2.a - c1.a));
	}

	for (y = 0; y < height; y++) {
		float pixy = ((float)y / height) * ((float)al_get_bitmap_height(source) - 1);
		float pixy_f = floor(pixy);
		for (x = 0; x < width; x++) {
			float pixx = ((float)x / width) * ((float)al_get_bitmap_width(source) - 1);
			float pixx_f = floor(pixx);

			ALLEGRO_COLOR a = al_get_pixel(source, pixx_f, pixy_f);
			ALLEGRO_COLOR b = al_get_pixel(source, pixx_f + 1, pixy_f);
			ALLEGRO_COLOR c = al_get_pixel(source, pixx_f, pixy_f + 1);
			ALLEGRO_COLOR d = al_get_pixel(source, pixx_f + 1, pixy_f + 1);

			ALLEGRO_COLOR ab = interpolate(a, b, pixx - pixx_f);
			ALLEGRO_COLOR cd = interpolate(c, d, pixx - pixx_f);
			ALLEGRO_COLOR result = interpolate(ab, cd, pixy - pixy_f);

			al_put_pixel(x, y, result);
		}
	}
	al_unlock_bitmap(al_get_target_bitmap());
	al_unlock_bitmap(source);
}

ALLEGRO_BITMAP* LoadScaledBitmap(struct Game *game, char* filename, int width, int height) {
	bool memoryscale = !atoi(GetConfigOptionDefault(game, "SuperDerpy", "GPU_scaling", "1"));
	ALLEGRO_BITMAP *source, *target = al_create_bitmap(width, height);
	al_set_target_bitmap(target);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	char* origfn = GetDataFilePath(game, filename);
	void GenerateBitmap() {
		if (memoryscale) al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

		source = al_load_bitmap( origfn );
		if (memoryscale) {
			al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
			ScaleBitmap(source, width, height);
		}
		else {
			al_draw_scaled_bitmap(source, 0, 0, al_get_bitmap_width(source), al_get_bitmap_height(source), 0, 0, width, height, 0);
		}
		/*al_save_bitmap(cachefn, target);
		PrintConsole(game, "Cache bitmap %s generated.", filename);*/
		al_destroy_bitmap(source);
	}

	/*source = al_load_bitmap( cachefn );
	if (source) {
		if ((al_get_bitmap_width(source)!=width) || (al_get_bitmap_height(source)!=height)) {
			al_destroy_bitmap(source);*/
	GenerateBitmap();
	free(origfn);
	return target;
	/*	}
		return source;
	} else GenerateBitmap();
	return target;*/
}

void FatalError(struct Game *game, bool fatal, char* format, ...) {
	char text[1024] = {};
	if (!game->_priv.console) {
		va_list vl;
		va_start(vl, format);
		vsnprintf(text, 1024, format, vl);
		va_end(vl);
		printf("%s\n", text);
	} else {
		PrintConsole(game, "Fatal Error, displaying Blue Screen of Derp...");
		va_list vl;
		va_start(vl, format);
		vsnprintf(text, 1024, format, vl);
		va_end(vl);
		PrintConsole(game, text);
	}

	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);
	al_use_transform(&trans);

	if (!game->_priv.font_bsod) {
		game->_priv.font_bsod = al_create_builtin_font();
	}

	al_set_target_backbuffer(game->display);
	al_clear_to_color(al_map_rgb(0,0,170));
	al_flip_display();
	al_rest(0.6);

	bool done = false;
	while (!done) {

		al_set_target_backbuffer(game->display);
		al_clear_to_color(al_map_rgb(0,0,170));

		char *header = "RADIO EDIT";

		al_draw_filled_rectangle(al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header)/2 - 4, (int)(al_get_display_height(game->display) * 0.32), 4 + al_get_display_width(game->display)/2 + al_get_text_width(game->_priv.font_bsod, header)/2, (int)(al_get_display_height(game->display) * 0.32) + al_get_font_line_height(game->_priv.font_bsod), al_map_rgb(170,170,170));

		al_draw_text(game->_priv.font_bsod, al_map_rgb(0, 0, 170), al_get_display_width(game->display)/2, (int)(al_get_display_height(game->display) * 0.32), ALLEGRO_ALIGN_CENTRE, header);

		char *header2 = "A fatal exception 0xD3RP has occured at 0028:M00F11NZ in GST SD(01) +";

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, (int)(al_get_display_height(game->display) * 0.32+2*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_CENTRE, header2);
		al_draw_textf(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header2)/2, (int)(al_get_display_height(game->display) * 0.32+3*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_LEFT, "%p and system just doesn't know what went wrong.", game);

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, (int)(al_get_display_height(game->display) * 0.32+5*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_CENTRE, text);

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header2)/2, (int)(al_get_display_height(game->display) * 0.32+7*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_LEFT, "* Press any key to terminate this error.");
		al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header2)/2, (int)(al_get_display_height(game->display) * 0.32+8*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_LEFT, "* Press any key to destroy all muffins in the world.");
		al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header2)/2, (int)(al_get_display_height(game->display) * 0.32+9*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_LEFT, "* Just kidding, please press any key anyway.");


		if (fatal) {
			al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header2)/2, (int)(al_get_display_height(game->display) * 0.32+11*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_LEFT, "This is fatal error. My bad.");

			al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, (int)(al_get_display_height(game->display) * 0.32+13*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_CENTRE, "Press any key to quit _");
		} else {
			al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2 - al_get_text_width(game->_priv.font_bsod, header2)/2, (int)(al_get_display_height(game->display) * 0.32+11*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_LEFT, "Anything I can do to help?");

			al_draw_text(game->_priv.font_bsod, al_map_rgb(255,255,255), al_get_display_width(game->display)/2, (int)(al_get_display_height(game->display) * 0.32+13*al_get_font_line_height(game->_priv.font_bsod)*1.25), ALLEGRO_ALIGN_CENTRE, "Press any key to continue _");
		}

		al_flip_display();

		ALLEGRO_KEYBOARD_STATE kb;
		al_get_keyboard_state(&kb);

		int i;
		for (i=0; i<ALLEGRO_KEY_MAX; i++) {
			if (al_key_down(&kb, i)) {
				done = true;
				break;
			}
		}
	}
	al_use_transform(&game->projection);
}

char* GetDataFilePath(struct Game *game, char* filename) {

	//TODO: support for current game

	char *result = 0;

	if (al_filename_exists(filename)) {
		return strdup(filename);
	}

	char origfn[255] = "data/";
	strcat(origfn, filename);

	if (al_filename_exists(origfn)) {
		return strdup(origfn);
	}

	void TestPath(char* subpath) {
		ALLEGRO_PATH *tail = al_create_path(filename);
		ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		ALLEGRO_PATH *data = al_create_path(subpath);
		al_join_paths(path, data);
		al_join_paths(path, tail);
		//printf("Testing for %s\n", al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		if (al_filename_exists(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP))) {
			result = strdup(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
		}
		al_destroy_path(tail);
		al_destroy_path(data);
		al_destroy_path(path);
	}
	TestPath("../share/radioedit/data/");
	TestPath("../data/");
	TestPath("../Resources/data/");
	TestPath("data/");

	if (!result) {
		FatalError(game, true, "Could not find data file: %s!", filename);
		exit(1);
	}
	return result;
}


void PrintConsole(struct Game *game, char* format, ...) {
	va_list vl;
	va_start(vl, format);
	char text[1024] = {};
	vsnprintf(text, 1024, format, vl);
	va_end(vl);
	if (game->config.debug) { printf("%s\n", text); fflush(stdout); }
	if (!game->_priv.console) return;
	ALLEGRO_BITMAP *con = al_create_bitmap(al_get_bitmap_width(game->_priv.console), al_get_bitmap_height(game->_priv.console));
	al_set_target_bitmap(con);
	al_clear_to_color(al_map_rgba(0,0,0,80));
	al_draw_bitmap_region(game->_priv.console, 0, (int)(al_get_bitmap_height(game->_priv.console)*0.2), al_get_bitmap_width(game->_priv.console), (int)(al_get_bitmap_height(game->_priv.console)*0.8), 0, 0, 0);
	al_draw_text(game->_priv.font_console, al_map_rgb(255,255,255), (int)(game->viewport.width*0.005), (int)(al_get_bitmap_height(game->_priv.console)*0.81), ALLEGRO_ALIGN_LEFT, text);
	al_set_target_bitmap(game->_priv.console);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap(con, 0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(game->display));
	al_destroy_bitmap(con);
}


void SelectSpritesheet(struct Game *game, struct Character *character, char* name) {
	struct Spritesheet *tmp = character->spritesheets;
	PrintConsole(game, "Selecting spritesheet for %s: %s", character->name, name);
	if (!tmp) {
		PrintConsole(game, "ERROR: No spritesheets registered for %s!", character->name);
		return;
	}
	while (tmp) {
		if (!strcmp(tmp->name, name)) {
			character->spritesheet = tmp;
			if (tmp->successor) {
				character->successor = strdup(tmp->successor);
			} else {
				character->successor = NULL;
			}
			character->pos = 0;
			if (character->bitmap) al_destroy_bitmap(character->bitmap);
			character->bitmap = al_create_bitmap(tmp->width / tmp->cols, tmp->height / tmp->rows);
			PrintConsole(game, "SUCCESS: Spritesheet for %s activated: %s (%dx%d)", character->name, name, al_get_bitmap_width(character->bitmap), al_get_bitmap_height(character->bitmap));
			return;
		}
		tmp = tmp->next;
	}
	PrintConsole(game, "ERROR: No spritesheets registered for %s with given name: %s", character->name, name);
	return;
}

void ChangeSpritesheet(struct Game *game, struct Character *character, char* name) {
	if (character->successor) free(character->successor);
	character->successor = strdup(name);
}

void LoadSpritesheets(struct Game *game, struct Character *character) {
	PrintConsole(game, "Loading spritesheets for character %s...", character->name);
	struct Spritesheet *tmp = character->spritesheets;
	while (tmp) {
		if (!tmp->bitmap) {
			char filename[255] = { };
			snprintf(filename, 255, "sprites/%s/%s.png", character->name, tmp->name);
			tmp->bitmap = al_load_bitmap(GetDataFilePath(game, filename));
			tmp->width = al_get_bitmap_width(tmp->bitmap);
			tmp->height = al_get_bitmap_height(tmp->bitmap);
		}
		tmp = tmp->next;
	}
}

void UnloadSpritesheets(struct Game *game, struct Character *character) {
	PrintConsole(game, "Unloading spritesheets for character %s...", character->name);
	struct Spritesheet *tmp = character->spritesheets;
	while (tmp) {
		if (tmp->bitmap) al_destroy_bitmap(tmp->bitmap);
		tmp->bitmap = NULL;
		tmp = tmp->next;
	}
}

void RegisterSpritesheet(struct Game *game, struct Character *character, char* name) {
	struct Spritesheet *s = character->spritesheets;
	while (s) {
		if (!strcmp(s->name, name)) {
			//PrintConsole(game, "%s spritesheet %s already registered!", character->name, name);
			return;
		}
		s = s->next;
	}
	PrintConsole(game, "Registering %s spritesheet: %s", character->name, name);
	char filename[255] = { };
	snprintf(filename, 255, "sprites/%s/%s.ini", character->name, name);
	ALLEGRO_CONFIG *config = al_load_config_file(GetDataFilePath(game, filename));
	s = malloc(sizeof(struct Spritesheet));
	s->name = strdup(name);
	s->bitmap = NULL;
	s->cols = atoi(al_get_config_value(config, "", "cols"));
	s->rows = atoi(al_get_config_value(config, "", "rows"));
	s->blanks = atoi(al_get_config_value(config, "", "blanks"));
	s->delay = atof(al_get_config_value(config, "", "delay"));
	s->successor=NULL;
	const char* successor = al_get_config_value(config, "", "successor");
	if (successor) {
		s->successor = malloc(255*sizeof(char));
		strncpy(s->successor, successor, 255);
	}
	s->next = character->spritesheets;
	character->spritesheets = s;
	al_destroy_config(config);
}

struct Character* CreateCharacter(struct Game *game, char* name) {
	PrintConsole(game, "Creating character %s...", name);
	struct Character *character = malloc(sizeof(struct Character));
	character->name = strdup(name);
	character->angle = 0;
	character->bitmap = NULL;
	character->data = NULL;
	character->pos = 0;
	character->pos_tmp = 0;
	character->x = -1;
	character->y = -1;
	character->spritesheets = NULL;
	character->spritesheet = NULL;
	character->successor = NULL;
	return character;
}

void DestroyCharacter(struct Game *game, struct Character *character) {
	PrintConsole(game, "Destroying character %s...", character->name);
	UnloadSpritesheets(game, character);
	struct Spritesheet *tmp, *s = character->spritesheets;
	tmp = s;
	while (s) {
		tmp = s;
		s = s->next;
		free(tmp);
	}

	if (character->bitmap) al_destroy_bitmap(character->bitmap);
	free(character->name);
	free(character);
}

void AnimateCharacter(struct Game *game, struct Character *character, float speed_modifier) {
	if (speed_modifier) {
		character->pos_tmp++;
		if (character->pos_tmp >= character->spritesheet->delay / speed_modifier) {
			character->pos_tmp = 0;
			character->pos++;
		}
		if (character->pos>=character->spritesheet->cols*character->spritesheet->rows-character->spritesheet->blanks) {
			character->pos=0;
			if (character->successor) {
				SelectSpritesheet(game, character, character->successor);
			}
		}
	}
}

void MoveCharacter(struct Game *game, struct Character *character, int x, int y, float angle) {
	character->x += x;
	character->y += y;
	character->angle += angle;
}

void SetCharacterPosition(struct Game *game, struct Character *character, int x, int y, float angle) {
	character->x = x;
	character->y = y;
	character->angle = angle;
}


void DrawCharacter(struct Game *game, struct Character *character, ALLEGRO_COLOR tilt, int flags) {
	al_set_target_bitmap(character->bitmap);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(character->spritesheet->bitmap, al_get_bitmap_width(character->bitmap)*(character->pos%character->spritesheet->cols),al_get_bitmap_height(character->bitmap)*(character->pos/character->spritesheet->cols),al_get_bitmap_width(character->bitmap), al_get_bitmap_height(character->bitmap),0,0,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_draw_tinted_rotated_bitmap(character->bitmap, tilt, al_get_bitmap_width(character->bitmap), al_get_bitmap_height(character->bitmap)/2, character->x + al_get_bitmap_width(character->bitmap), character->y + al_get_bitmap_height(character->bitmap)/2, character->angle, flags);

}
