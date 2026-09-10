# Plush

```
 ____  _            _     
|  _ \| |_   _  __ | |__  
| |_) | | | | |/ _)| '_ \ 
|  __/| | |_| |\_ \| | | |
|_|   |_|\__,_|(__/|_| |_| 
 
  P e r s o n a l i z e d
    L i n e   U s e r
         S H e l l
```

**Plush** is a customizable, minimal, and cozy unix shell, designed for users who love simplicity, retro vibes, and terminal elegance.  
  
The whole shell is written in `c` from scratch, and some performance and memory comparison (with `bash` mainly) will be made soon.  

### Why Plush ?
Because Bash was tough, Zsh was smart, Fish was friendly...  
But Plush? Plush is yours.  

---

### Themes preview
**Powerline**
```bash
┌─(root)─[~/Projects/Plush]─{main (up-to-date)}
└─> echo "Hello, Plush 🛋️"
```

**Emoji**
```bash
(👤 you - 📁 ~/Projects/Plush - 🌿 main) ⮞ echo "Simple, customizable, yours."
```

**Minimal**
```bash
you:~/Projects/Plush $> echo "For thoses who prefer efficiency !"
```

---

## Building
You can build the shell from the source code with `make`. 
You will need a C compiler (like `clang` or `gcc`).

The `make` command should return an executable named `plush`.

For developpement purpose, a makefile is in the `src/` directory with more targets.
If run from this directory, the executable will stay in the `src/` directory and will be named `main`.

More targets are availables in this makefile (like `make valgrind` or `make valgrind-full` which will check the memory allocations with the [valgrind](https://valgrind.org/) tool).  
In this makefile, the debug flag (`-g`) is also enabled by default. 


## Project status
Alpha version. (working with lots of bugs and missing features)  

## Acknowledgements
The input and line editing is fully handled by the open-source [linenoise](https://github.com/antirez/linenoise) library.  
Go check their work !

## Contributing
All PRs are welcome !
- Add feature
- Fix bug
- Add theme
- Improve doc

## License
Plush is licensed under GPL-3.0 (see **[LICENSE](LICENSE)**).

`linenoise.c` and `linenoise.h` are vendored from linenoise and remain under BSD-2-Clause — see the license header in those files