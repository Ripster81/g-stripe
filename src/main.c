#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer;
static Layer *s_canvas_layer, *s_canvas2_layer, *s_vert_layer, *s_vert2_layer;
static GFont s_time_font, s_date_font;
//static BitmapLayer *s_background_layer, *s_bt_icon_layer;
//static GBitmap *s_background_bitmap, *s_bt_icon_bitmap;

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  // Set the line color
  graphics_context_set_stroke_color(ctx, GColorBlack);

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlack);
 
  GRect rect_bounds = GRect(0, 44, 144, 50);
  
  // Draw a rectangle
  graphics_draw_rect(ctx, rect_bounds);

  // Fill a rectangle with rounded corners
  int corner_radius = 1;
  graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

}

static void canvas2_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  // Set the line color
  graphics_context_set_stroke_color(ctx, GColorBlack);

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlack);
 
  GRect rect_bounds = GRect(0, 120, 144, 22);
  
  // Draw a rectangle
  graphics_draw_rect(ctx, rect_bounds);

  // Fill a rectangle with rounded corners
  int corner_radius = 1;
  graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

}

static void vert_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  // Set the line color
  graphics_context_set_stroke_color(ctx, GColorLightGray);

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorLightGray);
 
  GRect rect_bounds = GRect(28, 0, 29, 168);
  
  // Draw a rectangle
  graphics_draw_rect(ctx, rect_bounds);

  // Fill a rectangle with rounded corners //
  int corner_radius = 1;
  graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

}

static void vert2_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  // Set the line color
  graphics_context_set_stroke_color(ctx, GColorLightGray);

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorLightGray);
 
  GRect rect_bounds = GRect(86, 0, 29, 168);
  
  // Draw a rectangle
  graphics_draw_rect(ctx, rect_bounds);

  // Fill a rectangle with rounded corners //
  int corner_radius = 1;
  graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

}


static void bluetooth_callback(bool connected) {
  
  if(!connected) {
    vibes_double_pulse();
  }
}

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer, and show the time
  static char buffer[] = "00:00";
  if(clock_is_24h_style()) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  text_layer_set_text(s_time_layer, buffer);
  
  // Show the date
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  
  //Create GBitmap, then set to created BitmapLayer
  //s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
  //s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  //bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Create canvas layer
  s_vert_layer = layer_create(bounds);
  s_vert2_layer = layer_create(bounds);
  s_canvas_layer = layer_create(bounds);
  s_canvas2_layer = layer_create(bounds);
                                
  
  // Assign the custom drawing procedure
  layer_set_update_proc(s_vert_layer, vert_update_proc);
  layer_set_update_proc(s_vert2_layer, vert2_update_proc);  
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_set_update_proc(s_canvas2_layer, canvas2_update_proc);


  // Add to Window
  layer_add_child(window_get_root_layer(window), s_vert_layer);
  layer_add_child(window_get_root_layer(window), s_vert2_layer);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);
  layer_add_child(window_get_root_layer(window), s_canvas2_layer);


  // Create GFonts
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPEN_60));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OPEN_17));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 30, 144, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorLightGray);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, 120, 144, 23));
  text_layer_set_text_color(s_date_layer, GColorLightGray);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_text(s_date_layer, "Sept 23");
  text_layer_set_font(s_date_layer, s_date_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  // Initialize the display
  update_time();

  bluetooth_callback(connection_service_peek_pebble_app_connection());

}

static void main_window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  
  //gbitmap_destroy(s_background_bitmap);
  //gbitmap_destroy(s_bt_icon_bitmap);

  //bitmap_layer_destroy(s_background_layer);
  //bitmap_layer_destroy(s_bt_icon_layer);
  
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  
}
  
static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  
  // Register with Event Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
  
  //Background color
  window_set_background_color(s_main_window, GColorBlack);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}