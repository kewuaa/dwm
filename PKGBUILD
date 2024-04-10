pkgname=dwm
pkgver="0.1.0"
pkgrel=1
arch=("x86_64")
depends=(
    "libxft"
    "libx11"
    "libxrender"
    "libxinerama"
    "freetype2"
    "fontconfig"
    "xorg-server"
)
makedepends=("gcc" "make")


build() {
    cd ${srcdir}/..
    make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11
}


package() {
    depends+=(
        # default terminal
        "st"
        # status bar content
        "slstatus"
        # menu
        "dmenu"
        # screen locker
        "slock"
        "xautolock"
        # font
        "ttf-firacode-nerd"
        # alpha needed
        "picom"
        # background image
        "feh"
        # adjust light
        "brightnessctl"
        # adjust volume
        "pamixer"
        # xorg init
        "xorg-xinit"
        "xorg-xset"
        "xorg-xsetroot"
    )
    optdepends=("xorg-xprop")

    cd ${srcdir}/..
    make DESTDIR="${pkgdir}" PREFIX="/usr" MANPREFIX="/usr/local/man" install
}
