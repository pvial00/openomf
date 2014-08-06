#include <stdlib.h>
#include <string.h>

#include "shadowdive/internal/helpers.h"
#include "shadowdive/animation.h"
#include "shadowdive/bkanim.h"
#include "shadowdive/error.h"

int sd_bk_anim_create(sd_bk_anim *bka) {
    if(bka == NULL) {
        return SD_INVALID_INPUT;
    }
    // clear everything
    memset(bka, 0, sizeof(sd_bk_anim));
    return SD_SUCCESS;
}

int sd_bk_anim_copy(sd_bk_anim *dst, const sd_bk_anim *src) {
    int ret;
    if(dst == NULL || src == NULL) {
        return SD_INVALID_INPUT;
    }

    // Clear destination
    memset(dst, 0, sizeof(sd_bk_anim));

    // Basic stuff
    dst->null = src->null;
    dst->chain_hit = src->chain_hit;
    dst->chain_no_hit = src->chain_no_hit;
    dst->load_on_start = src->load_on_start;
    dst->probability = src->probability;
    dst->hazard_damage = src->hazard_damage;

    // Footer string
    strcpy(dst->footer_string, src->footer_string);

    // Copy animation (if exists)
    if(src->animation != NULL) {
        if((dst->animation = malloc(sizeof(sd_animation))) == NULL) {
            return SD_OUT_OF_MEMORY;
        }
        if((ret = sd_animation_copy(dst->animation, src->animation)) != SD_SUCCESS) {
            return ret;
        }
    }

    return SD_SUCCESS;
}

void sd_bk_anim_free(sd_bk_anim *bka) {
    if(bka->animation != NULL) {
        sd_animation_free(bka->animation);
        free(bka->animation);
    }
}

int sd_bk_anim_load(sd_reader *r, sd_bk_anim *bka) {
    int ret;

    // BK Specific animation header
    bka->null = sd_read_ubyte(r);
    bka->chain_hit = sd_read_ubyte(r);
    bka->chain_no_hit = sd_read_ubyte(r);
    bka->load_on_start = sd_read_ubyte(r);
    bka->probability = sd_read_uword(r);
    bka->hazard_damage = sd_read_ubyte(r);
    sd_read_str(r, bka->footer_string);

    // Initialize animation
    if((bka->animation = malloc(sizeof(sd_animation))) == NULL) {
        return SD_OUT_OF_MEMORY;
    }
    if((ret = sd_animation_create(bka->animation)) != SD_SUCCESS) {
        return ret;
    }
    if((ret = sd_animation_load(r, bka->animation)) != SD_SUCCESS) {
        return ret;
    }

    // Return success
    return SD_SUCCESS;
}

void sd_bk_anim_save(sd_writer *w, const sd_bk_anim *bka) {
    // Write BK specific header
    sd_write_ubyte(w, bka->null);
    sd_write_ubyte(w, bka->chain_hit);
    sd_write_ubyte(w, bka->chain_no_hit);
    sd_write_ubyte(w, bka->load_on_start);
    sd_write_uword(w, bka->probability);
    sd_write_ubyte(w, bka->hazard_damage);
    sd_write_str(w, bka->footer_string);

    // Write animation
    sd_animation_save(w, bka->animation);
}

int sd_bk_set_anim_string(sd_bk_anim *bka, const char *data) {
    if(strlen(data) >= SD_BK_FOOTER_STRING_MAX-1) {
        return SD_INVALID_INPUT;
    }
    strcpy(bka->footer_string, data);
    return SD_SUCCESS;
}
