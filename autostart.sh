fcitx5 -d
slstatus &
while feh --bg-fill --randomize --recursive ~/.wallpaper;
do
    sleep 600
done &
xautolock -time 30 -locker slock
