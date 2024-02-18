fcitx5 -d
picom -b
slstatus &
while feh --bg-fill --randomize --recursive ~/.wallpaper;
do
    sleep 600
done &
xautolock -corners -+-+ -time 20 -locker slock
