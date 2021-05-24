#include <pebble.h>
#include "main.h"
#include "weekday.h"
#include "effect_layer.h"
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/fpath.h>
#include <pebble-fctx/ffont.h>

#define ROUND_VERTICAL_PADDING 15

//Static and initial vars
static GFont
  FontDate,
  FontWeather,
  FontSteps,
  FontIcon,
  FontIcon2,
  FontCity,
  FontWeatherIcons,
  FontWind;

char
  tempstring[44],
  condstring[44],
  citistring[44],
  windstring[44],
  windavestring[44],
  sunsetstring[44],
  sunrisestring[44],
  sunsetstring12[44],
  sunrisestring12[44],
  iconnowstring[8],
  iconforestring[8],
  windiconnowstring[8],
  windiconavestring[8],
  templowstring[44],
  temphistring[44];

FFont* time_font;

static Window * s_window;

//EffectLayer* effect_layer;
//EffectLayer* effect_layer_2; //rotation effect layer, not used in this version

//GColor color_loser;
//GColor color_winner;

static Layer * s_canvas_to_be_rotated;
static Layer * s_canvas;
static Layer * s_canvas_sunset_icon;
static Layer * s_canvas_bt_icon;

Layer * hour_area_layer;
Layer * minute_area_layer;
static TextLayer *s_step_layer;

static int s_hours, s_minutes, s_weekday, s_day, s_loop, s_countdown;

//link weather icons font symbols to weather conditions outputs from api, many thanks to relynr's Weatherstep watchface, at https://github.com/redlynr/Weatherstep
static char* weather_conditions[] = {
    "\U0000F07B", // 'unknown': 0,
    "\U0000F00D", // 'clear': 1,
    "\U0000F00D", // 'sunny': 2,
    "\U0000F002", // 'partlycloudy': 3,
    "\U0000F041", // 'mostlycloudy': 4,
    "\U0000F00C", // 'mostlysunny': 5,
    "\U0000F002", // 'partlysunny': 6,
    "\U0000F013", // 'cloudy': 7,
    "\U0000F019", // 'rain': 8,
    "\U0000F01B", // 'snow': 9,
    "\U0000F01D", // 'tstorms': 10,
    "\U0000F0b5", // 'sleat': 11,
    "\U0000F00A", // 'flurries': 12,
    "\U0000F0b6", // 'hazy': 13,
    "\U0000F01D", // 'chancetstorms': 14,
    "\U0000F01B", // 'chancesnow': 15,
    "\U0000F0b5", // 'chancesleat': 16,
    "\U0000F008", // 'chancerain': 17,
    "\U0000F01B", // 'chanceflurries': 18,
    "\U0000F07B", // 'nt_unknown': 19,
    "\U0000F02E", // 'nt_clear': 20,
    "\U0000F02E", // 'nt_sunny': 21,
    "\U0000F083", // 'nt_partlycloudy': 22,
    "\U0000F086", // 'nt_mostlycloudy': 23,
    "\U0000F081", // 'nt_mostlysunny': 24,
    "\U0000F086", // 'nt_partlysunny': 25,
    "\U0000F013", // 'nt_cloudy': 26,
    "\U0000F019", // 'nt_rain': 27,
    "\U0000F01B", // 'nt_snow': 28,
    "\U0000F01D", // 'nt_tstorms': 29,
    "\U0000F0b5", // 'nt_sleat': 30,
    "\U0000F038", // 'nt_flurries': 31,
    "\U0000F04A", // 'nt_hazy': 32,
    "\U0000F01D", // 'nt_chancetstorms': 33,
    "\U0000F038", // 'nt_chancesnow': 34,
    "\U0000F0B3", // 'nt_chancesleat': 35,
    "\U0000F036", // 'nt_chancerain': 36,
    "\U0000F038", // 'nt_chanceflurries': 37,
    "\U0000F003", // 'fog': 38,
    "\U0000F04A", // 'nt_fog': 39,
    "\U0000F050", // 'strong wind': 40,
    "\U0000F015", // 'hail': 41,
    "\U0000F056", // 'tornado': 42,
};

static char* wind_direction[] = {
    "\U0000F044",//'North': 0,
    "\U0000F043", //'NNE': 1,
     "\U0000F043",//'NE': 2,
     "\U0000F043",//'ENE': 3,
     "\U0000F048",//'East': 4,
     "\U0000F087",//'ESE': 5,
     "\U0000F087",//'SE': 6,
     "\U0000F087",//'SSE': 7,
     "\U0000F058",//'South': 8,
     "\U0000F057",//'SSW': 9,
     "\U0000F057",//'SW': 10,
     "\U0000F057",//'WSW': 11,
     "\U0000F04D",//'West': 12,
     "\U0000F088",//'WNW': 13,
     "\U0000F088",//'NW': 14,
     "\U0000F088",//'NNW': 15,
     "\U0000F04B", //'Variable': 16
};

//////Init Configuration///
//Init Clay app settings
ClaySettings settings;
// Initialize the default settings
static void prv_default_settings(){
  settings.Back1Color = GColorBlack;
  settings.Text1Color = GColorWhite;
  settings.Text2Color = GColorWhite;
  settings.Text3Color = GColorWhite;
  settings.Text4Color = GColorWhite;
  settings.Text5Color = GColorWhite;
  settings.Text7Color = GColorWhite;
  settings.HourColor = GColorWhite;
  settings.MinColor = GColorWhite;
  settings.HourColorN = GColorBlack;
  settings.MinColorN = GColorBlack;
  settings.Back1ColorN = GColorWhite;
  settings.Text1ColorN = GColorBlack;
  settings.Text2ColorN = GColorBlack;
  settings.Text3ColorN = GColorBlack;
  settings.Text4ColorN = GColorBlack;
  settings.Text5ColorN = GColorBlack;
  settings.Text7ColorN = GColorBlack;
  settings.WeatherUnit = 0;
  settings.WindUnit = 0;
  settings.UpSlider = 30;
  settings.NightTheme = true;
  settings.HealthOff = true;
  settings.AddZero12h = false;
  settings.RemoveZero24h = false;
}
int HourSunrise=700;
int HourSunset=2200;
bool BTOn=true;
bool GPSOn=true;
bool IsNightNow=false;
//////End Configuration///
///////////////////////////

static GColor ColorSelect(GColor ColorDay, GColor ColorNight){
  if (settings.NightTheme && IsNightNow && GPSOn ){
    return ColorNight;
  }
  else{
    return ColorDay;
  }
}

//// Callback for js request
void request_watchjs(){
  //Starting loop at 0
  s_loop = 0;
  // Begin dictionary
  DictionaryIterator * iter;
  app_message_outbox_begin( & iter);
  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);
  // Send the message!
  app_message_outbox_send();
}

////BT Connection status
static void bluetooth_callback(bool connected){
  BTOn = connected;
}

////BT Vibe on disconnect, when quiet time is not active
static void bluetooth_vibe_icon (bool connected) {
 layer_set_hidden(s_canvas_bt_icon, connected);
 layer_set_hidden(s_canvas_sunset_icon, !connected);

  if(!connected && !quiet_time_is_active()) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

////call weather data on reconnection
static void onreconnection(bool before, bool now){
  if (!before && now){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "BT reconnected, requesting weather at %d", s_minutes);
    request_watchjs();
  }
}

////Add in HEALTH to the watchface
static char s_current_steps_buffer[16];
static int s_step_count = 0, s_step_goal = 0, s_step_average = 0;

//// Is step data available?
bool step_data_is_available() {
  return HealthServiceAccessibilityMaskAvailable &
    health_service_metric_accessible(HealthMetricStepCount,
      time_start_of_today(), time(NULL));
}

//// Daily step goal
static void get_step_goal() {
  const time_t start = time_start_of_today();
  const time_t end = start + SECONDS_PER_DAY;
  s_step_goal = (int)health_service_sum_averaged(HealthMetricStepCount,
    start, end, HealthServiceTimeScopeDaily);
}

//// Today's current step count
static void get_step_count() {
    s_step_count = (int)health_service_sum_today(HealthMetricStepCount);
}


//// Average daily step count for this time of day
static void get_step_average() {
  const time_t start = time_start_of_today();
  const time_t end = time(NULL);
  s_step_average = (int)health_service_sum_averaged(HealthMetricStepCount,
    start, end, HealthServiceTimeScopeDaily);
}

////display step count
static void display_step_count() {
  int thousands = s_step_count / 1000;
  int hundreds = (s_step_count % 1000)/100;
//  int hundreds2 = (s_step_count % 1000);
  static char s_emoji[5];

  if(s_step_count >= s_step_average) {
    text_layer_set_text_color(s_step_layer, ColorSelect(settings.Text7Color, settings.Text7ColorN));//, color_winner));
    snprintf(s_emoji, sizeof(s_emoji), "\U0001F600");
  } else {
    text_layer_set_text_color(s_step_layer, ColorSelect(settings.Text7Color, settings.Text7ColorN));//, color_loser));
    snprintf(s_emoji, sizeof(s_emoji), "\U0001F61E");
  }

  if(thousands > 0) {
    snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
      "%d,%03d %s", thousands, hundreds, s_emoji);
  } else {
    snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
      "%d %s", hundreds, s_emoji);
  }
  text_layer_set_text(s_step_layer, s_current_steps_buffer);
}



static void health_handler(HealthEventType event, void *context) {
  if(event == HealthEventSignificantUpdate) {
    get_step_goal();
  }

  if(event != HealthEventSleepUpdate) {
    get_step_count();
    get_step_average();
    display_step_count();

  }
}

void update_hour_area_layer(Layer *l, GContext* ctx7) {
  // check layer bounds
  GRect bounds = layer_get_unobstructed_bounds(l);

  #ifdef PBL_ROUND
 //   bounds = GRect(0, ROUND_VERTICAL_PADDING, bounds.size.w, bounds.size.h - ROUND_VERTICAL_PADDING * 2);
     bounds = GRect(0, 0, 76, 180);
  #else
     bounds = GRect(0,0,58,168);
  #endif

  // initialize FCTX, the 3rd party drawing library
  FContext fctx;

  fctx_init_context(&fctx, ctx7);
  fctx_set_color_bias(&fctx, 0);
  fctx_set_fill_color(&fctx, ColorSelect(settings.HourColor, settings.HourColorN));

 // calculate font size
  int font_size = bounds.size.h/1.8;

  // font adjustment metrics
//  int v_padding = bounds.size.h / 16;
//  int h_padding = bounds.size.w / 16;
  int h_adjust = 0;
  int v_adjust = 0;


    #ifdef PBL_COLOR
      fctx_enable_aa(true);
    #endif


  // if it's a round watch, STUFF CHANGES
  #ifdef PBL_ROUND
//  v_adjust = ROUND_VERTICAL_PADDING;
    v_adjust = 0;

  #else
  // for rectangular watches, adjust X position based on sidebar position
  //    h_adjust -= ACTION_BAR_WIDTH / 2 + 1;
   h_adjust = 0;
  #endif

  FPoint time_pos;
  fctx_begin_fill(&fctx);
  fctx_set_text_em_height(&fctx, time_font, font_size);

  ////show time based on watch settings, as 12hour or 24hour
  /*int hourdraw;
  char hournow[8];
  if (clock_is_24h_style()){
    hourdraw=s_hours;
    snprintf(hournow,sizeof(hournow),"%02d",hourdraw);
    }
  else {
    if (s_hours==0 || s_hours==12){
      hourdraw=12;
    }
    else hourdraw=s_hours%12;
  snprintf(hournow, sizeof(hournow), "%d", hourdraw);
 // hourdraw = hourdraw1+(('0'==hourdraw1[0])?1:0));
}*/
  time_t temp = time(NULL);
  struct tm *time_now = localtime(&temp);

  char hourdraw[8];
  if(clock_is_24h_style() && settings.RemoveZero24h){
      strftime(hourdraw, sizeof(hourdraw),"%k",time_now);
  } else if (clock_is_24h_style() && !settings.RemoveZero24h) {
      strftime(hourdraw, sizeof(hourdraw),"%H",time_now);
  } else if (settings.AddZero12h) {
    strftime(hourdraw, sizeof(hourdraw),"%I",time_now);
  } else {
    strftime(hourdraw, sizeof(hourdraw),"%l",time_now);
  }
// draw hours
  time_pos.x = INT_TO_FIXED(bounds.size.w + h_adjust);
  time_pos.y = INT_TO_FIXED(bounds.size.h / 2 + v_adjust);

//  fctx_set_pivot(&fctx, time_pos);
  fctx_set_offset(&fctx, time_pos);
  fctx_draw_string(&fctx, hourdraw, time_font, GTextAlignmentRight, FTextAnchorMiddle);
  fctx_end_fill(&fctx);

  fctx_deinit_context(&fctx);
}

void update_minute_area_layer(Layer *a, GContext* ctx8) {
  // check layer bounds
 GRect bounds = layer_get_unobstructed_bounds(a);

  #ifdef PBL_ROUND
//   bounds = GRect(0, ROUND_VERTICAL_PADDING, bounds.size.w, bounds.size.h - ROUND_VERTICAL_PADDING * 2);
     bounds = GRect(100, 0, 80, 180);
  #else
     bounds = GRect(84,0,62,168);
  #endif
  // initialize FCTX, the 3rd party drawing library
  FContext fctx1;

  fctx_init_context(&fctx1, ctx8);
  fctx_set_color_bias(&fctx1, 0);
  fctx_set_fill_color(&fctx1, ColorSelect(settings.MinColor, settings.MinColorN));


  // calculate font size
 // int font_size = 4 * bounds.size.h / 7;
  int font_size2 = bounds.size.h/1.8;

  // font adjustment metrics
//  int v_padding2 = bounds.size.h / 16;
  int h_adjust2 = 0;
  int v_adjust2 = 0;


    #ifdef PBL_COLOR
      fctx_enable_aa(true);
    #endif


  // if it's a round watch, STUFF CHANGES
  #ifdef PBL_ROUND
//   v_adjust2 = ROUND_VERTICAL_PADDING;
     v_adjust2 = 0;

  #else
 // for rectangular watches, adjust X position based on sidebar position
 //  h_adjust2 -= ACTION_BAR_WIDTH / 2 + 1;
     h_adjust2 = 0;
  #endif

  FPoint time_pos2;
  fctx_begin_fill(&fctx1);
  fctx_set_text_em_height(&fctx1, time_font, font_size2);

  /*int mindraw;
  mindraw = s_minutes;
  char minnow[8];
  snprintf(minnow, sizeof(minnow), "%02d", mindraw);*/
  time_t temp = time(NULL);
  struct tm *time_now = localtime(&temp);

  char mindraw[8];
  strftime(mindraw, sizeof(mindraw),"%M",time_now);

  //draw minutes
  time_pos2.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(104,86) + h_adjust2);
  time_pos2.y = INT_TO_FIXED(bounds.size.h / 2 + v_adjust2);
  fctx_set_offset(&fctx1, time_pos2);
  fctx_draw_string(&fctx1, mindraw, time_font, GTextAlignmentLeft, FTextAnchorMiddle);
  fctx_end_fill(&fctx1);

  fctx_deinit_context(&fctx1);
}

//Update vertical weather complications layer
static void layer_update_proc_pre_rotate(Layer * layer1, GContext * ctx1){
  // Create Rects
  GRect bounds1 = layer_get_bounds(layer1);

  GRect MediumBand =
    PBL_IF_ROUND_ELSE(
      GRect(0,0,180,180),
      GRect(0, 0, (bounds1.size.w), bounds1.size.h));

  GRect WindKtsNowRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-10,2,20,20)),
      (GRect(bounds1.size.w/2-10,0,20,20))));

  GRect WindDirNowRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-10,20,20,20)),
      (GRect(bounds1.size.w/2-10,16,20,20))));

  GRect TempRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-15,34,30,20)),
      (GRect(bounds1.size.w/2-15,28,30,20))));

  GRect IconNowRect =
    (PBL_IF_ROUND_ELSE(
       GRect(90-20, 56,40,40),
       GRect(72-20, 50, 40,40)));

  GRect CondRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-15,78,30,20)),
      (GRect(bounds1.size.w/2-15,72,30,20))));

 GRect IconForeRect =
    (PBL_IF_ROUND_ELSE(
      GRect (90-20, 100,40,40),
      GRect(72-20, 94, 40,40)));

 GRect TempLowRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-15,122,30,20)),
      (GRect(bounds1.size.w/2-15,116,30,20))));

 GRect WindDirAveRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-10,142,20,20)),
      (GRect(bounds1.size.w/2-10,134,20,20))));

 GRect WindKtsAveRect =
    (PBL_IF_ROUND_ELSE(
      (GRect(bounds1.size.w/2-15,158,30,20)),
      (GRect(bounds1.size.w/2-15,148,30,20))));

char TempToDraw[20];
char CondToDraw[20];
char TempLowToDraw[20];
char IconNowToDraw[20];
char IconForeToDraw[20];
char WindDirNowToDraw[20];
char WindKtsNowToDraw[20];
char WindDirAveToDraw[20];
char WindKtsAveToDraw[20];

    snprintf(TempToDraw, sizeof(TempToDraw), "%s",tempstring);
    snprintf(CondToDraw, sizeof(CondToDraw), "%s",temphistring);
    snprintf(TempLowToDraw,sizeof(TempLowToDraw),"%s",templowstring);
    snprintf(IconNowToDraw, sizeof(IconNowToDraw),"%s",iconnowstring);
    snprintf(IconForeToDraw,sizeof(IconForeToDraw),"%s",iconforestring);

    snprintf(WindDirNowToDraw,sizeof(IconForeToDraw),"%s",windiconnowstring);
    snprintf(WindKtsNowToDraw,sizeof(IconForeToDraw),"%s",windstring);
    snprintf(WindDirAveToDraw,sizeof(IconForeToDraw),"%s",windiconavestring);
    snprintf(WindKtsAveToDraw,sizeof(IconForeToDraw),"%s",windavestring);

  //Build display, background colours
  graphics_context_set_fill_color(ctx1, ColorSelect(settings.Back1Color, settings.Back1ColorN));
  graphics_fill_rect(ctx1, bounds1, 0, GCornerNone);
  graphics_context_set_fill_color(ctx1, ColorSelect(settings.Back1Color, settings.Back1ColorN));
  graphics_fill_rect(ctx1, MediumBand,0,GCornerNone);

  // draw the weather conditions and temperatures
  graphics_context_set_text_color(ctx1, ColorSelect(settings.Text1Color, settings.Text1ColorN));
  graphics_draw_text(ctx1, TempToDraw, FontWeather, TempRect, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx1, CondToDraw, FontWeather, CondRect, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx1, TempLowToDraw, FontWeather, TempLowRect, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  graphics_draw_text (ctx1, IconNowToDraw,FontWeatherIcons,IconNowRect,GTextOverflowModeFill,GTextAlignmentCenter,NULL);
  graphics_draw_text(ctx1, IconForeToDraw,FontWeatherIcons,IconForeRect,GTextOverflowModeFill,GTextAlignmentCenter, NULL);

  // draw the wind conditions and direction
  graphics_context_set_text_color(ctx1, ColorSelect(settings.Text5Color, settings.Text5ColorN));
  graphics_draw_text(ctx1, WindDirNowToDraw,FontWeatherIcons,WindDirNowRect,GTextOverflowModeFill,GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx1, WindKtsNowToDraw,FontWind,WindKtsNowRect,GTextOverflowModeFill,GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx1, WindDirAveToDraw,FontWeatherIcons,WindDirAveRect,GTextOverflowModeFill,GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx1, WindKtsAveToDraw,FontWind,WindKtsAveRect,GTextOverflowModeFill,GTextAlignmentCenter, NULL);
}

//Update other complications layer
static void layer_update_proc(Layer * layer, GContext * ctx){
  // Create Rects
  GRect bounds3 = layer_get_bounds(layer);

  //int offsetdate = PBL_IF_RECT_ELSE(12, 10);
  GRect DateRect =
  //  (0, offsetdate, bounds3.size.w, bounds1.size.h/4);
    (PBL_IF_ROUND_ELSE(
        GRect((bounds3.size.w/2)+12, 122+10, 46, 20),
        GRect(96, bounds3.size.h-42, 46, 20)));

  GRect SunsetRect =
    (PBL_IF_ROUND_ELSE(
        GRect(0+18,122+10,bounds3.size.w/2-10-22,20),
        GRect(22, 126, bounds3.size.w/2,42)));

 GRect CitiRect =
    (PBL_IF_ROUND_ELSE(
        GRect(18, 30, bounds3.size.w/2-14-18,20),
        GRect(0,bounds3.size.h-20, bounds3.size.w/2 -10, 20)));

 GRect BatteryRect =
   (PBL_IF_ROUND_ELSE(
        GRect((bounds3.size.w/2)+12,142+8,bounds3.size.w,22),
        GRect((bounds3.size.w/2),bounds3.size.h - 20, bounds3.size.w/2, 20)));

  //Date
  // Local language
  const char * sys_locale = i18n_get_system_locale();
  char datenow[10];
  fetchwday(s_weekday, sys_locale, datenow);
  char convertday[4];
  snprintf(convertday, sizeof(convertday), " %02d", s_day);
  // Concatenate date
  strcat(datenow, convertday);

  //Battery
  int battery_level = battery_state_service_peek().charge_percent;
  char battperc[20];
  snprintf(battperc, sizeof(battperc), "%d", battery_level);
  strcat(battperc, "%");
  // Draw AM PM 24H
  /*char ampm[5];
  if (clock_is_24h_style()){
    snprintf(ampm, sizeof(ampm), "24H");
  }
  else if (s_hours<12){
    snprintf(ampm, sizeof(ampm), "AM");
  }
  else {
    snprintf(ampm, sizeof(ampm), "PM");
  }*/

//Location and Sunrise/sunset
  char LocToDraw[20];
  char SunsetToDraw[20];

  int nowthehouris = s_hours * 100 + s_minutes;
    snprintf(LocToDraw, sizeof(LocToDraw), "%s",citistring);

  //draw sunset if daytime, sunrise if nighttime
  if (clock_is_24h_style()){
    if (HourSunrise <= nowthehouris && nowthehouris <= HourSunset){
    snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",sunsetstring);
    }
    else{
    snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",sunrisestring);
    }
  } else {
    if (HourSunrise <= nowthehouris && nowthehouris <= HourSunset){
    snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",sunsetstring12);
    }
    else{
    snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",sunrisestring12);
    }
  }



  //draw sunrise sunset
  graphics_context_set_text_color(ctx, ColorSelect(settings.Text2Color, settings.Text2ColorN));
  graphics_draw_text(ctx, SunsetToDraw, FontDate, SunsetRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentRight,GTextAlignmentLeft), NULL);

  //draw location
  graphics_context_set_text_color(ctx,ColorSelect(settings.Text4Color,settings.Text4ColorN));
  graphics_draw_text(ctx, LocToDraw, FontCity, CitiRect, GTextOverflowModeTrailingEllipsis, PBL_IF_ROUND_ELSE(GTextAlignmentRight,GTextAlignmentLeft), NULL);

  //draw battery and date
  graphics_context_set_text_color(ctx, ColorSelect(settings.Text3Color, settings.Text3ColorN));
  graphics_draw_text(ctx, datenow, FontDate, DateRect, GTextOverflowModeTrailingEllipsis, PBL_IF_ROUND_ELSE(GTextAlignmentLeft,GTextAlignmentRight), NULL);
  graphics_draw_text(ctx, battperc, FontDate, BatteryRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentLeft, GTextAlignmentRight), NULL);

}

//update sunset, sunrise icon (layer will be hidden when bluetooth disconnected, or sunset or sunrise data is missing)
static void layer_update_proc_sunset(Layer * layer2, GContext * ctx2){
   // Create Rects
//  GRect bounds = layer_get_bounds(layer2);

  GRect SunsetIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(24,126+10,20,20),
      GRect(0,128+3,20,20)));

  char SunsetIconToDraw[20];
  char SunsetIconToShow[20];
  int nowthehouris = s_hours * 100 + s_minutes;
   if (HourSunrise <= nowthehouris && nowthehouris <= HourSunset){
      snprintf(SunsetIconToShow, sizeof(SunsetIconToShow),  "%s", "\U0000F052");
  }
  else{
      snprintf(SunsetIconToShow, sizeof(SunsetIconToShow),  "%s",  "\U0000F051");
  }


  if (strcmp(citistring, " ") == 0){
    snprintf(SunsetIconToDraw, sizeof(SunsetIconToDraw),  "%s", " ");
  }
  else {
    snprintf(SunsetIconToDraw, sizeof(SunsetIconToDraw), "%s",SunsetIconToShow);
  }

 graphics_context_set_text_color(ctx2, ColorSelect(settings.Text2Color, settings.Text2ColorN));
 graphics_draw_text(ctx2, SunsetIconToDraw, FontIcon, SunsetIconRect, GTextOverflowModeFill,GTextAlignmentCenter, NULL);

}

//update bluetooth icon (layer will replace sunset icon when bluetooth disconnected)
static void layer_update_proc_bt(Layer * layer3, GContext * ctx3){
   // Create Rects
//  GRect bounds = layer_get_bounds(layer2);

  GRect BTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(24,126+10,20,20),
      GRect(0,128+3,20,20)));

 onreconnection(BTOn, connection_service_peek_pebble_app_connection());
 bluetooth_callback(connection_service_peek_pebble_app_connection());

 graphics_context_set_text_color(ctx3, ColorSelect(settings.Text3Color, settings.Text3ColorN));
 graphics_draw_text(ctx3, "z", FontIcon2, BTIconRect, GTextOverflowModeFill,GTextAlignmentCenter, NULL);

}

/////////////////////////////////////////
////Init: Handle Settings and Weather////
/////////////////////////////////////////
// Read settings from persistent storage
static void prv_load_settings(){
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, & settings, sizeof(settings));
}
// Save the settings to persistent storage
static void prv_save_settings(){
  persist_write_data(SETTINGS_KEY, & settings, sizeof(settings));
}
// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator * iter, void * context){
  s_loop = s_loop + 1;
  if (s_loop == 1){
 ////Clean vars
  //  strcpy(tempstring, "");
  //  strcpy(condstring, "");
    strcpy(citistring, " ");  ///kept so that sunset icon is blank at startup or if there's missing weather data
  //  strcpy(windstring, "");
 //   strcpy(sunsetstring, " ");
  //  strcpy(iconnowstring, "");
  //  strcpy(iconforestring, "");
 //   strcpy(tempforestring, "");
    }

  // Background Coloor
  Tuple * bg1_color_t = dict_find(iter, MESSAGE_KEY_Back1Color);
  if (bg1_color_t){
    settings.Back1Color = GColorFromHEX(bg1_color_t-> value -> int32);
  }
  Tuple * nbg1_color_t = dict_find(iter, MESSAGE_KEY_Back1ColorN);
  if (nbg1_color_t){
    settings.Back1ColorN = GColorFromHEX(nbg1_color_t-> value -> int32);
  }
  //Text Colours
  Tuple * tx1_color_t = dict_find(iter, MESSAGE_KEY_Text1Color);
  if (tx1_color_t){
    settings.Text1Color = GColorFromHEX(tx1_color_t-> value -> int32);
  }
  Tuple * ntx1_color_t = dict_find(iter, MESSAGE_KEY_Text1ColorN);
  if (ntx1_color_t){
    settings.Text1ColorN = GColorFromHEX(ntx1_color_t-> value -> int32);
  }
  Tuple * hr_color_t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (hr_color_t){
    settings.HourColor = GColorFromHEX(hr_color_t-> value -> int32);
  }
  Tuple * nthr_color_t = dict_find(iter, MESSAGE_KEY_HourColorN);
  if (nthr_color_t){
    settings.HourColorN = GColorFromHEX(nthr_color_t-> value -> int32);
  }
  Tuple * min_color_t = dict_find(iter, MESSAGE_KEY_MinColor);
  if (min_color_t){
    settings.MinColor = GColorFromHEX(min_color_t-> value -> int32);
  }
  Tuple * ntmin_color_t = dict_find(iter, MESSAGE_KEY_MinColorN);
  if (ntmin_color_t){
    settings.MinColorN = GColorFromHEX(ntmin_color_t-> value -> int32);
  }
  Tuple * tx2_color_t = dict_find(iter, MESSAGE_KEY_Text2Color);
  if (tx2_color_t){
    settings.Text2Color = GColorFromHEX(tx2_color_t-> value -> int32);
  }
  Tuple * ntx2_color_t = dict_find(iter, MESSAGE_KEY_Text2ColorN);
  if (ntx2_color_t){
    settings.Text2ColorN = GColorFromHEX(ntx2_color_t-> value -> int32);
  }
   Tuple * tx3_color_t = dict_find(iter, MESSAGE_KEY_Text3Color);
  if (tx3_color_t){
    settings.Text3Color = GColorFromHEX(tx3_color_t-> value -> int32);
  }
  Tuple * ntx3_color_t = dict_find(iter, MESSAGE_KEY_Text3ColorN);
  if (ntx3_color_t){
    settings.Text3ColorN = GColorFromHEX(ntx3_color_t-> value -> int32);
  }
  Tuple * tx4_color_t = dict_find(iter,MESSAGE_KEY_Text4Color);
  if (tx4_color_t){
    settings.Text4Color = GColorFromHEX(tx4_color_t-> value -> int32);
    }
  Tuple * ntx4_color_t = dict_find(iter, MESSAGE_KEY_Text4ColorN);
  if(ntx4_color_t){
    settings.Text4ColorN = GColorFromHEX(ntx4_color_t-> value -> int32);
  }
  Tuple * tx5_color_t = dict_find(iter,MESSAGE_KEY_Text5Color);
  if (tx5_color_t){
    settings.Text5Color = GColorFromHEX(tx5_color_t-> value -> int32);
    }
  Tuple * ntx5_color_t = dict_find(iter, MESSAGE_KEY_Text5ColorN);
  if(ntx5_color_t){
    settings.Text5ColorN = GColorFromHEX(ntx5_color_t-> value -> int32);
  }
  Tuple * tx6_color_t = dict_find(iter,MESSAGE_KEY_Text7Color);
  if (tx6_color_t){
    settings.Text7Color = GColorFromHEX(tx6_color_t-> value -> int32);
    }
  Tuple * ntx6_color_t = dict_find(iter, MESSAGE_KEY_Text7ColorN);
  if(ntx6_color_t){
    settings.Text7ColorN = GColorFromHEX(ntx6_color_t-> value -> int32);
  }
  //Control of data from weather api
  // Weather Cond
  Tuple * wcond_t = dict_find(iter, MESSAGE_KEY_WeatherCond);
  if (wcond_t){
    snprintf(condstring, sizeof(condstring), "%s", wcond_t -> value -> cstring);
  }
  // Weather Temp
  Tuple * wtemp_t = dict_find(iter, MESSAGE_KEY_WeatherTemp);
  if (wtemp_t){
    snprintf(tempstring, sizeof(tempstring), "%s", wtemp_t -> value -> cstring);
  }
  //Weather wind
   Tuple * wwind_t = dict_find(iter, MESSAGE_KEY_WeatherWind);
  if (wwind_t){
    snprintf(windstring, sizeof(windstring), "%s", wwind_t -> value -> cstring);
  }
  //Wind forecast
     Tuple * wwindave_t = dict_find(iter, MESSAGE_KEY_WindFore);
  if (wwindave_t){
    snprintf(windavestring, sizeof(windavestring), "%s", wwindave_t -> value -> cstring);
  }
  //Times of Sunrise and Sunset
  Tuple * sunrise_t = dict_find(iter, MESSAGE_KEY_HourSunrise);
  if (sunrise_t){
    HourSunrise = (int) sunrise_t -> value -> int32;
  }
  Tuple * sunset_t = dict_find(iter, MESSAGE_KEY_HourSunset);
  if (sunset_t){
    HourSunset = (int) sunset_t -> value -> int32;
  }
  //Display versions of Sunset and Sunrise
  Tuple * sunset_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNSET_KEY);
  if (sunset_dt){
     snprintf(sunsetstring, sizeof(sunsetstring), "%s", sunset_dt -> value -> cstring);
  }
  Tuple * sunrise_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNRISE_KEY);
  if (sunrise_dt){
     snprintf(sunrisestring, sizeof(sunrisestring), "%s", sunrise_dt -> value -> cstring);
  }
  Tuple * sunset12_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNSET_KEY_12H);
  if (sunset12_dt){
     snprintf(sunsetstring12, sizeof(sunsetstring12), "%s", sunset12_dt -> value -> cstring);
  }
   Tuple * sunrise12_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNRISE_KEY_12H);
  if (sunrise12_dt){
     snprintf(sunrisestring12, sizeof(sunrisestring12), "%s", sunrise12_dt -> value -> cstring);
  }
  //Weather and wind icons
  Tuple * iconnow_tuple = dict_find(iter, MESSAGE_KEY_IconNow);
  if (iconnow_tuple){
    snprintf(iconnowstring,sizeof(iconnowstring),"%s",weather_conditions[(int)iconnow_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");  //test for sizing of different symbols
  }
  Tuple * iconfore_tuple = dict_find(iter, MESSAGE_KEY_IconFore);
  if (iconfore_tuple){
    snprintf(iconforestring,sizeof(iconforestring),"%s",weather_conditions[(int)iconfore_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }
   Tuple * iconwinddirnow_tuple = dict_find(iter, MESSAGE_KEY_WindIconNow);
  if (iconwinddirnow_tuple){
    snprintf(windiconnowstring,sizeof(windiconnowstring),"%s",wind_direction[(int)iconwinddirnow_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }
   Tuple * iconwinddirave_tuple = dict_find(iter, MESSAGE_KEY_WindIconAve);
  if (iconwinddirave_tuple){
    snprintf(windiconavestring,sizeof(windiconavestring),"%s",wind_direction[(int)iconwinddirave_tuple->value->int32]);
    //  snprintf(forecast_result,sizeof(forecast_result),"%s","\U0000F002");
  }

  Tuple * wforetemp_t = dict_find(iter, MESSAGE_KEY_TempFore);
  if (wforetemp_t){
    snprintf(temphistring, sizeof(temphistring), "%s", wforetemp_t -> value -> cstring);
  }

  Tuple * wforetemplow_t = dict_find(iter, MESSAGE_KEY_TempForeLow);
  if (wforetemplow_t){
    snprintf(templowstring, sizeof(templowstring), "%s", wforetemplow_t -> value -> cstring);
  }

  // Location
  Tuple * neigh_t = dict_find(iter, MESSAGE_KEY_NameLocation);
  if (neigh_t){
    snprintf(citistring, sizeof(citistring), "%s", neigh_t -> value -> cstring);
  }

  //Control of data gathered for http
  APP_LOG(APP_LOG_LEVEL_DEBUG, "After loop %d temp is %s Cond is %s and City is %s and Wind is %s", s_loop, tempstring, condstring, citistring, windstring);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Sunrise is %d Sunset is %d SunsetDisplay is %s SunriseDisplay is %s", HourSunrise,HourSunset,sunsetstring,sunrisestring);
  if (strcmp(tempstring, "") != 0 && strcmp(condstring, "") != 0 && strcmp(citistring, "")){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "GPS fully working at loop %d", s_loop);
    GPSOn = true;
  } else{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Missing info at loop %d, GPS false", s_loop);
    GPSOn = false;
  }
  //End data gathered
  // Get display handlers
  Tuple * frequpdate = dict_find(iter, MESSAGE_KEY_UpSlider);
  if (frequpdate){
    settings.UpSlider = (int) frequpdate -> value -> int32;
    //Restart the counter
    s_countdown = settings.UpSlider;
  }
  Tuple * disntheme_t = dict_find(iter, MESSAGE_KEY_NightTheme);
  if (disntheme_t){
    if (disntheme_t -> value -> int32 == 0){
      settings.NightTheme = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "NTheme off");
    } else settings.NightTheme = true;
  }

  Tuple * health_t = dict_find(iter, MESSAGE_KEY_HealthOff);
  if (health_t){
    if (health_t -> value -> int32 == 0){
      settings.HealthOff = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Health on");
      get_step_count();
      get_step_average();
      display_step_count();
    } else {
      settings.HealthOff = true;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Health off");
      snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer),
       "%s", "");
    }
  }

  Tuple * addzero12_t = dict_find(iter, MESSAGE_KEY_AddZero12h);
  if (addzero12_t){
    if (addzero12_t -> value -> int32 == 0){
      settings.AddZero12h = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Add Zero 12h off");
    } else {
    settings.AddZero12h = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Add Zero 12h on");
      }
    }

  Tuple * remzero24_t = dict_find(iter, MESSAGE_KEY_RemoveZero24h);
  if (remzero24_t){
    if (remzero24_t -> value -> int32 == 0){
      settings.RemoveZero24h = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Remove Zero 24h off");
    } else {
    settings.RemoveZero24h = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Remove Zero 24h off");
      }
    }
  //Update colors
  layer_mark_dirty(s_canvas_to_be_rotated);
  layer_mark_dirty(s_canvas);
  layer_mark_dirty(s_canvas_sunset_icon);
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(hour_area_layer);
  layer_mark_dirty(minute_area_layer);

  // Save the new settings to persistent storage
  prv_save_settings();
}


//Load main layer
static void window_load(Window * window){
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds4 = layer_get_bounds(window_layer);
  s_canvas_to_be_rotated = layer_create(bounds4);
  layer_set_update_proc(s_canvas_to_be_rotated, layer_update_proc_pre_rotate);
  layer_add_child(window_layer, s_canvas_to_be_rotated);

 /*  effect_layer = layer_create(bounds);

 effect_layer_2 = effect_layer_create(PBL_IF_ROUND_ELSE(
      GRect(0,0,180,180),
      GRect(0, 0,144,168 )));
  effect_layer_add_effect(effect_layer_2, effect_rotate_90_degrees, (void *)true);
  layer_add_child(window_get_root_layer(s_window), effect_layer_get_layer(effect_layer_2));*/


  s_canvas = layer_create(bounds4);
    layer_set_update_proc(s_canvas, layer_update_proc);
    layer_add_child(window_layer, s_canvas);

  s_canvas_sunset_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_sunset_icon, layer_update_proc_sunset);
    layer_add_child(window_layer, s_canvas_sunset_icon);

  s_canvas_bt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_bt_icon, layer_update_proc_bt);
    layer_add_child(window_layer, s_canvas_bt_icon);

  s_step_layer = text_layer_create (PBL_IF_ROUND_ELSE(
    GRect((bounds4.size.w/2)+12, 26, bounds4.size.w, 20),
    GRect((bounds4.size.w/2), 22, (bounds4.size.w/2), 20)));
    text_layer_set_background_color(s_step_layer, GColorClear);
    text_layer_set_font(s_step_layer,
                      FontSteps);
            //        fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(s_step_layer, (PBL_IF_ROUND_ELSE(GTextAlignmentLeft, GTextAlignmentRight)));
    layer_add_child(window_layer, text_layer_get_layer(s_step_layer));

  hour_area_layer = layer_create(bounds4);
    layer_add_child(window_get_root_layer(s_window), hour_area_layer);
    layer_set_update_proc(hour_area_layer, update_hour_area_layer);

  minute_area_layer = layer_create(bounds4);
    layer_add_child(window_get_root_layer(s_window), minute_area_layer);
    layer_set_update_proc(minute_area_layer, update_minute_area_layer);
}


static void window_unload(Window * window){
  layer_destroy(s_canvas_to_be_rotated);
 // effect_layer_destroy(effect_layer_2);
//    effect_layer_destroy(effect_layer_3);
  text_layer_destroy    (s_step_layer);
  layer_destroy(s_canvas);
  layer_destroy(hour_area_layer);
  layer_destroy(minute_area_layer);
  layer_destroy(s_canvas_sunset_icon);
  layer_destroy(s_canvas_bt_icon);
  //if (effect_layer != NULL) {
	//  effect_layer_destroy(effect_layer);
  // }
  window_destroy(s_window);
  fonts_unload_custom_font(FontIcon);
  fonts_unload_custom_font(FontIcon2);
  fonts_unload_custom_font(FontWeatherIcons);
  ffont_destroy(time_font);
}

void main_window_push(){
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

void main_window_update(int hours, int minutes, int weekday, int day){
  s_hours = hours;
  s_minutes = minutes;
  s_day = day;
  s_weekday = weekday;

  layer_mark_dirty(s_canvas);
  layer_mark_dirty(s_canvas_to_be_rotated);
  layer_mark_dirty(s_canvas_sunset_icon);
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(hour_area_layer);
  layer_mark_dirty(minute_area_layer);
}

static void tick_handler(struct tm * time_now, TimeUnits changed){
  main_window_update(time_now -> tm_hour, time_now -> tm_min, time_now -> tm_wday, time_now -> tm_mday);
  //update_time();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %d", time_now -> tm_min);
  s_loop = 0;
  if (s_countdown == 0){
    //Reset
    s_countdown = settings.UpSlider;
  } else{
    s_countdown = s_countdown - 1;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Countdown to update %d", s_countdown);
  // Evaluate if is day or night
  int nowthehouris = s_hours * 100 + s_minutes;
  if (HourSunrise <= nowthehouris && nowthehouris <= HourSunset){
    IsNightNow = false;
  } else{
    IsNightNow = true;
  }
  // Extra catch on sunrise and sunset
  if (nowthehouris == HourSunrise || nowthehouris == HourSunset){
    s_countdown = 1;
  }
  if (GPSOn && settings.NightTheme){
    //Extra catch around 1159 to gather information of today
    if (nowthehouris == 1159 && s_countdown > 5){
      s_countdown = 1;
    };
    // Change Color of background
    layer_mark_dirty(s_canvas_to_be_rotated);

  }
  // Get weather update every requested minutes and extra request 5 minutes earlier
  if (s_countdown == 0 || s_countdown == 5){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Update weather at %d", time_now -> tm_min);
      request_watchjs();
  }
  //If GPS was off request weather every 15 minutes
  if (!GPSOn){
      if (settings.UpSlider > 15){
        if (s_countdown % 15 == 0){
          APP_LOG(APP_LOG_LEVEL_DEBUG, "Attempt to request GPS on %d", time_now -> tm_min);
          request_watchjs();
        }
      }
    }
 }

static void init(){
  //color_loser = GColorRed;
  //color_winner = GColorJaegerGreen;

  prv_load_settings();
  //// Listen for AppMessages
  ////Starting loop at 0
  s_loop = 0;
  s_countdown = settings.UpSlider;
  //Clean vars
 // strcpy(tempstring, "");
 //strcpy(condstring, "");
  strcpy(citistring, " ");
 // strcpy(windstring,"");
 // strcpy(sunsetstring, " ");
 // strcpy(iconnowstring, "");
 //  strcpy(iconforestring, "");
 //  strcpy(tempforestring, "");

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  s_hours=t->tm_hour;
  s_minutes=t->tm_min;
  s_day=t->tm_mday;
  s_weekday=t->tm_wday;
  //Register and open
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(512, 512);
  // Load Fonts
   // allocate fonts
    time_font =  ffont_create_from_resource(RESOURCE_ID_FONT_STEELFISH);

  FontDate = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  FontIcon = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_12));
  FontIcon2 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
  FontWeather = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  FontCity = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  FontWeatherIcons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_18));
  FontSteps = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  FontWind = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  main_window_push();

  // Register with Event Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  if (!settings.HealthOff && step_data_is_available())  {
    health_service_events_subscribe(health_handler,NULL);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "health is on, and steps data is subscribed");
  }
  connection_service_subscribe((ConnectionHandlers){
    .pebble_app_connection_handler = bluetooth_vibe_icon
  });
  bluetooth_vibe_icon(connection_service_peek_pebble_app_connection());

}
static void deinit(){
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks(); //Destroy the callbacks for clean up
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
  health_service_events_unsubscribe();
}
int main(){
  init();
  app_event_loop();
  deinit();
}
