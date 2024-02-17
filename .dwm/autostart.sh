fcitx5 -d
picom -b
slstatus &
while feh --bg-fill --randomize --recursive ~/.wallpaper;
do
    sleep 600
done &
xautolock -time 30 -locker slock
