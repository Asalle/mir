# Getting Involved in Mir

## Getting involved

The Mir project website is <https://mir-server.io/>,
the code is [hosted on GitHub](https://github.com/MirServer)

For announcements and other discussions on Mir see:
[Mir on community.ubuntu](https://community.ubuntu.com/c/mir)

For other questions and discussion about the Mir project:
the [\#mir-server](https://web.libera.chat/?channels=#mir-server) IRC channel on Libera.Chat.


## Getting Mir source and dependencies

You can get the source with:

    git clone https://github.com/MirServer/mir.git
    cd mir

You may need to install git for the system you are working on.

You’ll also need a few development tools installed. The exact dependencies and
instructions vary across distros.

###  On Ubuntu

    sudo apt-get build-dep ./

### On Fedora and Alpine

As we build these distros in Mir's CI you can copy the instructions
from the corresponding files under `spread/build`.

## Building

    cmake -S . -Bbuild
    cd build
    cmake --build .

This creates an example shell (miral-shell) in the bin directory. This can be
run directly:

    bin/miral-shell

With the default options this runs in a window on X (which is convenient for
development).

The miral-shell example is simple, don’t expect to see a sophisticated launcher
by default. You can start mir apps from the command-line. For example:

    bin/miral-run qterminal

To exit from miral-shell press Ctrl-Alt-BkSp.

You can install the Mir examples, headers and libraries you've built with:

    sudo cmake --build . --target install

## Contributing to Mir

Please file bug reports at: https://github.com/MirServer/mir/issues

The Mir development mailing list can be found at: https://lists.ubuntu.com/mailman/listinfo/Mir-devel

```{raw} html
The Mir coding guidelines are <a href=../_static/cppguide/>here</a>.
```

## Working on code
 - Hacking guidelines can be found here: [Mir Hacking Guides](https://github.com/MirServer/mir/blob/main/HACKING.md)
 - You can configure *Mir* to provide runtime information helpful for debugging
   by enabling component reports: [Component Reports](component_reports.md)
 - A guide on versioning Mir DSOs: [DSO Versioning Guide](dso_versioning_guide.md)
