# pidp11-2.11bsd

Using the historical Unix 2.11BSC operating system on the pidp-11

The [pidp11](http://obsolescence.wixsite.com/obsolescence/pidp-11) is a replica of the
historical [PDP-11](http://pdp-11.nl), using a Raspberry Pi and the emulator
[simh](https://en.wikipedia.org/wiki/SIMH).

![Alt text](pidp11.jpg?raw=true "pidp11")

One of the available historical Unix systems included is [2.11BSD](https://en.wikipedia.org/wiki/Berkeley_Software_Distribution).

In this repository you can find examples of using 2.11BSD.

**Adding a user**

Login as root. Make an entry in /etc/passwd, using the special vipw
version of the vi editor`. _firstname_ is the first name of the user,
_fullname_ is the full name.

```
   vipw
```
I propose to use the following line:

  _firstname_::201:20:_fullname_:/home/_firstname_:/bin/tcsh  

Make a directory for your user with the name

```
   cd /home
   mkdir firstname
   chown firstname firstname
```

Copy some standard files into the home directory of the new user

```
   cd /home/user
   cp .* /home/firstname
   cd /home/firstname
   chown firstname .*
```

Èdit the .login file to make a better prompt:

```
   vi .login
```
Change the set prompt... line to:

   set prompt='%n@%m:%c$ '

Logout with

```
   logout
```

Login with the newly created user, then change the password with

```
  passwd
```

**Compiling a C program**

**Compiling a Pascal program**

