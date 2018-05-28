#pragma once
#include <pebble.h>  
#include "effects.h"
  
// Taken from https://github.com/ygalanter/pebble-effect-layer/blob/master/src/c/effect_layer.h 
//  Only included for rotate effect, which isn't included in this version (used in phoenix 90 and 270). *********


//number of supported effects on a single effect_layer (must be <= 255)
#define MAX_EFFECTS 4
  
// structure of effect layer
typedef struct {
  Layer*      layer;
  effect_cb*  effects[MAX_EFFECTS];
  void*       params[MAX_EFFECTS];
  uint8_t     next_effect;
} EffectLayer;


//creates effect layer
EffectLayer* effect_layer_create(GRect frame);

//destroys effect layer
void effect_layer_destroy(EffectLayer *effect_layer);

//sets effect for the layer
void effect_layer_add_effect(EffectLayer *effect_layer, effect_cb* effect, void* param);

//gets layer
Layer* effect_layer_get_layer(EffectLayer *effect_layer);