# fermiLisp
### _a portable lisp interpreter_

[![Support Ukraine!](https://img.shields.io/badge/Support-Ukraine-005BBB?labelColor=FFD500)](https://novaukraine.org/)

**fermiLisp** is a **free** public-domain Scheme interpreter made mainly for
FermiOS, but portable to basically any operating system that can build C99.

It is written in pure C99 without extensions and supports most stuff you'd expect form a basic
Scheme system, and can be embedded on many applications.

## Building

Use `make`, Both `gmake` **and** `bmake` work, but `bmake` is preferred. You'll
only need a C compiler.

Run:

```sh
$ make
cc ...
cc ....
```

If everything goes OK, a `./flisp` executable will appear in the directory where
fermiLisp was made, if you run it you will get a REPL where you can play around
with the system. To iunstall it, run:

```sh
# make install
```

> **Note**: the `#` signs means you must run the command as root: with sudo, doas,
> su, ...

You can uninstall fermiLisp at any time with:

```sh
# make uninstall
```

## Contributing

The repository is on [GitHub](https://github.com/cajald/fermilisp).

Use pull requests to suggest changes, and issues to report bugs.

## License

CC0. See the [LICENSE](./LICENSE) file for more information. Warranty:

> Affirmer offers the Work as-is and makes no representations or warranties of
> any kind concerning the Work, express, implied, statutory or otherwise, including
> without limitation warranties of title, merchantability, fitness for a particular
> purpose, non infringement, or the absence of latent or other defects, accuracy, or
> the present or absence of errors, whether or not discoverable, all to the greatest
> extent permissible under applicable law.

Originally written by [**Mario Rosell `<mario AT mariorosell DOT es>`**](mailto:mario@mariorosell.es).

