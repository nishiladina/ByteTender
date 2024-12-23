// change this depending on microstepping mode
// 200 if full step mode (nothing plugged into MS1/MS2/MS3)
// number of steps = 200 / (microstep amount)
// e.g. quarter step --> number of steps = 200 / (1/4) = 200 * 4 = 800
#define STEPS_PER_ROTATION 800

// change these depending on what motor gets plugged into A vs B vs C
#define Z1STEP ASTEP
#define Z1DIR ADIR
#define Z2STEP BSTEP
#define Z2DIR BDIR
#define XSTEP CSTEP
#define XDIR CDIR

// change these depending on which drink is where
// DRINK1 is on the right (near PCB), DRINK4 is on the left
#define GINGER_ALE DRINK1
#define HAWAIIAN_PUNCH DRINK2
#define SYRUP DRINK3
#define SPRITE DRINK4

// WiFi SSID and password
#define WIFI_SSID "CICS_Makerspace"
#define WIFI_PASSWORD "makerspace"

// your personal OTC token
#define CLOUD_TOKEN "OT4fa175d0cfd69f1457b8dba3d4d175"
