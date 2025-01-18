/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 18;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 18;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=12" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#486c75";
static const char col_custom[]      = "#68a0ad";
static const unsigned int baralpha  = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {


  /*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_custom,  col_custom  },
};

static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
  [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};


/* Autostart */

static const char *const autostart[] = {
	"feh", "--bg-fill", "/home/ryusuke/Pictures/Wallpaper/sololeveling.png", NULL,
 // "xwinwrap", "-g", "1920x1080", "-ni", "-s", "-nf", "-b", "-un", "-ov", "-fdt", "-argb", "--",  "mpv", "--wid=%WID", "--loop", "--no-audio", "--fs", "--panscan=1", "/home/ryusuke/Pictures/Wallpaper/pixel_sakura.gif", NULL,
  "picom", "-b", NULL, 
  "setxkbmap", "-option", "caps:escape", NULL, 
  "/usr/lib/xfce-polkit/xfce-polkit", NULL,
  "/usr/local/bin/dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "S" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor*/
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 }, 
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },    

};

#define FORCE_VSPLIT 0  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c" 
 
/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "><>",      NULL },    /* no layout function means floating behavior */
  { "[@]",      spiral },
  { "HHH",      grid }, 
  // { "[]=",      tile },    /* first entry is default */
  { ">M>",      centeredfloatingmaster },
  { "[\\]",     dwindle }, 
  { "[M]",      monocle }, 
  { "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

/* Volume and Brightness */

static const char *upvol[]      = { "/usr/bin/sh", "-c", "/usr/bin/wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+ ; pkill -RTMIN+10 dwmblocks", NULL };
static const char *downvol[]    = { "/usr/bin/sh", "-c", "/usr/bin/wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- ; pkill -RTMIN+10 dwmblocks",  NULL };
static const char *mutevol[]    = { "/usr/bin/wpctl",   "set-mute",   "@DEFAULT_AUDIO_SINK@",      "toggle",   NULL };

static const char *light_up[]   = { "/usr/bin/brightnessctl",   "s", "3+", NULL };
static const char *light_down[] = { "/usr/bin/brightnessctl",   "s", "5-", NULL };
static const char *kbl_up[]     = { "/usr/bin/brightnessctl",   "-s", "-d", "asus::kbd_backlight", "s", "1+", NULL };
static const char *kbl_down[]     = { "/usr/bin/brightnessctl",   "-s", "-d", "asus::kbd_backlight", "s", "1-", NULL };

/* Argument Memek */
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *screenshot[] = { "sh", "-c", "scrot -s /home/ryusuke/Pictures/%Y-%m-%d_%H:%M:%S.png -e 'xclip -selection clipboard -t image/png < $f'", NULL };
static const char *vsCode[]  = { "firefox", "https://www.facebook.com/groups/programmerhandal/", NULL };
#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
  { MODKEY,                       XK_p,      spawn,          {.v = screenshot } },
  { MODKEY,                       XK_v,      spawn,          {.v = vsCode } },
  { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,        {0} },
  { MODKEY,                       XK_s,      togglespecialtag, {0} },
  { MODKEY|ShiftMask,             XK_s,      movetospecialtag, {0} },


	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
//	{ MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY,                       XK_f,      togglefullscr,  {0} },
  { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
  { MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_comma,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_space,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
//	{ MODKEY,                       XK_space,  setlayout,      {} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
/*	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, */
	
// Volume
  
  { 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
 
// Brightness

	{ 0,				XF86XK_MonBrightnessUp,		spawn,	{.v = light_up} },
  { 0,				XF86XK_MonBrightnessDown,	spawn,	{.v = light_down} },
  { 0,				XF86XK_KbdBrightnessUp,		spawn,	{.v = kbl_up} },
	{ 0,				XF86XK_KbdBrightnessDown,	spawn,	{.v = kbl_down} },




  TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	//TAGKEYS(                        XK_s,                      8)
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


