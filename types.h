/* * Copyright (c) 2013 Bastien Dejean
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TYPES_H
#define _TYPES_H

#include <xcb/xcb_keysyms.h>
#include <stdbool.h>
#include "helpers.h"

#define KEYSYMS_PER_KEYCODE  4
#define MOD_STATE_FIELD      255
#define ESCAPE_KEYSYM        0xff1b

typedef struct chord_t chord_t;
struct chord_t {
    char repr[MAXLEN];
    xcb_keysym_t keysym;
    xcb_button_t button;
    uint16_t modfield;
    uint8_t event_type;
    bool replay_event;
    bool lock_chain;
    chord_t *next;
    chord_t *more;
};

typedef struct {
    chord_t *head;
    chord_t *tail;
    chord_t *state;
} chain_t;

typedef struct {
    int period;
    int delay;
} cycle_t;

typedef struct hotkey_t hotkey_t;
struct hotkey_t {
    chain_t *chain;
    char command[2 * MAXLEN];
    cycle_t *cycle;
    hotkey_t *next;
    hotkey_t *prev;
};

typedef struct {
    char *name;
    xcb_keysym_t keysym;
} keysym_dict_t;

xcb_keysym_t Alt_L, Alt_R, Super_L, Super_R, Hyper_L, Hyper_R,
             Meta_L, Meta_R, Mode_switch, Num_Lock, Scroll_Lock;

chord_t *make_chord(xcb_keysym_t, xcb_button_t, uint16_t, uint8_t, bool, bool);
chain_t *make_chain(void);
cycle_t *make_cycle(int, int);
void add_chord(chain_t *, chord_t *);
bool match_chord(chord_t *, uint8_t, xcb_keysym_t, xcb_button_t, uint16_t);
hotkey_t *find_hotkey(xcb_keysym_t, xcb_button_t, uint16_t, uint8_t, bool *);
hotkey_t *make_hotkey(chain_t *, char *);
void add_hotkey(hotkey_t *);
void abort_chain(void);
void destroy_chain(chain_t *);
void destroy_chord(chord_t *);

#endif
