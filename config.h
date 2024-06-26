#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 16;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode Nerd Font:size=15" };
static const char dmenufont[]       = "FiraCode Nerd Font:size=20";
static const char stfont[]          = "FiraCode Nerd Font:pixelsize=17.5:antialias=true:autohint=true";
static const char hi_fg[]           = "#ff757f";
static const char norm_fg[]         = "#828bb8";
static const char norm_bg[]         = "#1b1d2b";
static const char sele_fg[]         = "#444a73";
static const char sele_bg[]         = "#c8d3f5";
static const char sele_border[]     = "#ffc777";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
    /* fg bg border */
    [SchemeNorm] = { norm_fg, norm_bg, norm_bg },
    [SchemeSel]  = { sele_fg, sele_bg, sele_border },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

static const char *const autostart[] = {
    "picom", NULL,
    "slstatus", NULL,
    "redshift", NULL,
    "xautolock", "-corners", "-+-+", "-time", "30", "-locker", "slock", NULL,
    "sh", "-c", "while feh --bg-fill --randomize --recursive ~/wallpapers; do sleep 900; done", NULL,
    "sh", "-c", "command -v fcitx5 > /dev/null && fcitx5", NULL,
    NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "", "󰘅", "󰘑", "", ""};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class          instance    title       tags mask     isfloating   monitor */
    { "firefox",      NULL,       NULL,       1 << 1,       0,           -1 },
    { "QQ",           NULL,       NULL,       1 << 2,       1,           -1 },
    { "wechat",       NULL,       NULL,       1 << 3,       1,           -1 },
    { "VirtualBox",   NULL,       NULL,       1 << 4,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55f; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "󱗼",      tile },    /* first entry is default */
    { "󱡛",      NULL },    /* no layout function means floating behavior */
    { "[M]",    monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run",
    "-c",
    "-bw", "3",
    "-l", "9",
    "-m", dmenumon,
    "-fn", dmenufont,
    "-nb", norm_bg,
    "-nf", norm_fg,
    "-sb", sele_bg,
    "-sf", sele_fg,
    "-nhb", norm_bg,
    "-nhf", hi_fg,
    "-shb", sele_bg,
    "-shf", hi_fg,
    NULL,
};
static const char *termcmd[]  = { "st", "-f", stfont, NULL };
static const char *inclight[] = { "brightnessctl", "set", "+5%", NULL };
static const char *declight[] = { "brightnessctl", "set", "5%-", NULL };
static const char *incvolumn[] = { "pamixer", "-i", "5", NULL };
static const char *decvolumn[] = { "pamixer", "-d", "5", NULL };
static const char *mute[] = { "pamixer", "--toggle-mute", NULL };
static const char *screenshotcmd[] = {
    "sh", "-c",
    "sleep 0.2; scrot -s -e 'xclip -selection clipboard -t \"image/png\" < $f && rm $f'",
    NULL,
};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
    { MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
    { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    /* custom keymaps */
    { 0,                            XF86XK_MonBrightnessUp,   spawn, {.v = inclight} },
    { 0,                            XF86XK_MonBrightnessDown, spawn, {.v = declight} },
    { 0,                            XF86XK_AudioRaiseVolume,  spawn, {.v = incvolumn} },
    { 0,                            XF86XK_AudioLowerVolume,  spawn, {.v = decvolumn} },
    { 0,                            XF86XK_AudioMute,         spawn, {.v = mute} },
    { MODKEY|ShiftMask,             XK_s,                     spawn, {.v = screenshotcmd} },
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

