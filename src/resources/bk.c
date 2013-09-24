#include "resources/bk.h"
#include <shadowdive/shadowdive.h>

void bk_create(bk *b, void *src) {
	sd_bk_file *sdbk = (sd_bk_file*)src;

	// Copy VGA image
	sd_vga_image *raw_image = sd_vga_image_create(sdbk->background->w, sdbk->background->h);
	memcpy(raw_image->data, sdbk->background->data, raw_image->len);
	sprite_create_custom(&b->background, vec2i_create(0,0), raw_image);

	// Copy sound translation table
	memcpy(b->sound_translation_table, sdbk->soundtable, 30);

	// Copy palettes
	vector_create(&b->palettes, sizeof(palette));
	for(int i = 0; i < sdbk->num_palettes; i++) {
		vector_append(&b->palettes, (palette*)sdbk->palettes[i]);
	}

	// Copy info structs
	hashmap_create(&b->infos, 7);
	bk_info tmp_bk_info;
	for(int i = 0; i < 50; i++) {
		if(sdbk->anims[i] != NULL) {
			bk_info_create(&tmp_bk_info, (void*)sdbk->anims[i]);
			hashmap_iput(&b->infos, i, &tmp_bk_info, sizeof(bk_info));
		}
	}
}

bk_info* bk_get_info(bk *b, int id) {
	bk_info *val;
	unsigned int tmp;
	if(hashmap_iget(&b->infos, id, (void**)&val, &tmp) == 1) {
		return NULL;
	}
	return val;
}

void bk_free(bk *b) {
	sprite_free(&b->background);
	vector_free(&b->palettes);

	// Free info structs
	iterator it;
	hashmap_iter_begin(&b->infos, &it);
	hashmap_pair *pair = NULL;
	while((pair = iter_next(&it)) != NULL) {
		bk_info_free((bk_info*)pair->val);
	}
	hashmap_free(&b->infos);
}
