#ifndef MENU
#define MENU

#define MAX_BUTTONS 10
#define MAX_SETS 5

static int numButtons = 0;
static int numSets = 0;

static char DISPLAY_IP[] = "display_ip";
static char DISPLAY_PAIR_CODE[] = "display_pair";
static char REMOTE_ACTION[] = "remote";
static char SYNC_ACTION[] = "sync";

typedef struct {
  char* title;
  char* subtitle;
  char* action;
  char* data;
} ControlButton;

typedef struct {
  ControlButton menu[MAX_BUTTONS];
  char* title;
  int numbuttons;
} MenuSet;

MenuSet menuSets[MAX_SETS];

void initButtons(void)
{
  // General Menu
  menuSets[numSets].numbuttons = 0;
  menuSets[numSets].title = "Quick Access";
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "TV Off",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "1"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Input Select",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "47"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "OK Button",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "20"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Mute Toggle",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "26"
  };
  numSets++;
  
  // Extended Functions
  menuSets[numSets].numbuttons = 0;
  menuSets[numSets].title = "Other Buttons";
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Volume Up",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "24"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Volume Down",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "25"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Channel Up",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "27"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Channel Down",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "28"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "AV Mode",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "110"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Aspect Ratio",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "46"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Exit Button",
    .subtitle = NULL,
    .action = REMOTE_ACTION,
    .data = "112"
  };
  
  numSets++;
  
  // Settings
  /*menuSets[numSets].numbuttons = 0;
  menuSets[numSets].title = "Settings/Info";
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Sync TV Address",
    .subtitle = "Sync TV Address",
    .action = SYNC_ACTION,
    .data = "NULL"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "IP Address",
    .subtitle = "0.0.0.0",
    .action = DISPLAY_IP,
    .data = "NULL"
  };
  
  menuSets[numSets].menu[menuSets[numSets].numbuttons++] = (ControlButton) {
    .title = "Pair Code",
    .subtitle = "000000",
    .action = REMOTE_ACTION,
    .data = "pair"
  };
  
  numSets++; */
}

#endif