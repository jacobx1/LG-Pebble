#include <pebble.h>
#include <menu.h>
	
#define NUM_MENU_SECTIONS 1

enum {
	DICT_ADDRESS = 0x0,
	DICT_PAIR_CODE,
	COMMAND,
	DATA
};

// Buffers for control info
char address[32] = {0};
char pairCode[16] = {0};

bool didSendAddress = false;

static Window *window;

// This is a menu layer
// You have more control than with a simple menu layer
static MenuLayer *menu_layer;

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return numSets;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return menuSets[section_index].numbuttons;
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	// Determine which section we're working with
  	menu_cell_basic_header_draw(ctx, cell_layer, menuSets[section_index].title);
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  
	ControlButton* pushedButton = &menuSets[cell_index->section].menu[cell_index->row];
	
	menu_cell_basic_draw(ctx, cell_layer, pushedButton->title, pushedButton->subtitle, NULL);
	
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	
	// Use the row to specify which item will receive the select action
	ControlButton* pushedButton = &menuSets[cell_index->section].menu[cell_index->row];
	
	if (pushedButton->action == REMOTE_ACTION)
	{
		DictionaryIterator *iter;
		if (app_message_outbox_begin(&iter) != APP_MSG_OK) {
			//return;
		}
		
		if (!didSendAddress)
		{
			if (dict_write_cstring(iter, DICT_ADDRESS, address) != DICT_OK) {
			//return;
			}
			
			if (dict_write_cstring(iter, DICT_PAIR_CODE, pairCode) != DICT_OK) {
				//return;
			}
			
			didSendAddress = true;
		}
		
		if (dict_write_cstring(iter, COMMAND, pushedButton->action) != DICT_OK) {
			//return;
		}
		
		if (dict_write_cstring(iter, DATA, pushedButton->data) != DICT_OK) {
			//return;
		}
		
		dict_write_end(iter);
		app_message_outbox_send();
	}

	if (pushedButton->action == DISPLAY_IP)
	{
		// Update subtitle to show IP
		pushedButton->subtitle = address;
		menu_layer_reload_data(menu_layer);
	}
	
	if (pushedButton->action == DISPLAY_PAIR_CODE)
	{
		// Update subtitle to show Pair code
		pushedButton->subtitle = pairCode;
		menu_layer_reload_data(menu_layer);
	}
	
	if (pushedButton->action == SYNC_ACTION)
	{
		didSendAddress = false;
	}
}

// This initializes the menu upon window load
void window_load(Window *window) {

  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);
}

static void message_received(DictionaryIterator *iterator, void *context)
{
	Tuple* address_tuple = dict_find(iterator, DICT_ADDRESS);
	strcpy(address, address_tuple->value->cstring);
	persist_write_string(DICT_ADDRESS, address);
	
	Tuple* pair_tuple = dict_find(iterator, DICT_PAIR_CODE);
	strcpy(pairCode, pair_tuple->value->cstring);
	persist_write_string(DICT_PAIR_CODE, pairCode);
}

static void app_message_init(void) 
{
  // Reduce the sniff interval for more responsive messaging at the expense of
  // increased energy consumption by the Bluetooth module
  // The sniff interval will be restored by the system after the app has been
  // unloaded
  app_comm_set_sniff_interval(SNIFF_INTERVAL_REDUCED);
  // Init buffers
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
  app_message_register_inbox_received(&message_received);
}

int main(void) {
  window = window_create();
  app_message_init();
  initButtons();

  if (persist_exists(DICT_ADDRESS))
  {
	  persist_read_string(DICT_ADDRESS, address, sizeof(address)-1);
  }
	
  if (persist_exists(DICT_PAIR_CODE))
  {
	  persist_read_string(DICT_PAIR_CODE, pairCode, sizeof(pairCode)-1);
  }
	
  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);

  app_event_loop();

  window_destroy(window);
}
