# off-orbit-gb

You'll need SDCC 3.8.0 and [gbdk-n](https://github.com/andreasjhkarlsson/gbdk-n) installed.

SDCC has prebuilt installers but gbdk-n needs its makefile run.

make.bat needs to point at the location gbdk-n is located and SDCC needs to be in your path.

TODO: This needs updating! Should be using make-lcc now, or remove sdcc version.


---

## Music

Create the output.c file:

```
$ make-lcc music
```

Move output.c to src/output.c and comment out or remove the pragma line so it doesn't use banking.

> The gbt_player files are modified to disable banking since a 32KB cart doesn't have banking support.
